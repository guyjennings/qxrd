/******************************************************************
*
*  $Id: qxrdimageplot.cpp,v 1.4 2010/10/22 21:44:26 jennings Exp $
*
*******************************************************************/

#include "qxrdimageplot.h"
#include "qxrdrasterdata.h"
#include "qxrdplotzoomer.h"
#include "qxrdplotslicer.h"
#include "qxrdimageplotmeasurer.h"
#include "qxrdcenterfinder.h"
#include "qxrdcenterfinderpicker.h"
#include "qxrddataprocessor.h"
#include "qxrdmaskpicker.h"

#include <qwt_plot_rescaler.h>
#include <qwt_plot_marker.h>
#include <qwt_legend.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_scale_widget.h>
#include <qwt_double_range.h>

#include <QTime>

QxrdImagePlot::QxrdImagePlot(QWidget *parent)
  : QxrdPlot(parent),
    m_DisplayMinimumPct(this, "displayMinimumPct", 0),
    m_DisplayMaximumPct(this, "displayMaximumPct", 100),
    m_DisplayMinimumVal(this, "displayMinimumVal", 0),
    m_DisplayMaximumVal(this, "displayMaximumVal", 10000),
    m_DisplayMinimumPctle(this, "displayMinimumPercentile", 0),
    m_DisplayMaximumPctle(this, "displayMaximumPercentile", 100),
    m_DisplayScalingMode(this, "displayScalingMode", 0),
    m_DisplayColorMap(this, "displayColorMap", 0),
    m_ImageShown(this, "imageShown", 1),
    m_MaskShown(this, "maskShown", 0),
    m_OverflowShown(this, "overflowShown", 0),
    m_InterpolatePixels(this, "interpolatePixels", 1),
    m_MaintainAspectRatio(this, "maintainAspectRatio", 1),
    m_XMouse(this,"xMouse",0),
    m_YMouse(this,"yMouse",0),
    m_TTHMouse(this,"tthMouse",0),
    m_ValMouse(this,"valMouse",0),
    m_MaskMouse(this,"maskMouse",0),
    m_Rescaler(NULL),
    m_Slicer(NULL),
    m_Measurer(NULL),
//    m_Legend(NULL),
    m_Data(NULL),
    m_Mask(NULL),
    m_Overflow(NULL),
    m_DataImage(NULL),
    m_MaskImage(NULL),
    m_OverflowImage(NULL),
//    m_PlotImage(NULL),
    m_ColorMap(Qt::black, Qt::white),
    m_MaskColorMap(Qt::red, QColor(0,0,0,0)),
    m_MaskAlpha(80),
    m_OverflowColorMap(QColor(0,0,0,0), Qt::green),
    m_OverflowAlpha(256),
    m_DataProcessor(NULL),
    m_CenterFinderPicker(NULL),
    m_CenterMarker(NULL),
    m_Circles(NULL),
    m_Polygons(NULL),
    m_FirstTime(true),
    SOURCE_IDENT("$Id: qxrdimageplot.cpp,v 1.4 2010/10/22 21:44:26 jennings Exp $")
{
  delete m_Zoomer;

  m_Zoomer = QxrdPlotZoomerPtr(new QxrdImagePlotZoomer(QwtPlotCanvasPtr(canvas()), QxrdImagePlotPtr(this)));
  m_Zoomer -> setSelectionFlags(QwtPicker::DragSelection | QwtPicker::CornerToCorner);
  m_Zoomer -> setTrackerMode(QwtPicker::AlwaysOn);
  m_Zoomer -> setRubberBand(QwtPicker::RectRubberBand);

  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect2,
                              Qt::LeftButton, Qt::ControlModifier | Qt::ShiftModifier);
  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect3,
                              Qt::LeftButton, Qt::ControlModifier);

  m_Zoomer -> setEnabled(true);

  m_Rescaler = QwtPlotRescalerPtr(new QwtPlotRescaler(canvas(), QwtPlot::xBottom, QwtPlotRescaler::Expanding));
  m_Rescaler -> setEnabled(true);

  m_Slicer = QxrdPlotSlicerPtr(new QxrdPlotSlicer(QwtPlotCanvasPtr(canvas()), QxrdImagePlotPtr(this)));
  m_Slicer -> setEnabled(false);

  m_Measurer = QxrdImagePlotMeasurerPtr(new QxrdImagePlotMeasurer(QwtPlotCanvasPtr(canvas()), QxrdImagePlotPtr(this)));
  m_Measurer -> setEnabled(false);

//  m_Legend = QwtLegendPtr(new QwtLegend(this));
  m_Legend -> setFrameStyle(QFrame::Box|QFrame::Sunken);
  m_Legend -> setItemMode(QwtLegend::CheckableItem);

  insertLegend(m_Legend, QwtPlot::BottomLegend);

  m_DataImage = QwtPlotSpectrogramPtr(new QwtPlotSpectrogram());
  m_DataImage -> attach(this);

  m_MaskImage = QwtPlotSpectrogramPtr(new QwtPlotSpectrogram());
  m_MaskImage -> setAlpha(get_MaskShown() ? m_MaskAlpha : 0);
  m_MaskImage -> attach(this);

  m_OverflowImage = QwtPlotSpectrogramPtr(new QwtPlotSpectrogram());
  m_OverflowImage -> setAlpha(get_OverflowShown() ? m_OverflowAlpha : 0);
  m_OverflowImage -> attach(this);

//  m_PlotImage = QxrdPlotImagePtr(new QxrdPlotImage());
//  m_PlotImage -> attach(this);

  m_CenterFinderPicker = QxrdCenterFinderPickerPtr(new QxrdCenterFinderPicker(QxrdImagePlotPtr(this)));

  m_CenterMarker = QwtPlotMarkerPtr(new QwtPlotMarker());
  m_CenterMarker -> setLineStyle(QwtPlotMarker::Cross);
  m_CenterMarker -> attach(this);

  m_Circles = QxrdCircularMaskPickerPtr(new QxrdCircularMaskPicker(QwtPlotCanvasPtr(canvas()), QxrdImagePlotPtr(this)));
  m_Circles -> setEnabled(false);

  m_Polygons = QxrdPolygonalMaskPickerPtr(new QxrdPolygonalMaskPicker(QwtPlotCanvasPtr(canvas()), QxrdImagePlotPtr(this)));
  m_Polygons -> setEnabled(false);

  set100Range();
  setGrayscale();

  connect(prop_ImageShown(), SIGNAL(changedValue(bool)), this, SLOT(changeImageShown(bool)));
  connect(prop_MaskShown(), SIGNAL(changedValue(bool)), this, SLOT(changeMaskShown(bool)));
  connect(prop_DisplayMinimumPct(), SIGNAL(changedValue(double)), this, SLOT(recalculateDisplayedRange()));
  connect(prop_DisplayMaximumPct(), SIGNAL(changedValue(double)), this, SLOT(recalculateDisplayedRange()));
  connect(prop_DisplayMinimumVal(), SIGNAL(changedValue(double)), this, SLOT(recalculateDisplayedRange()));
  connect(prop_DisplayMaximumVal(), SIGNAL(changedValue(double)), this, SLOT(recalculateDisplayedRange()));
  connect(prop_DisplayMinimumPctle(), SIGNAL(changedValue(double)), this, SLOT(recalculateDisplayedRange()));
  connect(prop_DisplayMaximumPctle(), SIGNAL(changedValue(double)), this, SLOT(recalculateDisplayedRange()));
  connect(prop_DisplayScalingMode(), SIGNAL(changedValue(int)), this, SLOT(recalculateDisplayedRange()));
  connect(prop_InterpolatePixels(), SIGNAL(changedValue(bool)), this, SLOT(onInterpolateChanged(bool)));
  connect(prop_MaintainAspectRatio(), SIGNAL(changedValue(bool)), this, SLOT(onMaintainAspectChanged(bool)));
  connect(prop_DisplayColorMap(), SIGNAL(changedValue(int)), this, SLOT(setColorMap(int)));

  enableZooming();
}

void QxrdImagePlot::setDataProcessor(QxrdDataProcessorPtr proc)
{
  m_DataProcessor = proc;

  connect(m_CenterFinderPicker, SIGNAL(selected(QwtDoublePoint)),
          m_DataProcessor -> centerFinder(), SLOT(onCenterChanged(QwtDoublePoint)));

  connect(m_Circles, SIGNAL(selected(QwtDoubleRect)),
          m_DataProcessor, SLOT(maskCircle(QwtDoubleRect)));

  connect(m_Polygons, SIGNAL(selected(QwtArray<QwtDoublePoint>)),
          m_DataProcessor, SLOT(maskPolygon(QwtArray<QwtDoublePoint>)));

  connect(m_Measurer, SIGNAL(selected(QwtArray<QwtDoublePoint>)),
          m_DataProcessor, SLOT(measurePolygon(QwtArray<QwtDoublePoint>)));

  connect(m_Slicer, SIGNAL(selected(QwtArray<QwtDoublePoint>)),
          m_DataProcessor, SLOT(slicePolygon(QwtArray<QwtDoublePoint>)));
}

QxrdDataProcessorPtr QxrdImagePlot::processor() const
{
  return m_DataProcessor;
}

void QxrdImagePlot::readSettings(QxrdSettings &settings, QString section)
{
  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdImagePlot::writeSettings(QxrdSettings &settings, QString section)
{
  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdImagePlot::setAutoRange()
{
  emit criticalMessage("QxrdImagePlot::setAutoRange To do...");
}

void QxrdImagePlot::set005Range()
{
  set_DisplayMinimumPct(0);
  set_DisplayMaximumPct(5);
}

void QxrdImagePlot::set010Range()
{
  set_DisplayMinimumPct(0);
  set_DisplayMaximumPct(10);
}

void QxrdImagePlot::set100Range()
{
  set_DisplayMinimumPct(0);
  set_DisplayMaximumPct(100);
}

void QxrdImagePlot::recalculateDisplayedRange()
{
  double mindis, maxdis;

  if (get_DisplayScalingMode() == PercentageMode) {
    double minv = m_DataRaster.minValue();
    double maxv = m_DataRaster.maxValue();
    double del = maxv-minv;

    mindis = minv+del*get_DisplayMinimumPct()/100.0;
    maxdis = minv+del*get_DisplayMaximumPct()/100.0;
  } else if (get_DisplayScalingMode() == PercentileMode) {
    QwtDoubleInterval range = m_DataRaster.percentileRange(get_DisplayMinimumPctle(), get_DisplayMaximumPctle());

    mindis = range.minValue();
    maxdis = range.maxValue();
  } else {
    mindis = get_DisplayMinimumVal();
    maxdis = get_DisplayMaximumVal();
  }

  m_DataRaster.setDisplayedRange(mindis, maxdis);

  replotImage();
}

void QxrdImagePlot::replotImage()
{
  m_DataImage -> setData(m_DataRaster);

  m_DataImage -> invalidateCache();
  m_DataImage -> itemChanged();

  if (m_FirstTime) {
    autoScale();
    m_FirstTime = false;
  } else {
    replot();
  }
}

void QxrdImagePlot::onInterpolateChanged(bool interp)
{
//  printf("QxrdImagePlot::onInterpolateChanged(%d)\n", interp);

  m_DataRaster.setInterpolate(interp);

  replotImage();
}

void QxrdImagePlot::onMaintainAspectChanged(bool interp)
{
//  printf("QxrdImagePlot::onMaintainAspectChanged(%d)\n", interp);

  if (m_Rescaler) {
    m_Rescaler -> setEnabled(interp);
  }

  replotImage();
}

void QxrdImagePlot::setTrackerPen(const QPen &pen)
{
  m_Zoomer -> setTrackerPen(pen);
  m_Zoomer -> setRubberBandPen(pen);
  m_CenterFinderPicker -> setTrackerPen(pen);
  m_CenterFinderPicker -> setRubberBandPen(pen);
  m_Circles  -> setTrackerPen(pen);
  m_Circles  -> setRubberBandPen(pen);
  m_Polygons -> setTrackerPen(pen);
  m_Polygons -> setRubberBandPen(pen);
  m_Measurer -> setTrackerPen(pen);
  m_Measurer -> setRubberBandPen(pen);
  m_Slicer   -> setTrackerPen(pen);
  m_Slicer   -> setRubberBandPen(pen);

  if (m_CenterMarker) {
    m_CenterMarker -> setLinePen(pen);
  }

  m_MaskColorMap.setColorInterval(pen.color(), QColor(0,0,0,0));
}

void QxrdImagePlot::setGrayscale()
{
  m_ColorMap.setColorInterval(Qt::black, Qt::white);

  setTrackerPen(QPen(Qt::red));

  changedColorMap();
}

void QxrdImagePlot::setInverseGrayscale()
{
  m_ColorMap.setColorInterval(Qt::white, Qt::black);

  setTrackerPen(QPen(Qt::red));

  changedColorMap();
}

void QxrdImagePlot::setEarthTones()
{
  m_ColorMap.setColorInterval(Qt::black, Qt::white);
  m_ColorMap.addColorStop(0.15, Qt::blue);
  m_ColorMap.addColorStop(0.25, Qt::gray);
  m_ColorMap.addColorStop(0.35, Qt::green);
  m_ColorMap.addColorStop(0.5, Qt::darkYellow);
  m_ColorMap.addColorStop(0.85, Qt::darkMagenta);

  setTrackerPen(QPen(Qt::red));

  changedColorMap();
}

void QxrdImagePlot::setSpectrum()
{
  m_ColorMap.setColorInterval(Qt::magenta, Qt::red);
  m_ColorMap.addColorStop(0.2, Qt::blue);
  m_ColorMap.addColorStop(0.4, Qt::cyan);
  m_ColorMap.addColorStop(0.6, Qt::green);
  m_ColorMap.addColorStop(0.8, Qt::yellow);

  setTrackerPen(QPen(Qt::black));

  changedColorMap();
}

void QxrdImagePlot::setFire()
{
  m_ColorMap.setColorInterval(Qt::black, Qt::white);
  m_ColorMap.addColorStop(0.25, Qt::red);
  m_ColorMap.addColorStop(0.75, Qt::yellow);

  setTrackerPen(QPen(Qt::blue));

  changedColorMap();
}

void QxrdImagePlot::setIce()
{
  m_ColorMap.setColorInterval(Qt::black, Qt::white);
//  m_ColorMap.addColorStop(0.25, Qt::darkblue);
  m_ColorMap.addColorStop(0.25, Qt::blue);
  m_ColorMap.addColorStop(0.75, Qt::cyan);

  setTrackerPen(QPen(Qt::red));

  changedColorMap();
}

void QxrdImagePlot::setColorMap(int n)
{
  switch(n) {
  case 0:
    setGrayscale();
    break;

  case 1:
    setInverseGrayscale();
    break;

  case 2:
    setEarthTones();
    break;

  case 3:
    setSpectrum();
    break;

  case 4:
    setFire();
    break;

  case 5:
    setIce();
    break;
  }
}

void QxrdImagePlot::toggleShowImage()
{
  changeImageShown(!get_ImageShown());
}

void QxrdImagePlot::changeImageShown(bool shown)
{
  set_ImageShown(shown);

  if (m_DataImage) {
    m_DataImage -> setAlpha(get_ImageShown() ? 255 : 0);
    m_DataImage -> invalidateCache();
    m_DataImage -> itemChanged();

    replotImage();
  }
}

void QxrdImagePlot::toggleShowMask()
{
  changeMaskShown(!get_MaskShown());
}

void QxrdImagePlot::changeMaskShown(bool shown)
{
  set_MaskShown(shown);

  if (m_MaskImage) {
    m_MaskImage -> setAlpha(get_MaskShown() ? m_MaskAlpha : 0);
    m_MaskImage -> invalidateCache();
    m_MaskImage -> itemChanged();

    replotImage();
  }
}

void QxrdImagePlot::toggleShowOverflow()
{
  changeOverflowShown(!get_OverflowShown());
}

void QxrdImagePlot::changeOverflowShown(bool shown)
{
  set_OverflowShown(shown);

  if (m_OverflowImage) {
    m_OverflowImage -> setAlpha(get_ImageShown() ? m_OverflowAlpha : 0);
    m_OverflowImage -> invalidateCache();
    m_OverflowImage -> itemChanged();

    replotImage();
  }
}

void QxrdImagePlot::changedColorMap()
{
  m_DataImage -> setColorMap(m_ColorMap);
  m_DataImage -> invalidateCache();
  m_DataImage -> itemChanged();

  m_MaskImage   -> setColorMap(m_MaskColorMap);
  m_MaskImage   -> invalidateCache();
  m_MaskImage   -> itemChanged();

  replotImage();
}

void QxrdImagePlot::setImage(QxrdRasterData data)
{
  m_DataRaster = data;

  m_DataImage -> setData(data);
  m_DataImage -> invalidateCache();
  m_DataImage -> itemChanged();

  recalculateDisplayedRange();
}

void QxrdImagePlot::setMask(QxrdMaskRasterData mask)
{
  m_MaskRaster = mask;

  m_MaskImage -> setData(mask);
  m_MaskImage -> invalidateCache();
  m_MaskImage -> itemChanged();

  replot();
}

void QxrdImagePlot::setOverflows(QxrdMaskDataPtr overflow)
{
  short int *ov = overflow->data();
  short int *msk = m_MaskRaster.data();

  int npix=overflow->get_Width()*overflow->get_Height();

  for (int i=0; i<npix; i++) {
    msk[i] = (msk[i] & (~2)) | (ov[i] & 2);
  }

  m_MaskImage -> setData(m_MaskRaster);
  m_MaskImage -> invalidateCache();
  m_MaskImage -> itemChanged();
}

void QxrdImagePlot::onProcessedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow)
{
  QTime tic;
  tic.start();

  m_Data = image;
  m_Overflow = overflow;

  if (!image ||
      image->get_Width() != m_DataRaster.width() ||
      image->get_Height() != m_DataRaster.height()) {
    m_FirstTime = true;
  }

  QxrdRasterData data(image, get_InterpolatePixels(), QxrdMaskDataPtr(NULL));

  if (overflow == NULL) {
    setImage(data);
  } else {
    setOverflows(overflow);
    setImage(data);
  }

  if (image) {
    setTitle(image -> get_Title());
  } else {
    setTitle("");
  }

  replotImage();

  emit printMessage(tr("plot image took %1 msec").arg(tic.elapsed()));
}

void QxrdImagePlot::onMaskedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr mask)
{
  m_Data = image;
  m_Mask = mask;

  if (!image ||
      image->get_Width() != m_DataRaster.width() ||
      image->get_Height() != m_DataRaster.height()) {
    m_FirstTime = true;
  }

  QxrdRasterData data(image, get_InterpolatePixels(), QxrdMaskDataPtr(NULL));
  QxrdMaskRasterData msk(mask, false);

  setImage(data);
  setMask(msk);

  if (image) {
    setTitle(image -> get_Title());
  } else {
    setTitle("");
  }

  replotImage();
}

void QxrdImagePlot::onDarkImageAvailable(QxrdDoubleImageDataPtr /*image*/)
{
}

void QxrdImagePlot::onCenterXChanged(double cx)
{
  m_CenterMarker -> setXValue(cx);
  replot();
}

void QxrdImagePlot::onCenterYChanged(double cy)
{
  m_CenterMarker -> setYValue(cy);
  replot();
}

void QxrdImagePlot::onCenterChanged(QwtDoublePoint c)
{
  m_CenterMarker -> setValue(c);
  replot();
}

const QxrdRasterData* QxrdImagePlot::raster() const
{
  return &m_DataRaster;
}

QxrdRasterData* QxrdImagePlot::raster()
{
  return &m_DataRaster;
}

const QxrdMaskRasterData* QxrdImagePlot::maskRaster() const
{
  return &m_MaskRaster;
}

QxrdMaskRasterData* QxrdImagePlot::maskRaster()
{
  return &m_MaskRaster;
}

void QxrdImagePlot::enableZooming()
{
  m_Zoomer       -> setEnabled(true);
  m_CenterFinderPicker -> setEnabled(false);
  m_Slicer       -> setEnabled(false);
  m_Measurer     -> setEnabled(false);
  m_Circles  -> setEnabled(false);
  m_Polygons -> setEnabled(false);
}

void QxrdImagePlot::enableCentering()
{
  m_Zoomer   -> setEnabled(false);
  m_CenterFinderPicker -> setEnabled(true);
  m_Slicer   -> setEnabled(false);
  m_Measurer -> setEnabled(false);
  m_Circles  -> setEnabled(false);
  m_Polygons -> setEnabled(false);
}

void QxrdImagePlot::enableSlicing()
{
  m_Zoomer   -> setEnabled(false);
  m_CenterFinderPicker -> setEnabled(false);
  m_Slicer   -> setEnabled(true);
  m_Measurer -> setEnabled(false);
  m_Circles  -> setEnabled(false);
  m_Polygons -> setEnabled(false);
}

void QxrdImagePlot::enableMeasuring()
{
  m_Zoomer   -> setEnabled(false);
  m_CenterFinderPicker -> setEnabled(false);
  m_Slicer   -> setEnabled(false);
  m_Measurer -> setEnabled(true);
  m_Circles  -> setEnabled(false);
  m_Polygons -> setEnabled(false);
}

void QxrdImagePlot::enableMaskCircles()
{
  m_Zoomer   -> setEnabled(false);
  m_CenterFinderPicker -> setEnabled(false);
  m_Slicer   -> setEnabled(false);
  m_Measurer -> setEnabled(false);
  m_Circles  -> setEnabled(true);
  m_Polygons -> setEnabled(false);
}

void QxrdImagePlot::enableMaskPolygons()
{
  m_Zoomer   -> setEnabled(false);
  m_CenterFinderPicker -> setEnabled(false);
  m_Slicer   -> setEnabled(false);
  m_Measurer -> setEnabled(false);
  m_Circles  -> setEnabled(false);
  m_Polygons -> setEnabled(true);
}

//#undef replot

void QxrdImagePlot::replot()
{
//  QTime tic;
//  tic.start();

  QxrdPlot::replot();

//  emit printMessage(tr("QxrdImagePlot::replot took %1 msec").arg(tic.restart()));
}

QwtText QxrdImagePlot::trackerText(const QwtDoublePoint &pos) const
{
  const QxrdRasterData *ras = this->raster();

  QxrdDataProcessorPtr processor = this->processor();
  QxrdCenterFinderPtr centerFinder = NULL;

  if (processor) {
    centerFinder = processor->centerFinder();
  }

  QString res = tr("%1, %2").arg(pos.x()).arg(pos.y());

  ((QxrdImagePlot*) this)->set_XMouse(pos.x());
  ((QxrdImagePlot*) this)->set_YMouse(pos.y());

  if (ras) {
    double val = ras->value(pos.x(),pos.y());
    res += tr(", %1").arg(val);
    ((QxrdImagePlot*) this)->set_ValMouse(val);
  }

  if (m_MaskRaster.data()) {
    double mask = m_MaskRaster.value(pos.x(),pos.y());
    res += tr(", %1").arg(mask);
    ((QxrdImagePlot*) this)->set_MaskMouse(mask);
  }

  if (centerFinder) {
    double tth = centerFinder->getTTH(pos);
    res += tr(", TTH %1").arg(tth);
    ((QxrdImagePlot*) this)->set_TTHMouse(tth);
  }

  return res;
}

/******************************************************************
*
*  $Log: qxrdimageplot.cpp,v $
*  Revision 1.4  2010/10/22 21:44:26  jennings
*  *** empty log message ***
*
*  Revision 1.3  2010/09/17 16:21:51  jennings
*  Rationalised the trackerText implementations
*
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.6  2010/09/10 21:09:52  jennings
*  Tilt and powder fitting modifications
*
*  Revision 1.1.2.5  2010/09/09 16:45:00  jennings
*  Added separate parameters for percentile display mode, changed display control
*  to only show relevent parameters
*
*  Revision 1.1.2.4  2010/09/08 20:11:57  jennings
*  Improved percentile display mode
*
*  Revision 1.1.2.3  2010/09/08 19:40:57  jennings
*  Added tilt controls to center finder
*  Disabled fit refine command (for now)
*  Added percentile display mode
*
*  Revision 1.1.2.2  2010/08/11 15:58:27  jennings
*  Added changedValue(QwtDoublePoint) slots
*
*  Revision 1.1.2.1  2010/07/22 18:39:39  jennings
*  Moving files into source subdirectory
*
*  Revision 1.42.2.8  2010/06/23 21:40:20  jennings
*  Eliminated a crash if image or mask are NULL
*
*  Revision 1.42.2.7  2010/05/09 13:42:37  jennings
*  Eliminated spurious legend on image plot caused by duplicate definition of m_Legend in QxrdImagePlot and QxrdPlot
*
*  Revision 1.42.2.6  2010/05/06 18:52:42  jennings
*  Make plot legends owned by plot
*
*  Revision 1.42.2.5  2010/05/02 16:56:50  jennings
*  Removed embedded C comments from commit log
*
*  Revision 1.42.2.4  2010/05/02 08:12:06  jennings
*  Replaced 'returnImageToPool' and 'replaceImageFromPool' by
*  equivalent smart pointer assignments
*
*  Revision 1.42.2.3  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.42.2.2  2010/04/26 20:53:26  jennings
*  More attempts to get QSharedPointers to work...
*
*  Revision 1.42.2.1  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.42  2009/10/26 02:41:20  jennings
*  Image plot updates when plot range is changed
*
*  Revision 1.41  2009/09/29 18:39:47  jennings
*  Removed references to 'QxrdDataProcessor::processedCount'
*  Fixed up the various 'status' scripting functions so that they work properly
*
*  Revision 1.40  2009/09/22 21:34:03  jennings
*  Made message pane read-only
*
*  Revision 1.39  2009/09/22 19:45:33  jennings
*  Small changes to range calculating code for image data
*
*  Revision 1.38  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
*
*  Revision 1.37  2009/09/13 13:58:09  jennings
*  Removed multi-line comment
*
*  Revision 1.36  2009/08/25 18:43:03  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.35  2009/08/12 19:44:59  jennings
*  Reorganized plot zoomers into a single class, initialized in QxrdPlot, which
*  takes its tracker text from a QxrdPlot virtual member function
*
*  Revision 1.34  2009/08/09 15:39:10  jennings
*  Added a separate QxrdImagePlotMeasurer class
*
*  Revision 1.33  2009/08/05 14:04:25  jennings
*  Turned off right hand color bar
*  Rotated left axis labels
*
*  Revision 1.32  2009/08/04 16:45:20  jennings
*  Moved mask data into separate class
*
*  Revision 1.31  2009/08/02 21:14:16  jennings
*  Added masking dummy routines
*
*  Revision 1.30  2009/08/02 18:02:42  jennings
*  Added a number of masking operations to the UI - no actual implementation yet
*
*  Revision 1.29  2009/07/25 17:03:40  jennings
*  More improvements to image plotting code
*
*  Revision 1.28  2009/07/25 15:18:39  jennings
*  Moved graph zooming code into QxrdPlot - a common base class
*  Made QxrdMaskColorMap descend from QwtLinearColorMap
*
*  Revision 1.27  2009/07/22 11:55:34  jennings
*  Center finder modifications
*
*  Revision 1.26  2009/07/21 22:55:48  jennings
*  Rearranged center finder and integrator code so that the center finder and integrator objects go into the data processor thread, and the GUI stuff goes in the GUI thread
*
*  Revision 1.25  2009/07/20 00:35:23  jennings
*  Trying to optimise screen redraws
*
*  Revision 1.24  2009/07/17 20:41:20  jennings
*  Modifications related to mask display
*
*  Revision 1.23  2009/07/16 21:26:25  jennings
*  Made various image display variables into properties
*
*  Revision 1.22  2009/07/16 20:10:43  jennings
*  Made various image display variables into properties
*
*  Revision 1.21  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.20  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

