/******************************************************************
*
*  $Id: qxrdimageplot.cpp,v 1.34 2009/08/09 15:39:10 jennings Exp $
*
*******************************************************************/

#include "qxrdimageplot.h"
#include "qxrdrasterdata.h"
#include "qxrdplotzoomer.h"
#include "qxrdplottracker.h"
#include "qxrdplotslicer.h"
#include "qxrdimageplotmeasurer.h"
#include "qxrdcenterfinder.h"
#include "qxrdcenterfinderpicker.h"
#include "qxrddataprocessor.h"
#include "qxrdmaskpicker.h"

//#include <qwt_plot_zoomer.h>
//#include <qwt_plot_panner.h>
//#include <qwt_plot_magnifier.h>
#include <qwt_plot_rescaler.h>
#include <qwt_plot_marker.h>
#include <qwt_legend.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_scale_widget.h>
//#include <QMetaMethod>
#include <QTime>

//#define replot() { \
//  printf("replot() called from %s:%d\n", __FILE__, __LINE__); \
//  re##plot(); \
//}

QxrdImagePlot::QxrdImagePlot(QWidget *parent)
  : QxrdPlot(true, true, parent),
    m_DisplayMinimumPct(this, "displayMinimumPct", 0),
    m_DisplayMaximumPct(this, "displayMaximumPct", 100),
    m_DisplayMinimumVal(this, "displayMinimumVal", 0),
    m_DisplayMaximumVal(this, "displayMaximumVal", 10000),
    m_DisplayScalingMode(this, "displayScalingMode", 0),
    m_DisplayColorMap(this, "displayColorMap", 0),
    m_ImageShown(this, "imageShown", 1),
    m_MaskShown(this, "maskShown", 0),
    m_InterpolatePixels(this, "interpolatePixels", 1),
    m_MaintainAspectRatio(this, "maintainAspectRatio", 1),
    m_Rescaler(NULL),
    m_Slicer(NULL),
    m_Measurer(NULL),
    m_Legend(NULL),
    m_Spectrogram(NULL),
    m_MaskImage(NULL),
    m_ColorMap(Qt::black, Qt::white),
    m_MaskColorMap(Qt::red, QColor(0,0,0,0)),
    m_MaskAlpha(80),
    m_DataProcessor(NULL),
    m_CenterFinderPicker(NULL),
    m_CenterMarker(NULL),
    m_Circles(NULL),
    m_Polygons(NULL),
    m_FirstTime(true),
    SOURCE_IDENT("$Id: qxrdimageplot.cpp,v 1.34 2009/08/09 15:39:10 jennings Exp $")
{
  setCustomTracker(new QxrdPlotTracker(canvas(), this));
  setCustomZoomer(new QxrdPlotZoomer(canvas(), this));

  m_Rescaler = new QwtPlotRescaler(canvas(), QwtPlot::xBottom, QwtPlotRescaler::Expanding);
  m_Rescaler -> setEnabled(true);

  m_Slicer = new QxrdPlotSlicer(canvas(), this);
  m_Slicer -> setEnabled(false);

  m_Measurer = new QxrdImagePlotMeasurer(canvas(), this);
  m_Measurer -> setEnabled(false);

  m_Legend = new QwtLegend;
  m_Legend -> setFrameStyle(QFrame::Box|QFrame::Sunken);
  m_Legend -> setItemMode(QwtLegend::CheckableItem);

  insertLegend(m_Legend, QwtPlot::BottomLegend);

  m_Spectrogram = new QwtPlotSpectrogram();
  m_Spectrogram -> attach(this);

  m_MaskImage = new QwtPlotSpectrogram();
  m_MaskImage -> setAlpha(get_MaskShown() ? m_MaskAlpha : 0);
  m_MaskImage -> attach(this);

  m_CenterFinderPicker = new QxrdCenterFinderPicker(this);

  m_CenterMarker = new QwtPlotMarker();
  m_CenterMarker -> setLineStyle(QwtPlotMarker::Cross);
  m_CenterMarker -> attach(this);

  m_Circles = new QxrdCircularMaskPicker(canvas(), this);
  m_Circles -> setEnabled(false);

  m_Polygons = new QxrdPolygonalMaskPicker(canvas(), this);
  m_Polygons -> setEnabled(false);

  set100Range();
  setGrayscale();

  connect(prop_ImageShown(), SIGNAL(changedValue(bool)), this, SLOT(changeImageShown(bool)));
  connect(prop_MaskShown(), SIGNAL(changedValue(bool)), this, SLOT(changeMaskShown(bool)));
  connect(prop_DisplayMinimumPct(), SIGNAL(changedValue(double)), this, SLOT(recalculateDisplayedRange()));
  connect(prop_DisplayMaximumPct(), SIGNAL(changedValue(double)), this, SLOT(recalculateDisplayedRange()));
  connect(prop_DisplayMinimumVal(), SIGNAL(changedValue(double)), this, SLOT(recalculateDisplayedRange()));
  connect(prop_DisplayMaximumVal(), SIGNAL(changedValue(double)), this, SLOT(recalculateDisplayedRange()));
  connect(prop_DisplayScalingMode(), SIGNAL(changedValue(int)), this, SLOT(recalculateDisplayedRange()));
  connect(prop_InterpolatePixels(), SIGNAL(changedValue(bool)), this, SLOT(onInterpolateChanged(bool)));
  connect(prop_MaintainAspectRatio(), SIGNAL(changedValue(bool)), this, SLOT(onMaintainAspectChanged(bool)));
  connect(prop_DisplayColorMap(), SIGNAL(changedValue(int)), this, SLOT(setColorMap(int)));

  enableZooming();
}

void QxrdImagePlot::setDataProcessor(QxrdDataProcessor *proc)
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

void QxrdImagePlot::readSettings(QxrdSettings *settings, QString section)
{
  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdImagePlot::writeSettings(QxrdSettings *settings, QString section)
{
  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdImagePlot::setAutoRange()
{
  printf("To do...\n");
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
    double minv = m_Raster.minValue();
    double maxv = m_Raster.maxValue();
    double del = maxv-minv;

    mindis = minv+del*get_DisplayMinimumPct()/100.0;
    maxdis = minv+del*get_DisplayMaximumPct()/100.0;
  } else {
    mindis = get_DisplayMinimumVal();
    maxdis = get_DisplayMaximumVal();
  }

  m_Raster.setDisplayedRange(mindis, maxdis);

//  QwtScaleWidget *rightAxis = axisWidget(QwtPlot::yRight);
//  setAxisScale(QwtPlot::yRight, mindis, maxdis);
//  rightAxis -> setColorBarEnabled(true);
//  rightAxis -> setColorMap(m_Spectrogram->data().range(),
//                           m_Spectrogram->colorMap());
//
//  replotImage();
}

void QxrdImagePlot::replotImage()
{
  m_Spectrogram -> setData(m_Raster);

  m_Spectrogram -> invalidateCache();
  m_Spectrogram -> itemChanged();

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

  m_Raster.setInterpolate(interp);

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
  m_Tracker -> setTrackerPen(pen);
  m_Tracker -> setRubberBandPen(pen);
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

  if (m_Spectrogram) {
    m_Spectrogram -> setAlpha(get_ImageShown() ? 255 : 0);
    m_Spectrogram -> invalidateCache();
    m_Spectrogram -> itemChanged();

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

void QxrdImagePlot::changedColorMap()
{
  m_Spectrogram -> setColorMap(m_ColorMap);
  m_Spectrogram -> invalidateCache();
  m_Spectrogram -> itemChanged();

  m_MaskImage   -> setColorMap(m_MaskColorMap);
  m_MaskImage   -> invalidateCache();
  m_MaskImage   -> itemChanged();

//  QwtScaleWidget *rightAxis = axisWidget(QwtPlot::yRight);
//  rightAxis -> setColorBarEnabled(true);
//  rightAxis -> setColorMap(m_Spectrogram->data().range(),
//                           m_Spectrogram->colorMap());

  replotImage();
}

void QxrdImagePlot::setImage(QxrdRasterData data)
{
  m_Raster = data;

  m_Spectrogram -> setData(data);
//  m_Spectrogram -> setColorMap(m_ColorMap);
//  m_Spectrogram -> setAlpha(get_ImageShown() ? 255 : 0);
  m_Spectrogram -> invalidateCache();
  m_Spectrogram -> itemChanged();

//  QwtScaleWidget *rightAxis = axisWidget(QwtPlot::yRight);
//  rightAxis -> setColorBarEnabled(true);
//  rightAxis -> setColorMap(QwtDoubleInterval(0,1),
//                           m_Spectrogram->colorMap());

   enableAxis(QwtPlot::yRight);

  QwtDoubleInterval range = m_Raster.range();

  setAxisScale(QwtPlot::yRight, m_Raster.minValue(), m_Raster.maxValue());

//   setDisplayedRange(0,100);

  recalculateDisplayedRange();

//  replot();
}

void QxrdImagePlot::setMask(QxrdMaskRasterData mask)
{
  m_MaskRaster = mask;

  m_MaskImage -> setData(mask);
//  m_MaskImage -> setColorMap(m_MaskColorMap);
//  m_MaskImage -> setAlpha(get_MaskShown() ? m_MaskAlpha : 0);
  m_MaskImage -> invalidateCache();
  m_MaskImage -> itemChanged();
//
//  replot();
}

void QxrdImagePlot::onProcessedImageAvailable(QxrdImageData *image)
{
  if (!image ||
      image->get_Width() != m_Raster.width() ||
      image->get_Height() != m_Raster.height()) {
    m_FirstTime = true;
  }

  m_DataProcessor -> decrementProcessedCount();

  QxrdRasterData data(image, get_InterpolatePixels());

  setImage(data);

  setTitle(image -> get_Title());

  replotImage();
}

void QxrdImagePlot::onMaskedImageAvailable(QxrdImageData *image, QxrdMaskData *mask)
{
  if (!image ||
      image->get_Width() != m_Raster.width() ||
      image->get_Height() != m_Raster.height()) {
    m_FirstTime = true;
  }

  m_DataProcessor -> decrementProcessedCount();

  QxrdRasterData data(image, get_InterpolatePixels());
  QxrdMaskRasterData msk(mask, get_InterpolatePixels());

  setImage(data);
  setMask(msk);

  setTitle(image -> get_Title());

  replotImage();
}

void QxrdImagePlot::onDarkImageAvailable(QxrdImageData *image)
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

QxrdRasterData* QxrdImagePlot::raster()
{
  return &m_Raster;
}

void QxrdImagePlot::enableZooming()
{
  m_Tracker      -> setEnabled(true);
  m_Zoomer       -> setEnabled(true);
  m_CenterFinderPicker -> setEnabled(false);
  m_Slicer       -> setEnabled(false);
  m_Measurer     -> setEnabled(false);
  m_Circles  -> setEnabled(false);
  m_Polygons -> setEnabled(false);
}

void QxrdImagePlot::enableCentering()
{
  m_Tracker  -> setEnabled(false);
  m_Zoomer   -> setEnabled(false);
  m_CenterFinderPicker -> setEnabled(true);
  m_Slicer   -> setEnabled(false);
  m_Measurer -> setEnabled(false);
  m_Circles  -> setEnabled(false);
  m_Polygons -> setEnabled(false);
}

void QxrdImagePlot::enableSlicing()
{
  m_Tracker  -> setEnabled(false);
  m_Zoomer   -> setEnabled(false);
  m_CenterFinderPicker -> setEnabled(false);
  m_Slicer   -> setEnabled(true);
  m_Measurer -> setEnabled(false);
  m_Circles  -> setEnabled(false);
  m_Polygons -> setEnabled(false);
}

void QxrdImagePlot::enableMeasuring()
{
  m_Tracker  -> setEnabled(false);
  m_Zoomer   -> setEnabled(false);
  m_CenterFinderPicker -> setEnabled(false);
  m_Slicer   -> setEnabled(false);
  m_Measurer -> setEnabled(true);
  m_Circles  -> setEnabled(false);
  m_Polygons -> setEnabled(false);
}

void QxrdImagePlot::enableMaskCircles()
{
  m_Tracker  -> setEnabled(false);
  m_Zoomer   -> setEnabled(false);
  m_CenterFinderPicker -> setEnabled(false);
  m_Slicer   -> setEnabled(false);
  m_Measurer -> setEnabled(false);
  m_Circles  -> setEnabled(true);
  m_Polygons -> setEnabled(false);
}

void QxrdImagePlot::enableMaskPolygons()
{
  m_Tracker  -> setEnabled(false);
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
  QTime tic;
  tic.start();

  QxrdPlot::replot();

  printf("QxrdImagePlot::replot took %d msec\n", tic.restart());
}

/******************************************************************
*
*  $Log: qxrdimageplot.cpp,v $
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

