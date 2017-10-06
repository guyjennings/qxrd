#include "qxrddebug.h"
#include "qxrdimageplot.h"
#include "qxrdrasterdata.h"
#include "qxrdmaskrasterdata.h"
#include "qxrdoverflowrasterdata.h"
#include "qxrdplotzoomer.h"
#include "qxrdplotslicer.h"
#include "qxrdimageplotmeasurer.h"
#include "qxrdcenterfinder.h"
#include "qxrdcenterfinderpicker.h"
#include "qxrddataprocessor.h"
#include "qxrdmaskpicker.h"
#include "qxrdhistogramselector.h"
#include "qxrdapplication.h"
#include "qxrdallocator.h"
#include "qxrddataprocessor.h"
#include "qxrdacquisition.h"
#include <qwt_plot_rescaler.h>
#include <qwt_plot_marker.h>
#include <qwt_legend.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_scale_widget.h>
#include <qwt_symbol.h>
#include <qwt_picker_machine.h>
#include <QTime>
#include <QMenu>
#include <QContextMenuEvent>
#include <QtConcurrentRun>
#include "qxrdplotcurvevector.h"
#include "qwt_plot_piecewise_curve.h"
#include "qcepmutexlocker.h"

QxrdImagePlot::QxrdImagePlot(QWidget *parent)
  : QxrdPlot(parent),
    m_ObjectNamer(this, "imageGraph"),
    m_ImagePlotSettings(),
    m_Rescaler(NULL),
    m_Slicer(NULL),
    m_Measurer(NULL),
    m_HistogramSelector(NULL),
    m_Data(NULL),
    m_Mask(NULL),
    m_DataProcessor(),
    m_CenterFinderPicker(NULL),
    m_Circles(NULL),
    m_Polygons(NULL),
    m_PowderPointPicker(NULL),
    m_FirstTime(true),
    m_ContextMenuEnabled(true),
    m_MinDisplayedValue(0),
    m_MaxDisplayedValue(100)
{
  qRegisterMetaType<QwtPlotSpectrogramPtr>("QwtPlotSpectrogramPtr");

  connect(this, &QxrdImagePlot::newPlotSpectrograms,
          this, &QxrdImagePlot::onNewPlotSpectrograms,
          Qt::QueuedConnection);
}

void QxrdImagePlot::init(QxrdImagePlotSettingsWPtr settings)
{
  QxrdPlot::init(settings);

  m_ImagePlotSettings = settings;

  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  delete m_Zoomer;

  m_Zoomer = new QxrdImagePlotZoomer(canvas(), this);
  m_Zoomer -> setStateMachine(new QwtPickerDragRectMachine());
  m_Zoomer -> setTrackerMode(QwtPicker::AlwaysOn);
  m_Zoomer -> setRubberBand(QwtPicker::RectRubberBand);

  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect2,
                              Qt::LeftButton, Qt::ControlModifier | Qt::ShiftModifier);
  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect3,
                              Qt::LeftButton, Qt::ControlModifier);

  m_Zoomer -> setEnabled(true);

  m_Rescaler = new QwtPlotRescaler(canvas(), QwtPlot::yLeft, QwtPlotRescaler::Expanding);
  m_Rescaler -> setEnabled(true);
  m_Rescaler -> setExpandingDirection(QwtPlotRescaler::ExpandBoth);

  m_Slicer = new QxrdPlotSlicer(canvas(), this);
  m_Slicer -> setEnabled(false);

  m_Measurer = new QxrdImagePlotMeasurer(canvas(), this);
  m_Measurer -> setEnabled(false);

  m_HistogramSelector = new QxrdHistogramSelector(canvas(), this);
  m_HistogramSelector -> setEnabled(false);

  m_Legend -> setFrameStyle(QFrame::Box|QFrame::Sunken);
  m_Legend -> setDefaultItemMode(QwtLegendData::Checkable);

  insertLegend(m_Legend, QwtPlot::BottomLegend);

  m_CenterFinderPicker = new QxrdCenterFinderPicker(this);

  m_Circles = new QxrdCircularMaskPicker(canvas(), this);
  m_Circles -> setEnabled(false);

  m_Polygons = new QxrdPolygonalMaskPicker(canvas(), this);
  m_Polygons -> setEnabled(false);

  m_PowderPointPicker = new QxrdPowderPointPicker(this);
  m_PowderPointPicker -> setEnabled(false);

  set100Range();

  if (set) {
    connect(m_Zoomer, SIGNAL(zoomed(QRectF)), this, SLOT(onImageScaleChanged()));
    connect(set->prop_ImageShown(), SIGNAL(valueChanged(bool,int)), this, SLOT(changeImageShown(bool)));
    connect(set->prop_MaskShown(), SIGNAL(valueChanged(bool,int)), this, SLOT(changeMaskShown(bool)));
    connect(set->prop_OverflowShown(), SIGNAL(valueChanged(bool,int)), this, SLOT(changeOverflowShown(bool)));
    connect(set->prop_DisplayMinimumPct(), SIGNAL(valueChanged(double,int)), this, SLOT(recalculateDisplayedRange()));
    connect(set->prop_DisplayMaximumPct(), SIGNAL(valueChanged(double,int)), this, SLOT(recalculateDisplayedRange()));
    connect(set->prop_DisplayMinimumVal(), SIGNAL(valueChanged(double,int)), this, SLOT(recalculateDisplayedRange()));
    connect(set->prop_DisplayMaximumVal(), SIGNAL(valueChanged(double,int)), this, SLOT(recalculateDisplayedRange()));
    connect(set->prop_DisplayMinimumPctle(), SIGNAL(valueChanged(double,int)), this, SLOT(recalculateDisplayedRange()));
    connect(set->prop_DisplayMaximumPctle(), SIGNAL(valueChanged(double,int)), this, SLOT(recalculateDisplayedRange()));
    connect(set->prop_DisplayScalingMode(), SIGNAL(valueChanged(int,int)), this, SLOT(recalculateDisplayedRange()));
    connect(set->prop_InterpolatePixels(), SIGNAL(valueChanged(bool,int)), this, SLOT(onInterpolateChanged(bool)));
    connect(set->prop_MaintainAspectRatio(), SIGNAL(valueChanged(bool,int)), this, SLOT(onMaintainAspectChanged(bool)));
    connect(set->prop_DisplayColorMap(), SIGNAL(valueChanged(int,int)), this, SLOT(redoColorMap()));
    connect(set->prop_DisplayLog(), SIGNAL(valueChanged(bool,int)), this, SLOT(redoColorMap()));

    changeImageShown(set->get_ImageShown());
    changeMaskShown(set->get_MaskShown());
    changeOverflowShown(set->get_OverflowShown());
    recalculateDisplayedRange();
    onInterpolateChanged(set->get_InterpolatePixels());
    onMaintainAspectChanged(set->get_MaintainAspectRatio());
    redoColorMap();
  }

  enableZooming();

  onImageScaleChanged();
}

void QxrdImagePlot::setProcessor(QxrdDataProcessorWPtr proc)
{
  m_DataProcessor = proc;

  QxrdDataProcessorPtr dp(m_DataProcessor);

  if (dp) {
    QxrdCenterFinderPtr cf(dp->centerFinder());

    if (cf) {
      connect(m_CenterFinderPicker, SIGNAL(selected(QPointF)),
              cf.data(), SLOT(onCenterChanged(QPointF)));

      connect(m_Circles, SIGNAL(selected(QRectF)),
              dp.data(), SLOT(maskCircle(QRectF)));

      connect(m_Polygons, SIGNAL(selected(QVector<QPointF>)),
              dp.data(), SLOT(maskPolygon(QVector<QPointF>)));

      connect(m_Measurer, SIGNAL(selected(QVector<QPointF>)),
              dp.data(), SLOT(measurePolygon(QVector<QPointF>)));

      //  connect(m_Slicer, SIGNAL(selected(QVector<QPointF>)),
    //          m_DataProcessor, SLOT(slicePolygon(QVector<QPointF>)));

      connect(m_PowderPointPicker, SIGNAL(selected(QPointF)),
              cf.data(), SLOT(onPointSelected(QPointF)));

      onCenterChanged(QPointF(cf->get_CenterX(), cf->get_CenterY()));

      connect(cf->prop_MarkedPoints(), SIGNAL(valueChanged(QxrdPowderPointVector,int)),
              this, SLOT(onMarkedPointsChanged()));

      onMarkedPointsChanged();
    }
  }

  connect(m_Slicer, SIGNAL(selected(QVector<QPointF>)),
          this, SIGNAL(slicePolygon(QVector<QPointF>)));

  connect(m_HistogramSelector, SIGNAL(selected(QRectF)),
          this, SIGNAL(selectHistogram(QRectF)));
}

QxrdDataProcessorWPtr QxrdImagePlot::processor() const
{
  return m_DataProcessor;
}

QxrdImagePlotSettingsWPtr QxrdImagePlot::imagePlotSettings()
{
  return m_ImagePlotSettings;
}

void QxrdImagePlot::autoScale()
{
  inherited::autoScale();

  onImageScaleChanged();
}

void QxrdImagePlot::setAutoRange()
{
  if (g_Application) {
    g_Application->criticalMessage("QxrdImagePlot::setAutoRange To do...");
  }
}

void QxrdImagePlot::set005Range()
{
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    set->set_DisplayMinimumPct(0);
    set->set_DisplayMaximumPct(5);
  }
}

void QxrdImagePlot::set010Range()
{
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    set->set_DisplayMinimumPct(0);
    set->set_DisplayMaximumPct(10);
  }
}

void QxrdImagePlot::set100Range()
{
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    set->set_DisplayMinimumPct(0);
    set->set_DisplayMaximumPct(100);
  }
}

void QxrdImagePlot::recalculateDisplayedRange()
{
  replotImage();
}

void QxrdImagePlot::calculateDisplayedRange()
{
  QxrdDoubleImageDataPtr   data = m_Data;
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set && data) {
    double mindis, maxdis;

    if (set->get_DisplayScalingMode() == PercentageMode) {
      double minv = data->get_MinValue();
      double maxv = data->get_MaxValue();
      double del = maxv-minv;

      mindis = minv+del*set->get_DisplayMinimumPct()/100.0;
      maxdis = minv+del*set->get_DisplayMaximumPct()/100.0;
    } else if (set->get_DisplayScalingMode() == PercentileMode) {
      QcepInterval range = data->percentileRange(set->get_DisplayMinimumPctle(), set->get_DisplayMaximumPctle());

      mindis = range.minValue;
      maxdis = range.maxValue;
    } else {
      mindis = set->get_DisplayMinimumVal();
      maxdis = set->get_DisplayMaximumVal();
    }

    m_MinDisplayedValue = mindis;
    m_MaxDisplayedValue = maxdis;

  }
}

void QxrdImagePlot::replotImage()
{
  QtConcurrent::run(this, &QxrdImagePlot::updateImagePlot);
}

void QxrdImagePlot::onInterpolateChanged(bool interp)
{
  replotImage();
}

void QxrdImagePlot::onMaintainAspectChanged(bool interp)
{
  //  printf("QxrdImagePlot::onMaintainAspectChanged(%d)\n", interp);

  if (m_Rescaler) {
    m_Rescaler -> setEnabled(interp);
  }

  onImageScaleChanged();

  replotImage();
}

void QxrdImagePlot::setColorMap(int n)
{
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    set -> set_DisplayColorMap(n);
  }
}

void QxrdImagePlot::setGrayscale()
{
  setColorMap(0);
}

void QxrdImagePlot::setInverseGrayscale()
{
  setColorMap(1);
}

void QxrdImagePlot::setEarthTones()
{
  setColorMap(2);
}

void QxrdImagePlot::setSpectrum()
{
  setColorMap(3);
}

void QxrdImagePlot::setFire()
{
  setColorMap(4);
}

void QxrdImagePlot::setIce()
{
  setColorMap(5);
}

QPen QxrdImagePlot::markerPen()
{
  return m_Pen;
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
  m_PowderPointPicker   -> setTrackerPen(pen);
  m_PowderPointPicker   -> setRubberBandPen(pen);

  m_Pen = pen;
}

void QxrdImagePlot::colorMapStart(QwtLinearColorMap* map, QColor startColor, QColor endColor)
{
  map->setColorInterval(startColor, endColor);
}

void QxrdImagePlot::colorMapRange(QwtLinearColorMap *map, double value1, QColor color1, double value2, QColor color2)
{
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set && set->get_DisplayLog()) {
    int n1 = int(value1*100);
    int n2 = int(value2*100);
    double r1 = color1.redF();
    double r2 = color2.redF();
    double g1 = color1.greenF();
    double g2 = color2.greenF();
    double b1 = color1.blueF();
    double b2 = color2.blueF();

    for (int n=n1; n<n2; n++) {
      double pos = double(n)/100.0;
      double val = (pow(10.0, pos) - 1.0)/9.0;
      double interp = (pos-value1)/(value2-value1);

      QColor col = QColor::fromRgbF(r1 + (r2-r1)*interp, g1 + (g2 - g1)*interp, b1 + (b2 - b1)*interp);

      map->addColorStop(val, col);
    }
  } else {
    map->addColorStop(value1, color1);
  }
}

QwtColorMap *QxrdImagePlot::newImageColorMap()
{
  QwtLinearColorMap *map =
      new QwtLinearColorMap(Qt::black, Qt::white);

  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (map && set) {
    switch (set->get_DisplayColorMap()) {
    case 0:
      setGrayscaleMap(map);
      break;

    case 1:
      setInverseGrayscaleMap(map);
      break;

    case 2:
      setEarthTonesMap(map);
      break;

    case 3:
      setSpectrumMap(map);
      break;

    case 4:
      setFireMap(map);
      break;

    case 5:
      setIceMap(map);
      break;
    }
  }

  return map;
}

void QxrdImagePlot::setGrayscaleMap(QwtLinearColorMap *map)
{
  colorMapStart(map, Qt::black, Qt::white);
  colorMapRange(map, 0.0, Qt::black, 1.0, Qt::white);

  setTrackerPen(QPen(Qt::red));
}

void QxrdImagePlot::setInverseGrayscaleMap(QwtLinearColorMap *map)
{
  colorMapStart(map, Qt::white, Qt::black);
  colorMapRange(map, 0.0, Qt::white, 1.0, Qt::black);

  setTrackerPen(QPen(Qt::red));
}

void QxrdImagePlot::setEarthTonesMap(QwtLinearColorMap *map)
{
  colorMapStart(map, Qt::black, Qt::white);

  colorMapRange(map, 0.0, Qt::black, 0.15, Qt::blue);
  colorMapRange(map, 0.15, Qt::blue, 0.25, Qt::gray);
  colorMapRange(map, 0.25, Qt::gray, 0.35, Qt::green);
  colorMapRange(map, 0.35, Qt::green, 0.5, Qt::darkYellow);
  colorMapRange(map, 0.5, Qt::darkYellow, 0.85, Qt::darkMagenta);
  colorMapRange(map, 0.85, Qt::darkMagenta, 1.0, Qt::white);

  setTrackerPen(QPen(Qt::red));
}

void QxrdImagePlot::setSpectrumMap(QwtLinearColorMap *map)
{
  colorMapStart(map, Qt::magenta, Qt::red);

  colorMapRange(map, 0.0, Qt::magenta,0.2, Qt::blue);
  colorMapRange(map, 0.2, Qt::blue,   0.4, Qt::cyan);
  colorMapRange(map, 0.4, Qt::cyan,   0.6, Qt::green);
  colorMapRange(map, 0.6, Qt::green,  0.8, Qt::yellow);
  colorMapRange(map, 0.8, Qt::yellow, 1.0, Qt::red);

  setTrackerPen(QPen(Qt::black));
}

void QxrdImagePlot::setFireMap(QwtLinearColorMap *map)
{
  colorMapStart(map, Qt::black, Qt::white);

  colorMapRange(map, 0.0,  Qt::black,  0.25, Qt::red);
  colorMapRange(map, 0.25, Qt::red,    0.75, Qt::yellow);
  colorMapRange(map, 0.75, Qt::yellow, 1.0,  Qt::white);

  setTrackerPen(QPen(Qt::blue));
}

void QxrdImagePlot::setIceMap(QwtLinearColorMap *map)
{
  colorMapStart(map, Qt::black, Qt::white);

  colorMapRange(map, 0.0,  Qt::black, 0.25, Qt::blue);
  colorMapRange(map, 0.25, Qt::blue,  0.75, Qt::cyan);
  colorMapRange(map, 0.75, Qt::cyan,  1.0,  Qt::white);

  setTrackerPen(QPen(Qt::red));
}

void QxrdImagePlot::redoColorMap()
{
  replotImage();
}

void QxrdImagePlot::toggleShowImage()
{
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    changeImageShown(!set->get_ImageShown());
  }
}

void QxrdImagePlot::changeImageShown(bool shown)
{
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    set->set_ImageShown(shown);

    replotImage();
  }
}

void QxrdImagePlot::toggleShowMask()
{
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    changeMaskShown(!set->get_MaskShown());
  }
}

void QxrdImagePlot::changeMaskShown(bool shown)
{
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    set->set_MaskShown(shown);

    replotImage();
  }
}

void QxrdImagePlot::toggleShowOverflow()
{
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    changeOverflowShown(!set->get_OverflowShown());
  }
}

void QxrdImagePlot::changeOverflowShown(bool shown)
{
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    set->set_OverflowShown(shown);

    replotImage();
  }
}

double QxrdImagePlot::overflowLevel()
{
  double res = 60000;

  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    QxrdAcquisitionPtr acq = proc->acquisition();

    if (acq) {
      res = acq->get_OverflowLevel();
    }
  }

  return res;
}

void QxrdImagePlot::onProcessedImageAvailable(QxrdDoubleImageDataPtr image)
{
  if (g_Application && qcepDebug(DEBUG_DISPLAY)) {
    g_Application->printMessage(tr("QxrdImagePlot::onProcessedImageAvailable starting %1")
                                .arg(image?image->get_Title():"NULL"));
  }

  m_Data     = image;

  replotImage();
}

void QxrdImagePlot::updateImagePlot()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_UpdateMutex);

  int c = m_UpdateCount.fetchAndAddOrdered(1);

  QxrdDoubleImageDataPtr image    = m_Data;
  QxrdMaskDataPtr        mask     = m_Mask;

  if (g_Application && (qcepDebug(DEBUG_DISPLAY) || (c > 0))) {
    g_Application->printMessage(tr("QxrdImagePlot::updateImagePlot starting %1 depth %2")
                                .arg(image?image->get_Title():"NULL").arg(c));
  }

  QTime tic;
  tic.start();

  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  calculateDisplayedRange();

  if (set) {
    QwtPlotSpectrogramPtr dataSpectrogram;

    if (set->get_ImageShown()) {
      dataSpectrogram =
          QwtPlotSpectrogramPtr(
            new QwtPlotSpectrogram("image"));

      dataSpectrogram -> setRenderThreadCount(0);
      dataSpectrogram -> setColorMap(newImageColorMap());

      QxrdRasterData     *dataRaster = new QxrdRasterData(image, set->get_InterpolatePixels(), QxrdMaskDataPtr(NULL));

      dataRaster -> setDisplayedRange(m_MinDisplayedValue, m_MaxDisplayedValue);

      dataSpectrogram -> setData(dataRaster);
    }

    QwtPlotSpectrogramPtr maskSpectrogram;

    if (set->get_MaskShown()) {
      maskSpectrogram =
        QwtPlotSpectrogramPtr(
          new QwtPlotSpectrogram("mask"));

      maskSpectrogram -> setAlpha(80);
      maskSpectrogram -> setRenderThreadCount(0);
      maskSpectrogram -> setColorMap(new QxrdMaskColorMap(Qt::red, QColor(0,0,0,0)));

      QxrdMaskRasterData *maskRaster = new QxrdMaskRasterData(mask);

      maskSpectrogram -> setData(maskRaster);
    }

    QwtPlotSpectrogramPtr overflowSpectrogram;

    if (set->get_OverflowShown()) {
      overflowSpectrogram =
        QwtPlotSpectrogramPtr(
          new QwtPlotSpectrogram("overflow"));

      overflowSpectrogram -> setAlpha(256);
      overflowSpectrogram -> setRenderThreadCount(0);
      overflowSpectrogram -> setColorMap(new QxrdMaskColorMap(QColor(0,0,0,0), Qt::green));

      QxrdOverflowRasterData *overflowRaster =
          new QxrdOverflowRasterData(image, overflowLevel());

      overflowSpectrogram -> setData(overflowRaster);
    }

    QxrdPlotCurveVectorPtr powderCurves =
        QxrdPlotCurveVectorPtr(
          new QxrdPlotCurveVector());

    displayPowderMarkers(powderCurves);

    emit newPlotSpectrograms(dataSpectrogram,
                             maskSpectrogram,
                             overflowSpectrogram,
                             powderCurves);

    if (g_Application && qcepDebug(DEBUG_DISPLAY)) {
      g_Application->printMessage(tr("QxrdImagePlot::updateImagePlot finished %1 after %2 msec depth %3")
                                  .arg(image?image->get_Title():"NULL").arg(tic.elapsed()).arg(c));
    }
  }

  m_UpdateCount.fetchAndSubOrdered(1);
}

void QxrdImagePlot::onNewPlotSpectrograms(QwtPlotSpectrogramPtr  image,
                                          QwtPlotSpectrogramPtr  mask,
                                          QwtPlotSpectrogramPtr  overflow,
                                          QxrdPlotCurveVectorPtr powderCurves)
{
  m_DataSpectrogram     = image;
  m_MaskSpectrogram     = mask;
  m_OverflowSpectrogram = overflow;
  m_PowderPointCurves   = powderCurves;

  detachItems();

  if (m_DataSpectrogram) {
    m_DataSpectrogram -> attach(this);
  }

  if (m_MaskSpectrogram) {
    m_MaskSpectrogram -> attach(this);
  }

  if (m_OverflowSpectrogram) {
    m_OverflowSpectrogram -> attach(this);
  }

  QwtPlotMarker *centerMarker = new QwtPlotMarker();
  centerMarker -> setLineStyle(QwtPlotMarker::Cross);
  centerMarker -> setValue(m_CenterX, m_CenterY);
  centerMarker -> setLinePen(markerPen());

  centerMarker -> attach(this);

  if (m_PowderPointCurves) {
    for (int i=0; i<m_PowderPointCurves->count(); i++) {
      QwtPlotPiecewiseCurve *c = m_PowderPointCurves->value(i);

      if (c) {
        c -> attach(this);
      }
    }
  }

  if (m_Data) {
    setTitle(m_Data->get_Title());
  } else {
    setTitle("");
  }

  if (m_Data && m_FirstTime) {
    autoScale();

    m_FirstTime = false;
  }

  replot();
}

void QxrdImagePlot::onMaskedImageAvailable(QxrdMaskDataPtr mask)
{
  m_Mask = mask;

  replotImage();
}

void QxrdImagePlot::onDarkImageAvailable(QxrdDoubleImageDataPtr /*image*/)
{
}

void QxrdImagePlot::onCenterXChanged(double cx)
{
  m_CenterX = cx;

  replotImage();
}

void QxrdImagePlot::onCenterYChanged(double cy)
{
  m_CenterY = cy;

  replotImage();
}

void QxrdImagePlot::onCenterChanged(QPointF c)
{
  m_CenterX = c.x();
  m_CenterY = c.y();

  replotImage();
}

void QxrdImagePlot::onImageScaleChanged()
{
  if (m_Rescaler && m_Rescaler->isEnabled()) {
    m_Rescaler->rescale();
  }
}

void QxrdImagePlot::disablePickers()
{
  m_Zoomer             -> setEnabled(false);
  m_CenterFinderPicker -> setEnabled(false);
  m_Slicer             -> setEnabled(false);
  m_Measurer           -> setEnabled(false);
  m_HistogramSelector  -> setEnabled(false);
  m_Circles            -> setEnabled(false);
  m_Polygons           -> setEnabled(false);
  m_PowderPointPicker  -> setEnabled(false);

  enableContextMenu();
}

void QxrdImagePlot::enableZooming()
{
  disablePickers();

  m_Zoomer       -> setEnabled(true);
}

void QxrdImagePlot::enableCentering()
{
  disablePickers();

  m_CenterFinderPicker -> setEnabled(true);
}

void QxrdImagePlot::enableSlicing()
{
  disablePickers();

  m_Slicer   -> setEnabled(true);
}

void QxrdImagePlot::enableMeasuring()
{
  disablePickers();

  m_Measurer -> setEnabled(true);
}

void QxrdImagePlot::enableHistograms()
{
  disablePickers();

  m_HistogramSelector -> setEnabled(true);
}

void QxrdImagePlot::enableMaskCircles()
{
  disablePickers();

  m_Circles  -> setEnabled(true);
}

void QxrdImagePlot::enableMaskPolygons()
{
  disablePickers();

  m_Polygons -> setEnabled(true);
}

void QxrdImagePlot::enablePowderPoints()
{
  disablePickers();

  m_PowderPointPicker -> setEnabled(true);

//  displayPowderMarkers();
}

//#undef replot

void QxrdImagePlot::replot()
{
//    QTime tic;
//    tic.start();

  QxrdPlot::replot();

//    g_Application->printMessage(tr("QxrdImagePlot::replot took %1 msec").arg(tic.restart()));
}

QwtText QxrdImagePlot::trackerTextF(const QPointF &pos)
{
  QxrdDoubleImageDataPtr image    = m_Data;
  QxrdMaskDataPtr        mask     = m_Mask;

  QxrdDataProcessorPtr processor = this->processor();
  QxrdCenterFinderPtr centerFinder;

  if (processor) {
    centerFinder = processor->centerFinder();
  }

  QString res = tr("%1, %2").arg(pos.x()).arg(pos.y());

  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    set->set_XMouse(pos.x());
    set->set_YMouse(pos.y());
  }

  if (image) {
    double val = image->value(pos.x(),pos.y());
    res += tr(", %1").arg(val);

    if (set) {
      set->set_ValMouse(val);
    }
  }

  if (mask) {
    double m = mask->value(pos.x(),pos.y());
    res += tr(", %1").arg(m);

    if (set) {
      set->set_MaskMouse(m);
    }
  }

  if (centerFinder) {
    double tth = centerFinder->getTTH(pos);
    res += tr("\nTTH %1").arg(tth);

    if (set) {
      set->set_TTHMouse(tth);
    }

    double q = centerFinder->getQ(pos);
    res += tr(", Q %1").arg(q);

    if (set) {
      set->set_QMouse(q);
    }

    double r = centerFinder->getR(pos);
    res += tr(", R %1").arg(r);

    if (set) {
      set->set_RMouse(r);
    }

    double chi = centerFinder->getChi(pos);
    res += tr(", Chi %1").arg(chi);

//    if (m_PowderPointPicker -> isEnabled()) {
//      QPointF rpt = ras->optimizePeakPosition(pos);
//      res += tr("\nPtx %1, Pty %2").arg(rpt.x()).arg(rpt.y());
//    }
  }

  return res;
}

void QxrdImagePlot::enableContextMenu()
{
  m_ContextMenuEnabled = true;
}

void QxrdImagePlot::disableContextMenu()
{
  m_ContextMenuEnabled = false;
}

void QxrdImagePlot::contextMenuEvent(QContextMenuEvent * event)
{
  if (m_ContextMenuEnabled) {
    QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

    if (set) {
      QMenu plotMenu(NULL, NULL);

      QAction *auSc = plotMenu.addAction("Autoscale");
      QAction *prGr = plotMenu.addAction("Print Graph...");

      plotMenu.addSeparator();

      QxrdDataProcessorPtr dp(m_DataProcessor);

      if (dp) {
        QxrdCenterFinderPtr cf(dp->centerFinder());

        if (cf) {
          QwtScaleMap xMap = canvasMap(QwtPlot::xBottom);
          QwtScaleMap yMap = canvasMap(QwtPlot::yLeft);

          QWidget *canv = canvas();

          QPoint evlocal = canv->mapFromParent(event->pos());

          double x = xMap.invTransform(evlocal.x());
          double y = yMap.invTransform(evlocal.y());

          QxrdPowderPoint nearest = cf->nearestPowderPoint(x, y);

          QAction *fitCircle        = plotMenu.addAction(tr("Fit Circle Center from Points on Ring %1").arg(nearest.n1()));
          QAction *fitEllipse       = plotMenu.addAction(tr("Fit Ellipse from Points on Ring %1").arg(nearest.n1()));
          QAction *fitEllipses      = plotMenu.addAction(tr("Fit Ellipses to all powder rings"));
          QAction *delPoint         = plotMenu.addAction(tr("Delete point at (%1,%2)").arg(nearest.x()).arg(nearest.y()));
          QAction *delRing          = plotMenu.addAction(tr("Delete Ring %1").arg(nearest.n1()));
          QAction *deleteAllPoints  = plotMenu.addAction(tr("Delete all Rings"));
          QAction *disableRing      = plotMenu.addAction(tr("Disable Ring %1").arg(nearest.n1()));
          QAction *enableRing       = plotMenu.addAction(tr("Enable Ring %1").arg(nearest.n1()));
          QAction *normalizeRings   = plotMenu.addAction(tr("Normalize Powder Rings"));
          QAction *fitPeakNear      = plotMenu.addAction(tr("Fit Diffracted Peak near (%1,%2) [%3,%4]").arg(x).arg(y).arg(event->x()).arg(event->y()));
          QAction *fitRingNear      = plotMenu.addAction(tr("Fit Point on Diffracted Ring near (%1,%2) [%3,%4]").arg(x).arg(y).arg(event->x()).arg(event->y()));
          QAction *traceRingClockwise = plotMenu.addAction(tr("Trace Diffracted Ring starting at (%1,%2) [%3,%4]").arg(x).arg(y).arg(event->x()).arg(event->y()));
          QAction *missingRing      = plotMenu.addAction(tr("Missing Diffracted Ring near (%1,%2)").arg(x).arg(y));
          //          QAction *traceRingParallel = plotMenu.addAction(tr("Trace Diffracted Ring starting at (%1,%2) [%3,%4] in parallel").arg(x).arg(y).arg(event->x()).arg(event->y()));
          QAction *zapPixel         = plotMenu.addAction(tr("Zap (replace with avg of neighboring values) pixel [%1,%2]").arg((int)x).arg(int(y)));

          QAction *action = plotMenu.exec(event->globalPos());

          if (action == auSc) {
            autoScale();
          } else if (action == prGr) {
            printGraph();
          } else if (action == fitCircle) {
            cf->fitPowderCircle(nearest.n1());
          } else if (action == fitEllipse) {
            cf->fitPowderEllipse(nearest.n1());
          } else if (action == fitEllipses) {
            cf->fitPowderEllipses();
          } else if (action == delPoint) {
            cf->deletePowderPointNear(x,y);
          } else if (action == delRing) {
            cf->deletePowderRing(nearest.n1());
          } else if (action == deleteAllPoints) {
            cf->deletePowderPoints();
          } else if (action == enableRing) {
            cf->enablePowderRing(nearest.n1());
          } else if (action == disableRing) {
            cf->disablePowderRing(nearest.n1());
          } else if (action == normalizeRings) {
            cf->normalizePowderRings();
          } else if (action == fitPeakNear) {
            QMetaObject::invokeMethod(cf.data(), "fitPeakNear",
                                      Q_ARG(double,x),
                                      Q_ARG(double,y));
          } else if (action == fitRingNear) {
            QMetaObject::invokeMethod(cf.data(), "fitRingNear",
                                      Q_ARG(double,x),
                                      Q_ARG(double,y));
          } else if (action == traceRingClockwise) {
            QMetaObject::invokeMethod(cf.data(), "traceRingNear",
                                      Q_ARG(double,x),
                                      Q_ARG(double,y),
                                      Q_ARG(double,25.0));
          } else if (action == missingRing) {
            cf->missingRingNear(x,y);
//            QMetaObject::invokeMethod((cf.data(), "missingRingNear",
//                                       Q_ARG(double,x),
//                                       Q_ARG(double,y));
//          } else if (action == traceRingParallel) {
//            QMetaObject::invokeMethod(cf.data(), "traceRingNearParallel",
//                                      Q_ARG(double,x),
//                                      Q_ARG(double,y),
//                                      Q_ARG(double,25.0));
          } else if (action == zapPixel) {
            this->zapPixel(qRound(x),qRound(y));
          }
        }
      }
    }

    event->accept();
  } else {
    event->accept();
  }
}

void QxrdImagePlot::zapPixel(int x, int y)
{
  double sum = 0;
  int    npx = 0;

  for (int ix = x-1; ix <= x+1; ix++) {
    for (int iy = y-1; iy <= y+1; iy++) {
      sum += m_Data->value(ix, iy);
      npx += 1;
    }
  }

  sum -= m_Data->value(x,y);
  npx -= 1;

  m_Data->setValue(x,y, sum/npx);

  replot();
}

void QxrdImagePlot::onMarkedPointsChanged()
{
  replotImage();
}

void QxrdImagePlot::displayPowderMarkers(QxrdPlotCurveVectorPtr &curves)
{
  QxrdDataProcessorPtr dp(m_DataProcessor);

  if (dp) {
    QxrdCenterFinderPtr cf(dp->centerFinder());

    if (cf) {
      int nrgs = cf->countPowderRings();
      int npts = cf->countPowderRingPoints();

      for (int r=0; r<nrgs; r++) {
        QVector<double> x,y;

        for (int i=0; i<npts; i++) {
          QxrdPowderPoint pt = cf->powderRingPoint(i);

          if (pt.n1() == r) {
            x.append(pt.x());
            y.append(pt.y());
          }
        }

        if (x.count() > 0) {
          QwtPlotPiecewiseCurve *pc = new QwtPlotPiecewiseCurve(this, tr("Ring %1").arg(r));

          setPlotCurveStyle(r, pc);
          pc -> setSamples(x, y);
          pc -> setStyle(QwtPlotCurve::NoCurve);
          pc -> setLegendAttribute(QwtPlotCurve::LegendShowSymbol, true);

          curves -> append(pc);
        }
      }
    }
  }
}
