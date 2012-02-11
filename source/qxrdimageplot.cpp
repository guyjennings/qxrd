#include "qxrdimageplot.h"
#include "qxrdrasterdata.h"
#include "qxrdplotzoomer.h"
#include "qxrdplotslicer.h"
#include "qxrdimageplotmeasurer.h"
#include "qxrdcenterfinder.h"
#include "qxrdcenterfinderpicker.h"
#include "qxrddataprocessor.h"
#include "qxrdmaskpicker.h"
#include "qxrdhistogramselector.h"
#include "qxrdapplication.h"

#include <qwt_plot_rescaler.h>
#include <qwt_plot_marker.h>
#include <qwt_legend.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_scale_widget.h>
#include <qwt_double_range.h>

#include <QTime>
#include <QMenu>
#include <QContextMenuEvent>

QxrdImagePlot::QxrdImagePlot(QWidget *parent)
  : QxrdPlot(parent),
    m_DisplayMinimumPct(QxrdSettingsSaverPtr(), this, "displayMinimumPct", 0),
    m_DisplayMaximumPct(QxrdSettingsSaverPtr(), this, "displayMaximumPct", 100),
    m_DisplayMinimumVal(QxrdSettingsSaverPtr(), this, "displayMinimumVal", 0),
    m_DisplayMaximumVal(QxrdSettingsSaverPtr(), this, "displayMaximumVal", 10000),
    m_DisplayMinimumPctle(QxrdSettingsSaverPtr(), this, "displayMinimumPercentile", 0),
    m_DisplayMaximumPctle(QxrdSettingsSaverPtr(), this, "displayMaximumPercentile", 100),
    m_DisplayScalingMode(QxrdSettingsSaverPtr(), this, "displayScalingMode", 0),
    m_DisplayColorMap(QxrdSettingsSaverPtr(), this, "displayColorMap", 0),
    m_ImageShown(QxrdSettingsSaverPtr(), this, "imageShown", 1),
    m_MaskShown(QxrdSettingsSaverPtr(), this, "maskShown", 0),
    m_OverflowShown(QxrdSettingsSaverPtr(), this, "overflowShown", 0),
    m_InterpolatePixels(QxrdSettingsSaverPtr(), this, "interpolatePixels", 1),
    m_MaintainAspectRatio(QxrdSettingsSaverPtr(), this, "maintainAspectRatio", 1),
    m_TTHMouse(QxrdSettingsSaverPtr(), this,"tthMouse",0),
    m_QMouse(QxrdSettingsSaverPtr(), this,"qMouse",0),
    m_ValMouse(QxrdSettingsSaverPtr(), this,"valMouse",0),
    m_MaskMouse(QxrdSettingsSaverPtr(), this,"maskMouse",0),
    m_Rescaler(NULL),
    m_Slicer(NULL),
    m_Measurer(NULL),
    m_HistogramSelector(NULL),
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
    m_DataProcessor(),
    m_CenterFinderPicker(NULL),
    m_CenterMarker(NULL),
    m_Circles(NULL),
    m_Polygons(NULL),
    m_FirstTime(true)
{
  delete m_Zoomer;

  m_Zoomer = new QxrdImagePlotZoomer(canvas(), this);
  m_Zoomer -> setSelectionFlags(QwtPicker::DragSelection | QwtPicker::CornerToCorner);
  m_Zoomer -> setTrackerMode(QwtPicker::AlwaysOn);
  m_Zoomer -> setRubberBand(QwtPicker::RectRubberBand);

  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect2,
                              Qt::LeftButton, Qt::ControlModifier | Qt::ShiftModifier);
  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect3,
                              Qt::LeftButton, Qt::ControlModifier);

  m_Zoomer -> setEnabled(true);

  m_Rescaler = new QwtPlotRescaler(canvas(), QwtPlot::xBottom, QwtPlotRescaler::Expanding);
  m_Rescaler -> setEnabled(true);

  m_Slicer = new QxrdPlotSlicer(canvas(), this);
  m_Slicer -> setEnabled(false);

  m_Measurer = new QxrdImagePlotMeasurer(canvas(), this);
  m_Measurer -> setEnabled(false);

  m_HistogramSelector = new QxrdHistogramSelector(canvas(), this);
  m_HistogramSelector -> setEnabled(false);

//  m_Legend = QwtLegendPtr(new QwtLegend(this));
  m_Legend -> setFrameStyle(QFrame::Box|QFrame::Sunken);
  m_Legend -> setItemMode(QwtLegend::CheckableItem);

  insertLegend(m_Legend, QwtPlot::BottomLegend);

  m_DataImage = new QwtPlotSpectrogram();
  m_DataImage -> attach(this);

  m_MaskImage = new QwtPlotSpectrogram();
  m_MaskImage -> setAlpha(get_MaskShown() ? m_MaskAlpha : 0);
  m_MaskImage -> attach(this);

  m_OverflowImage = new QwtPlotSpectrogram();
  m_OverflowImage -> setAlpha(get_OverflowShown() ? m_OverflowAlpha : 0);
  m_OverflowImage -> attach(this);

//  m_PlotImage = QxrdPlotImagePtr(new QxrdPlotImage());
//  m_PlotImage -> attach(this);

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

  connect(prop_ImageShown(), SIGNAL(valueChanged(bool,int)), this, SLOT(changeImageShown(bool)));
  connect(prop_MaskShown(), SIGNAL(valueChanged(bool,int)), this, SLOT(changeMaskShown(bool)));
  connect(prop_OverflowShown(), SIGNAL(valueChanged(bool,int)), this, SLOT(changeOverflowShown(bool)));
  connect(prop_DisplayMinimumPct(), SIGNAL(valueChanged(double,int)), this, SLOT(recalculateDisplayedRange()));
  connect(prop_DisplayMaximumPct(), SIGNAL(valueChanged(double,int)), this, SLOT(recalculateDisplayedRange()));
  connect(prop_DisplayMinimumVal(), SIGNAL(valueChanged(double,int)), this, SLOT(recalculateDisplayedRange()));
  connect(prop_DisplayMaximumVal(), SIGNAL(valueChanged(double,int)), this, SLOT(recalculateDisplayedRange()));
  connect(prop_DisplayMinimumPctle(), SIGNAL(valueChanged(double,int)), this, SLOT(recalculateDisplayedRange()));
  connect(prop_DisplayMaximumPctle(), SIGNAL(valueChanged(double,int)), this, SLOT(recalculateDisplayedRange()));
  connect(prop_DisplayScalingMode(), SIGNAL(valueChanged(int,int)), this, SLOT(recalculateDisplayedRange()));
  connect(prop_InterpolatePixels(), SIGNAL(valueChanged(bool,int)), this, SLOT(onInterpolateChanged(bool)));
  connect(prop_MaintainAspectRatio(), SIGNAL(valueChanged(bool,int)), this, SLOT(onMaintainAspectChanged(bool)));
  connect(prop_DisplayColorMap(), SIGNAL(valueChanged(int,int)), this, SLOT(setColorMap(int)));

  enableZooming();
}

void QxrdImagePlot::setProcessor(QxrdDataProcessorWPtr proc)
{
  m_DataProcessor = proc;

  QxrdDataProcessorPtr dp(m_DataProcessor);

  if (dp) {
    QxrdCenterFinderPtr cf(dp->centerFinder());
    connect(m_CenterFinderPicker, SIGNAL(selected(QwtDoublePoint)),
            cf.data(), SLOT(onCenterChanged(QwtDoublePoint)));

    connect(m_Circles, SIGNAL(selected(QwtDoubleRect)),
            dp.data(), SLOT(maskCircle(QwtDoubleRect)));

    connect(m_Polygons, SIGNAL(selected(QwtArray<QwtDoublePoint>)),
            dp.data(), SLOT(maskPolygon(QwtArray<QwtDoublePoint>)));

    connect(m_Measurer, SIGNAL(selected(QwtArray<QwtDoublePoint>)),
            dp.data(), SLOT(measurePolygon(QwtArray<QwtDoublePoint>)));

    //  connect(m_Slicer, SIGNAL(selected(QwtArray<QwtDoublePoint>)),
    //          m_DataProcessor, SLOT(slicePolygon(QwtArray<QwtDoublePoint>)));
  }

  connect(m_Slicer, SIGNAL(selected(QwtArray<QwtDoublePoint>)),
          this, SIGNAL(slicePolygon(QwtArray<QwtDoublePoint>)));

  connect(m_HistogramSelector, SIGNAL(selected(QwtDoubleRect)),
          this, SIGNAL(selectHistogram(QwtDoubleRect)));
}

void QxrdImagePlot::setSaver(QxrdSettingsSaverPtr saver)
{
  QxrdPlot::setSaver(saver);

  prop_ImageShown()->setSaver(saver);
  prop_MaskShown()->setSaver(saver);
  prop_OverflowShown()->setSaver(saver);
  prop_DisplayMinimumPct()->setSaver(saver);
  prop_DisplayMaximumPct()->setSaver(saver);
  prop_DisplayMinimumVal()->setSaver(saver);
  prop_DisplayMaximumVal()->setSaver(saver);
  prop_DisplayMinimumPctle()->setSaver(saver);
  prop_DisplayMaximumPctle()->setSaver(saver);
  prop_DisplayScalingMode()->setSaver(saver);
  prop_InterpolatePixels()->setSaver(saver);
  prop_MaintainAspectRatio()->setSaver(saver);
  prop_DisplayColorMap()->setSaver(saver);
}

QxrdDataProcessorWPtr QxrdImagePlot::processor() const
{
  return m_DataProcessor;
}

void QxrdImagePlot::readSettings(QSettings *settings, QString section)
{
  QxrdPlot::readSettings(settings, section);
  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdImagePlot::writeSettings(QSettings *settings, QString section)
{
  QxrdPlot::writeSettings(settings, section);
  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdImagePlot::setAutoRange()
{
  if (g_Application) {
    g_Application->criticalMessage("QxrdImagePlot::setAutoRange To do...");
  }
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
  m_HistogramSelector   -> setTrackerPen(pen);
  m_HistogramSelector   -> setRubberBandPen(pen);

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
    m_OverflowImage -> setAlpha(get_OverflowShown() ? m_OverflowAlpha : 0);
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

  m_OverflowImage   -> setColorMap(m_OverflowColorMap);
  m_OverflowImage   -> invalidateCache();
  m_OverflowImage   -> itemChanged();

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

void QxrdImagePlot::setOverflows(QxrdMaskRasterData overflow)
{
  m_OverflowRaster = overflow;

  m_OverflowImage -> setData(overflow);
  m_OverflowImage -> invalidateCache();
  m_OverflowImage -> itemChanged();

  replot();
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

  if (g_Application) {
    g_Application->printMessage(tr("plot image took %1 msec").arg(tic.elapsed()));
  }
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
  m_HistogramSelector -> setEnabled(false);
  m_Circles  -> setEnabled(false);
  m_Polygons -> setEnabled(false);
}

void QxrdImagePlot::enableCentering()
{
  m_Zoomer   -> setEnabled(false);
  m_CenterFinderPicker -> setEnabled(true);
  m_Slicer   -> setEnabled(false);
  m_Measurer -> setEnabled(false);
  m_HistogramSelector -> setEnabled(false);
  m_Circles  -> setEnabled(false);
  m_Polygons -> setEnabled(false);
}

void QxrdImagePlot::enableSlicing()
{
  m_Zoomer   -> setEnabled(false);
  m_CenterFinderPicker -> setEnabled(false);
  m_Slicer   -> setEnabled(true);
  m_Measurer -> setEnabled(false);
  m_HistogramSelector -> setEnabled(false);
  m_Circles  -> setEnabled(false);
  m_Polygons -> setEnabled(false);
}

void QxrdImagePlot::enableMeasuring()
{
  m_Zoomer   -> setEnabled(false);
  m_CenterFinderPicker -> setEnabled(false);
  m_Slicer   -> setEnabled(false);
  m_Measurer -> setEnabled(true);
  m_HistogramSelector -> setEnabled(false);
  m_Circles  -> setEnabled(false);
  m_Polygons -> setEnabled(false);
}

void QxrdImagePlot::enableHistograms()
{
  m_Zoomer   -> setEnabled(false);
  m_CenterFinderPicker -> setEnabled(false);
  m_Slicer   -> setEnabled(false);
  m_Measurer -> setEnabled(false);
  m_HistogramSelector -> setEnabled(true);
  m_Circles  -> setEnabled(false);
  m_Polygons -> setEnabled(false);
}

void QxrdImagePlot::enableMaskCircles()
{
  m_Zoomer   -> setEnabled(false);
  m_CenterFinderPicker -> setEnabled(false);
  m_Slicer   -> setEnabled(false);
  m_Measurer -> setEnabled(false);
  m_HistogramSelector -> setEnabled(false);
  m_Circles  -> setEnabled(true);
  m_Polygons -> setEnabled(false);
}

void QxrdImagePlot::enableMaskPolygons()
{
  m_Zoomer   -> setEnabled(false);
  m_CenterFinderPicker -> setEnabled(false);
  m_Slicer   -> setEnabled(false);
  m_Measurer -> setEnabled(false);
  m_HistogramSelector -> setEnabled(false);
  m_Circles  -> setEnabled(false);
  m_Polygons -> setEnabled(true);
}

//#undef replot

void QxrdImagePlot::replot()
{
//  QTime tic;
//  tic.start();

  QxrdPlot::replot();

//  g_Application->printMessage(tr("QxrdImagePlot::replot took %1 msec").arg(tic.restart()));
}

QwtText QxrdImagePlot::trackerText(const QwtDoublePoint &pos)
{
  const QxrdRasterData *ras = this->raster();

  QxrdDataProcessorPtr processor = this->processor();
  QxrdCenterFinderPtr centerFinder;

  if (processor) {
    centerFinder = processor->centerFinder();
  }

  QString res = tr("%1, %2").arg(pos.x()).arg(pos.y());

  set_XMouse(pos.x());
  set_YMouse(pos.y());

  if (ras) {
    double val = ras->value(pos.x(),pos.y());
    res += tr(", %1").arg(val);
    set_ValMouse(val);
  }

  if (m_MaskRaster.data()) {
    double mask = m_MaskRaster.value(pos.x(),pos.y());
    res += tr(", %1").arg(mask);
    set_MaskMouse(mask);
  }

  if (centerFinder) {
    double tth = centerFinder->getTTH(pos);
    res += tr("\nTTH %1").arg(tth);
    set_TTHMouse(tth);

    double q = centerFinder->getQ(pos);
    res += tr(", Q %1").arg(q);
    set_QMouse(q);
  }

  return res;
}

void QxrdImagePlot::contextMenuEvent(QContextMenuEvent * /*event*/)
{
//  QMenu plotMenu(NULL, NULL);

////  QAction *xLog = plotMenu.addAction("Log X Axis");
////  QAction *yLog = plotMenu.addAction("Log Y Axis");
//  QAction *auSc = plotMenu.addAction("Autoscale");

////  xLog->setCheckable(true);
////  yLog->setCheckable(true);
////  xLog->setChecked(get_XAxisLog());
////  yLog->setChecked(get_YAxisLog());

//  QAction *action = plotMenu.exec(event->globalPos());

////  if (action == xLog) {
////    set_XAxisLog(!get_XAxisLog());
////  } else if (action == yLog) {
////    set_YAxisLog(!get_YAxisLog());
///*  } else*/ if (action == auSc) {
//    autoScale();
//  }

//  event->accept();
}
