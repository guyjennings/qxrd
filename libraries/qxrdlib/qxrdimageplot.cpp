#include "qxrddebug.h"
#include "qxrdimageplot.h"
#include "qxrdrasterdata.h"
#include "qxrdplotslicer.h"
#include "qxrdimageplotmeasurer.h"
#include "qxrdimageplotzoomer.h"
#include "qxrdcenterfinder.h"
#include "qxrdcenterfinderpicker.h"
#include "qxrddataprocessor.h"
#include "qxrdmaskpicker.h"
#include "qxrdhistogramselector.h"
#include "qxrdapplication.h"
#include "qcepallocator.h"
#include "qxrddataprocessor.h"
#include "qxrdacquisition.h"
#include <qwt_plot_rescaler.h>
#include <qwt_plot_marker.h>
#include <qwt_legend.h>
#include <qwt_legend_label.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_scale_widget.h>
#include <qwt_symbol.h>
#include <qwt_picker_machine.h>
#include <QTime>
#include <QMenu>
#include <QContextMenuEvent>
#include "qxrdroicoordinateslistmodel.h"
#include "qxrdroicoordinates.h"
#include "qxrdroieditordialog.h"

#include "qwt_plot_piecewise_curve.h"

QxrdImagePlot::QxrdImagePlot(QWidget *parent)
  : QcepPlot(parent),
    m_ObjectNamer(this, "imageGraph"),
    m_ImagePlotSettings(),
    m_Rescaler(NULL),
    m_Slicer(NULL),
    m_Measurer(NULL),
    m_HistogramSelector(NULL),
    m_Data(NULL),
    m_Mask(NULL),
    m_Overflow(NULL),
    m_DataRaster(NULL),
    m_MaskRaster(NULL),
    m_OverflowRaster(NULL),
    m_DataImage(NULL),
    m_MaskImage(NULL),
    m_OverflowImage(NULL),
    m_ColorMap(new QwtLinearColorMap(Qt::black, Qt::white)),
    m_MaskColorMap(new QxrdMaskColorMap(Qt::red, QColor(0,0,0,0))),
    m_MaskAlpha(80),
    m_OverflowColorMap(new QxrdMaskColorMap(QColor(0,0,0,0), Qt::green)),
    m_OverflowAlpha(256),
    m_DataProcessor(),
    m_CenterFinderPicker(NULL),
    m_CenterMarker(NULL),
    m_Circles(NULL),
    m_Polygons(NULL),
    m_PowderPointPicker(NULL),
    m_FirstTime(true),
    m_ContextMenuEnabled(true),

    m_ROIModel(),
    m_ROISelection(NULL),
    m_ROICurves()
{
}

void QxrdImagePlot::init(QxrdImagePlotSettingsWPtr settings, QcepObjectWPtr parent)
{
  QcepPlot::init(settings);

  m_Parent = parent;

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

//  insertLegend(m_Legend, QwtPlot::BottomLegend);

  m_DataImage = new QwtPlotSpectrogram();
  m_DataImage -> attach(this);
  m_DataImage -> setRenderThreadCount(0);

  m_MaskImage = new QwtPlotSpectrogram();
  m_MaskImage -> setAlpha(set && set->get_MaskShown() ? m_MaskAlpha : 0);
  m_MaskImage -> attach(this);
  m_MaskImage -> setRenderThreadCount(0);

  m_OverflowImage = new QwtPlotSpectrogram();
  m_OverflowImage -> setAlpha(set && set->get_OverflowShown() ? m_OverflowAlpha : 0);
  m_OverflowImage -> attach(this);
  m_OverflowImage -> setRenderThreadCount(0);

  m_CenterFinderPicker = new QxrdCenterFinderPicker(this);

  m_CenterMarker = new QwtPlotMarker();
  m_CenterMarker -> setLineStyle(QwtPlotMarker::Cross);
  m_CenterMarker -> attach(this);

  m_Circles = new QxrdCircularMaskPicker(canvas(), this);
  m_Circles -> setEnabled(false);

  m_Polygons = new QxrdPolygonalMaskPicker(canvas(), this);
  m_Polygons -> setEnabled(false);

  m_PowderPointPicker = new QxrdPowderPointPicker(this);
  m_PowderPointPicker -> setEnabled(false);

  set100Range();
  setGrayscale();

  if (set) {
    connect(m_Zoomer, &QwtPlotZoomer::zoomed, this, &QxrdImagePlot::onImageScaleChanged);
    connect(set->prop_ImageShown(), &QcepBoolProperty::valueChanged, this, &QxrdImagePlot::changeImageShown);
    connect(set->prop_MaskShown(), &QcepBoolProperty::valueChanged, this, &QxrdImagePlot::changeMaskShown);
    connect(set->prop_OverflowShown(), &QcepBoolProperty::valueChanged, this, &QxrdImagePlot::changeOverflowShown);
    connect(set->prop_DisplayMinimumPct(), &QcepDoubleProperty::valueChanged, this, &QxrdImagePlot::recalculateDisplayedRange);
    connect(set->prop_DisplayMaximumPct(), &QcepDoubleProperty::valueChanged, this, &QxrdImagePlot::recalculateDisplayedRange);
    connect(set->prop_DisplayMinimumVal(), &QcepDoubleProperty::valueChanged, this, &QxrdImagePlot::recalculateDisplayedRange);
    connect(set->prop_DisplayMaximumVal(), &QcepDoubleProperty::valueChanged, this, &QxrdImagePlot::recalculateDisplayedRange);
    connect(set->prop_DisplayMinimumPctle(), &QcepDoubleProperty::valueChanged, this, &QxrdImagePlot::recalculateDisplayedRange);
    connect(set->prop_DisplayMaximumPctle(), &QcepDoubleProperty::valueChanged, this, &QxrdImagePlot::recalculateDisplayedRange);
    connect(set->prop_DisplayScalingMode(), &QcepIntProperty::valueChanged, this, &QxrdImagePlot::recalculateDisplayedRange);
    connect(set->prop_InterpolatePixels(), &QcepBoolProperty::valueChanged, this, &QxrdImagePlot::onInterpolateChanged);
    connect(set->prop_MaintainAspectRatio(), &QcepBoolProperty::valueChanged, this, &QxrdImagePlot::onMaintainAspectChanged);
    connect(set->prop_DisplayColorMap(), &QcepIntProperty::valueChanged, this, &QxrdImagePlot::setColorMap);
    connect(set->prop_DisplayLog(), &QcepBoolProperty::valueChanged, this, &QxrdImagePlot::redoColorMap);

    changeImageShown(set->get_ImageShown());
    changeMaskShown(set->get_MaskShown());
    changeOverflowShown(set->get_OverflowShown());
    recalculateDisplayedRange();
    onInterpolateChanged(set->get_InterpolatePixels());
    onMaintainAspectChanged(set->get_MaintainAspectRatio());
    setColorMap(set->get_DisplayColorMap());
    redoColorMap();
  }

  enableZooming();

  onImageScaleChanged();
}

void QxrdImagePlot::printMessage(QString msg, QDateTime dt) const
{
  QcepObjectPtr parent(m_Parent);

  if (parent) {
    parent->printMessage(msg, dt);
  }
}

void QxrdImagePlot::setProcessor(QxrdDataProcessorWPtr proc)
{
  m_DataProcessor = proc;

  QxrdDataProcessorPtr dp(m_DataProcessor);

  if (dp) {
    QxrdCenterFinderPtr cf(dp->centerFinder());

    if (cf) {
      connect(m_CenterFinderPicker, (void (QcepPlotMeasurer::*)( const QPointF &)) &QwtPlotPicker::selected,
              cf.data(), &QxrdCenterFinder::onCenterChanged);

      connect(m_Circles, (void (QcepPlotMeasurer::*)( const QRectF &)) &QwtPlotPicker::selected,
              dp.data(), &QxrdDataProcessor::maskCircle);

      connect(m_Polygons, (void (QcepPlotMeasurer::*)( const QVector<QPointF> &)) &QwtPlotPicker::selected,
              dp.data(), &QxrdDataProcessor::maskPolygon);

      connect(m_Measurer, (void (QcepPlotMeasurer::*)( const QVector<QPointF> &)) &QwtPlotPicker::selected,
              dp.data(), &QxrdDataProcessor::measurePolygon);

      connect(m_PowderPointPicker, (void (QcepPlotMeasurer::*)( const QPointF &)) &QwtPlotPicker::selected,
              cf.data(), &QxrdCenterFinder::onPointSelected);

      onCenterChanged(QPointF(cf->get_CenterX(), cf->get_CenterY()));

      connect(cf->prop_MarkedPoints(), &QxrdPowderPointVectorProperty::valueChanged,
              this, &QxrdImagePlot::onMarkedPointsChanged);

      onMarkedPointsChanged();
    }
  }

  connect(m_Slicer, (void (QcepPlotMeasurer::*)( const QVector<QPointF> &)) &QwtPlotPicker::selected,
          this, &QxrdImagePlot::slicePolygon);

  connect(m_HistogramSelector, (void (QcepPlotMeasurer::*)( const QRectF &)) &QwtPlotPicker::selected,
          this, &QxrdImagePlot::selectHistogram);
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
  QcepPlot::autoScale();

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
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set && m_DataRaster) {
    double mindis, maxdis;

    if (set->get_DisplayScalingMode() == PercentageMode) {
      double minv = m_DataRaster->minValue();
      double maxv = m_DataRaster->maxValue();
      double del = maxv-minv;

      mindis = minv+del*set->get_DisplayMinimumPct()/100.0;
      maxdis = minv+del*set->get_DisplayMaximumPct()/100.0;
    } else if (set->get_DisplayScalingMode() == PercentileMode) {
      QwtInterval range = m_DataRaster->percentileRange(set->get_DisplayMinimumPctle(), set->get_DisplayMaximumPctle());

      mindis = range.minValue();
      maxdis = range.maxValue();
    } else {
      mindis = set->get_DisplayMinimumVal();
      maxdis = set->get_DisplayMaximumVal();
    }

    m_DataRaster->setDisplayedRange(mindis, maxdis);

    replotImage();
  }
}

void QxrdImagePlot::replotImage()
{
  m_DataImage -> setData(m_DataRaster);

//  m_DataImage -> invalidateCache();
//  m_DataImage -> itemChanged();

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

  if (m_DataRaster) {
    m_DataRaster->setInterpolate(interp);

  }

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

  if (m_CenterMarker) {
    m_CenterMarker -> setLinePen(pen);
  }

  m_MaskColorMap->setColorInterval(pen.color(), QColor(0,0,0,0));

//  foreach (QwtPlotMarker *m, m_PowderPointMarkers) {
//    const QwtSymbol *oldsym = m->symbol();

//    QwtSymbol *sym = new QwtSymbol(oldsym->style(),oldsym->brush(),oldsym->pen(),oldsym->size());

//    sym->setPen(pen);
//    sym->setBrush(QBrush(pen.color()));

//    m->setSymbol(sym);
//  }
}

void QxrdImagePlot::colorMapStart(QColor startColor, QColor endColor)
{
  m_ColorMap->setColorInterval(startColor, endColor);
}

void QxrdImagePlot::colorMapRange(double value1, QColor color1, double value2, QColor color2)
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

      m_ColorMap->addColorStop(val, col);
    }
  } else {
    m_ColorMap->addColorStop(value1, color1);
  }
}

void QxrdImagePlot::setGrayscale()
{
  colorMapStart(Qt::black, Qt::white);
  colorMapRange(0.0, Qt::black, 1.0, Qt::white);

  setTrackerPen(QPen(Qt::red));

  changedColorMap();
}

void QxrdImagePlot::setInverseGrayscale()
{
  colorMapStart(Qt::white, Qt::black);
  colorMapRange(0.0, Qt::white, 1.0, Qt::black);

  setTrackerPen(QPen(Qt::red));

  changedColorMap();
}

void QxrdImagePlot::setEarthTones()
{
  colorMapStart(Qt::black, Qt::white);

  colorMapRange(0.0, Qt::black, 0.15, Qt::blue);
  colorMapRange(0.15, Qt::blue, 0.25, Qt::gray);
  colorMapRange(0.25, Qt::gray, 0.35, Qt::green);
  colorMapRange(0.35, Qt::green, 0.5, Qt::darkYellow);
  colorMapRange(0.5, Qt::darkYellow, 0.85, Qt::darkMagenta);
  colorMapRange(0.85, Qt::darkMagenta, 1.0, Qt::white);

  setTrackerPen(QPen(Qt::red));

  changedColorMap();
}

void QxrdImagePlot::setSpectrum()
{
  colorMapStart(Qt::magenta, Qt::red);

  colorMapRange(0.0, Qt::magenta,0.2, Qt::blue);
  colorMapRange(0.2, Qt::blue,   0.4, Qt::cyan);
  colorMapRange(0.4, Qt::cyan,   0.6, Qt::green);
  colorMapRange(0.6, Qt::green,  0.8, Qt::yellow);
  colorMapRange(0.8, Qt::yellow, 1.0, Qt::red);

  setTrackerPen(QPen(Qt::black));

  changedColorMap();
}

void QxrdImagePlot::setFire()
{
  colorMapStart(Qt::black, Qt::white);

  colorMapRange(0.0,  Qt::black,  0.25, Qt::red);
  colorMapRange(0.25, Qt::red,    0.75, Qt::yellow);
  colorMapRange(0.75, Qt::yellow, 1.0,  Qt::white);

  setTrackerPen(QPen(Qt::blue));

  changedColorMap();
}

void QxrdImagePlot::setIce()
{
  colorMapStart(Qt::black, Qt::white);

  colorMapRange(0.0,  Qt::black, 0.25, Qt::blue);
  colorMapRange(0.25, Qt::blue,  0.75, Qt::cyan);
  colorMapRange(0.75, Qt::cyan,  1.0,  Qt::white);

  setTrackerPen(QPen(Qt::red));

  changedColorMap();
}

void QxrdImagePlot::redoColorMap()
{
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    setColorMap(set->get_DisplayColorMap());
  }
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

    if (m_DataImage) {
      m_DataImage -> setAlpha(set->get_ImageShown() ? 255 : 0);
//      m_DataImage -> invalidateCache();
//      m_DataImage -> itemChanged();

      replotImage();
    }
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

    if (m_MaskImage) {
      m_MaskImage -> setAlpha(set->get_MaskShown() ? m_MaskAlpha : 0);
//      m_MaskImage -> invalidateCache();
//      m_MaskImage -> itemChanged();

      replotImage();
    }
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

    if (m_OverflowImage) {
      m_OverflowImage -> setAlpha(set->get_OverflowShown() ? m_OverflowAlpha : 0);
//      m_OverflowImage -> invalidateCache();
//      m_OverflowImage -> itemChanged();

      replotImage();
    }
  }
}

void QxrdImagePlot::toggleShowROI()
{
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    changeROIShown(!set->get_DisplayROI());
  }
}

void QxrdImagePlot::changeROIShown(bool shown)
{
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    set->set_DisplayROI(shown);

    updateROIDisplay();
  }
}

void QxrdImagePlot::toggleLogDisplay()
{
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    changeLogDisplay(!set->get_DisplayLog());
  }
}

void QxrdImagePlot::changeLogDisplay(bool isLog)
{
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    set->set_DisplayLog(isLog);
  }
}

void QxrdImagePlot::changedColorMap()
{
  m_DataImage -> setColorMap(m_ColorMap);
//  m_DataImage -> invalidateCache();
//  m_DataImage -> itemChanged();

  m_MaskImage   -> setColorMap(m_MaskColorMap);
//  m_MaskImage   -> invalidateCache();
//  m_MaskImage   -> itemChanged();

  m_OverflowImage   -> setColorMap(m_OverflowColorMap);
//  m_OverflowImage   -> invalidateCache();
//  m_OverflowImage   -> itemChanged();

  replotImage();
}

void QxrdImagePlot::setImage(QxrdRasterData *data)
{
  QTime t;
  t.start();

  m_DataRaster = data;

  m_DataImage -> setData(data);

  if (g_Application && qcepDebug(DEBUG_DISPLAY)) {
    g_Application->printMessage(tr("QxrdImagePlot::setImage setData after %1 msec").arg(t.elapsed()));
  }

//  m_DataImage -> invalidateCache();
//  m_DataImage -> itemChanged();

  recalculateDisplayedRange();

  if (g_Application && qcepDebug(DEBUG_DISPLAY)) {
    g_Application->printMessage(tr("QxrdImagePlot::setImage recalculate after %1 msec").arg(t.elapsed()));
  }

  onImageScaleChanged();

  if (g_Application && qcepDebug(DEBUG_DISPLAY)) {
    g_Application->printMessage(tr("QxrdImagePlot::setImage scaleChanged after %1 msec").arg(t.elapsed()));
  }
}

void QxrdImagePlot::setMask(QxrdMaskRasterData *mask)
{
  m_MaskRaster = mask;

  m_MaskImage -> setData(mask);
//  m_MaskImage -> invalidateCache();
//  m_MaskImage -> itemChanged();

  replot();
}

void QxrdImagePlot::setOverflows(QxrdMaskRasterData *overflow)
{
  m_OverflowRaster = overflow;

  m_OverflowImage -> setData(overflow);
//  m_OverflowImage -> invalidateCache();
//  m_OverflowImage -> itemChanged();

  replot();
}

void QxrdImagePlot::setAutoOverflow()
{
  QTime t;
  t.start();

  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (m_Data && proc) {
    int w = m_Data->get_Width();
    int h = m_Data->get_Height();

    QxrdAcquisitionPtr acq = proc->acquisition();

    if (acq) {
      double ovfLevel = acq->get_OverflowLevel();

      m_Overflow = QcepAllocator::newMask("mask", w, h, 0, QcepAllocator::AlwaysAllocate);

      m_Data->markOverflows(m_Overflow, ovfLevel);
    }
  }


  if (g_Application && qcepDebug(DEBUG_DISPLAY)) {
    g_Application->printMessage(tr("QxrdImageplot::setAutoOverflow took %1 msec").arg(t.elapsed()));
  }
}

void QxrdImagePlot::onProcessedImageAvailable(QcepImageDataBasePtr image, QcepMaskDataPtr overflow)
{
  QTime tic;
  tic.start();

  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    m_Data = image;
    m_Overflow = overflow;

    if (!image ||
        m_DataRaster == NULL ||
        image->get_Width() != m_DataRaster->width() ||
        image->get_Height() != m_DataRaster->height()) {
      m_FirstTime = true;
    }

    QxrdRasterData *data = new QxrdRasterData(image, set->get_InterpolatePixels());

    if (g_Application && qcepDebug(DEBUG_DISPLAY)) {
      g_Application->printMessage(tr("QxrdImagePlot::onProcessedImageAvailable new raster after %1 msec").arg(tic.elapsed()));
    }

    if (overflow == NULL) {
      setAutoOverflow();
      setImage(data);
    } else {
      setImage(data);
    }


    if (g_Application && qcepDebug(DEBUG_DISPLAY)) {
      g_Application->printMessage(tr("QxrdImagePlot::onProcessedImageAvailable set image after %1 msec").arg(tic.elapsed()));
    }

    setOverflows(new QxrdMaskRasterData(m_Overflow));

    if (g_Application && qcepDebug(DEBUG_DISPLAY)) {
      g_Application->printMessage(tr("QxrdImagePlot::onProcessedImageAvailable set overflows after %1 msec").arg(tic.elapsed()));
    }

    if (image) {
      setTitle(image -> get_Name());
    } else {
      setTitle("");
    }

    replotImage();

    if (g_Application && qcepDebug(DEBUG_DISPLAY)) {
      g_Application->printMessage(tr("QxrdImagePlot::onProcessedImageAvailable replot image after %1 msec").arg(tic.elapsed()));
    }
  }

  if (g_Application && qcepDebug(DEBUG_DISPLAY)) {
    g_Application->printMessage(tr("QxrdImagePlot::onProcessedImageAvailable took %1 msec").arg(tic.elapsed()));
  }
}

void QxrdImagePlot::onMaskedImageAvailable(QcepImageDataBasePtr image, QcepMaskDataPtr mask)
{
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    m_Data = image;
    m_Mask = mask;

    if (!image ||
        image->get_Width() != m_DataRaster->width() ||
        image->get_Height() != m_DataRaster->height()) {
      m_FirstTime = true;
    }

    QxrdRasterData *data = new QxrdRasterData(image, set->get_InterpolatePixels());
    QxrdMaskRasterData *msk = new QxrdMaskRasterData(mask, false);

    setImage(data);
    setMask(msk);

    if (image) {
      setTitle(image -> get_Name());
    } else {
      setTitle("");
    }

    replotImage();
  }
}

void QxrdImagePlot::onDarkImageAvailable(QcepImageDataBasePtr /*image*/)
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

void QxrdImagePlot::onCenterChanged(QPointF c)
{
  m_CenterMarker -> setValue(c);
  replot();
}

void QxrdImagePlot::onImageScaleChanged()
{
  if (m_Rescaler && m_Rescaler->isEnabled()) {
    m_Rescaler->rescale();
  }
}

const QxrdRasterData* QxrdImagePlot::raster() const
{
  return m_DataRaster;
}

QxrdRasterData* QxrdImagePlot::raster()
{
  return m_DataRaster;
}

const QxrdMaskRasterData* QxrdImagePlot::maskRaster() const
{
  return m_MaskRaster;
}

QxrdMaskRasterData* QxrdImagePlot::maskRaster()
{
  return m_MaskRaster;
}

void QxrdImagePlot::disablePickers()
{
  if (m_Zoomer) {
    m_Zoomer             -> setEnabled(false);
  }

  if (m_CenterFinderPicker) {
    m_CenterFinderPicker -> setEnabled(false);
  }

  if (m_Slicer) {
    m_Slicer             -> setEnabled(false);
  }

  if (m_Measurer) {
    m_Measurer           -> setEnabled(false);
  }

  if (m_HistogramSelector) {
    m_HistogramSelector  -> setEnabled(false);
  }

  if (m_Circles) {
    m_Circles            -> setEnabled(false);
  }

  if (m_Polygons) {
    m_Polygons           -> setEnabled(false);
  }

  if (m_PowderPointPicker) {
    m_PowderPointPicker  -> setEnabled(false);
  }

  enableContextMenu();

  displayPowderMarkers();
}

void QxrdImagePlot::enableZooming()
{
  disablePickers();

  if (m_Zoomer) {
    m_Zoomer       -> setEnabled(true);
  }
}

void QxrdImagePlot::enableCentering()
{
  disablePickers();

  if (m_CenterFinderPicker) {
    m_CenterFinderPicker -> setEnabled(true);
  }
}

void QxrdImagePlot::enableSlicing()
{
  disablePickers();

  if (m_Slicer) {
    m_Slicer   -> setEnabled(true);
  }
}

void QxrdImagePlot::enableMeasuring()
{
  disablePickers();

  if (m_Measurer) {
    m_Measurer -> setEnabled(true);
  }
}

void QxrdImagePlot::enableHistograms()
{
  disablePickers();

  if (m_HistogramSelector) {
    m_HistogramSelector -> setEnabled(true);
  }
}

void QxrdImagePlot::enableMaskCircles()
{
  disablePickers();

  if (m_Circles) {
    m_Circles  -> setEnabled(true);
  }
}

void QxrdImagePlot::enableMaskPolygons()
{
  disablePickers();

  if (m_Polygons) {
    m_Polygons -> setEnabled(true);
  }
}

void QxrdImagePlot::enablePowderPoints()
{
  disablePickers();

  if (m_PowderPointPicker) {
    m_PowderPointPicker -> setEnabled(true);
  }

//  displayPowderMarkers();
}

//#undef replot

void QxrdImagePlot::replot()
{
//    QTime tic;
//    tic.start();

  QcepPlot::replot();

//    g_Application->printMessage(tr("QxrdImagePlot::replot took %1 msec").arg(tic.restart()));
}

QwtText QxrdImagePlot::trackerTextF(const QPointF &pos)
{
  const QxrdRasterData *ras = this->raster();

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

  if (ras) {
    double val = ras->value(pos.x(),pos.y());
    res += tr(", %1").arg(val);

    if (set) {
      set->set_ValMouse(val);
    }
  }

  if (m_MaskRaster && m_MaskRaster->data()) {
    double mask = m_MaskRaster->value(pos.x(),pos.y());
    res += tr(", %1").arg(mask);

    if (set) {
      set->set_MaskMouse(mask);
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

    if (m_PowderPointPicker -> isEnabled()) {
      QPointF rpt = ras->optimizePeakPosition(pos);
      res += tr("\nPtx %1, Pty %2").arg(rpt.x()).arg(rpt.y());
    }
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

          QAction *adjustEnergy     = plotMenu.addAction(tr("Adjust Energy to match Calibrant Ring %1").arg(nearest.n1()));
          QAction *adjustDistance   = plotMenu.addAction(tr("Adjust Detector Distance to match Calibrant Ring %1").arg(nearest.n1()));

          QAction *adjustFit        = plotMenu.addAction(tr("Fit to nearby peak when adding powder points?"));
          adjustFit->setCheckable(true); adjustFit->setChecked(cf->get_FitPowderPointPosition());
          QAction *addPoint         = plotMenu.addAction(tr("Add point at (%1,%2)").arg(x).arg(y));
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
          } else if (action == adjustEnergy) {
            cf->adjustEnergy(nearest.n1());
          } else if (action == adjustDistance) {
            cf->adjustDistance(nearest.n1());
          } else if (action == adjustFit) {
            cf->toggle_FitPowderPointPosition();
          } else if (action == addPoint) {
            cf->appendPowderPoint(x,y);
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
      sum += m_Data->getImageData(ix, iy);
      npx += 1;
    }
  }

  sum -= m_Data->getImageData(x,y);
  npx -= 1;

  m_Data->setImageData(x,y, sum/npx);

  replot();
}

void QxrdImagePlot::onMarkedPointsChanged()
{
  displayPowderMarkers();

  replot();
}

void QxrdImagePlot::displayPowderMarkers()
{
  clearPowderMarkers();

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
          QwtPlotCurve *pc = new QwtPlotCurve(tr("Ring %1").arg(r));

          setPlotCurveStyle(r, pc);
          pc -> setSamples(x, y);
          pc -> setStyle(QwtPlotCurve::NoCurve);
          pc -> setLegendAttribute(QwtPlotCurve::LegendShowSymbol, true);
          pc -> attach(this);

          m_PowderPointCurves.append(pc);
        }
      }

      replot();
    }
  }
}

void QxrdImagePlot::clearPowderMarkers()
{
  foreach(QwtPlotCurve *curve, m_PowderPointCurves) {
    curve->detach();
    delete curve;
  }

  m_PowderPointCurves.clear();
}

void QxrdImagePlot::enableROIDisplay(bool enable)
{
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    set->set_DisplayROI(enable);
  }

  updateROIDisplay();
}

void QxrdImagePlot::setROIModel(QxrdROICoordinatesListModelWPtr model)
{
  m_ROIModel = model;

  updateROIDisplay();

  QxrdROICoordinatesListModelPtr roiModel(m_ROIModel);

  if (roiModel) {
    connect(roiModel.data(), &QAbstractItemModel::modelReset,    this, &QxrdImagePlot::updateROIDisplay);
    connect(roiModel.data(), &QAbstractItemModel::dataChanged,   this, &QxrdImagePlot::roiDataChanged);
    connect(roiModel.data(), &QAbstractItemModel::rowsInserted,  this, &QxrdImagePlot::roiRowsInserted);
    connect(roiModel.data(), &QAbstractItemModel::rowsMoved,     this, &QxrdImagePlot::roiRowsMoved);
    connect(roiModel.data(), &QAbstractItemModel::rowsRemoved,   this, &QxrdImagePlot::roiRowsRemoved);
  }
}

QxrdROICoordinatesListModelWPtr QxrdImagePlot::roiModel()
{
  return m_ROIModel;
}

void QxrdImagePlot::setROISelection(QItemSelectionModel *select)
{
  m_ROISelection = select;

  updateROIDisplay();

  if (m_ROISelection) {
    connect(m_ROISelection, &QItemSelectionModel::selectionChanged,
            this, &QxrdImagePlot::updateROISelection);
  }
}

QItemSelectionModel* QxrdImagePlot::roiSelection()
{
  return m_ROISelection;
}

void QxrdImagePlot::clearROIDisplay()
{
  foreach (QwtPlotCurve *curve, m_ROICurves) {
    curve->detach();
    delete curve;
  }

  m_ROICurves.clear();
}

void QxrdImagePlot::updateROIDisplay()
{
  clearROIDisplay();

  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set && set->get_DisplayROI() && m_ROIModel && m_ROISelection) {
    QxrdROICoordinatesListModelPtr roiModel(m_ROIModel);

    if (roiModel) {
      int nROI = roiModel->rowCount(QModelIndex());

      for (int i=0; i<nROI; i++) {
        QxrdROICoordinatesPtr roi = roiModel->roi(i);

        if (roi) {
          QVector<QPointF> pts = roi->markerCoords();

          QwtPlotPiecewiseCurve *pc = new QwtPlotPiecewiseCurve(this, tr("ROI %1").arg(i));

          setPlotCurveStyle(i, pc);

          bool on = m_ROISelection->rowIntersectsSelection(i, QModelIndex());

          if (pc) {
            QPen pen = pc->pen();
            const QwtSymbol *oldsym = pc->symbol();
            QwtSymbol *sym = NULL;

            if (oldsym) {
              sym = new QwtSymbol(oldsym->style(), oldsym->brush(), oldsym->pen(), oldsym->size());
            }

            if (on) {
              pen.setWidth(3);
              if (sym) {
                sym->setSize(9,9);
              }
            } else {
              pen.setWidth(1);
              if (sym) {
                sym->setSize(5,5);
              }
            }
            pc->setPen(pen);
            if (sym) {
              pc->setSymbol(sym);
            }
          }

          pc->setSamples(pts);

          pc->attach(this);

          m_ROICurves.append(pc);
        }
      }
    }
  }

  replot();
}

void QxrdImagePlot::roiDataChanged(const QModelIndex &topLeft,
                                   const QModelIndex &bottomRight,
                                   const QVector<int> &roles)
{
  int t = topLeft.row();
  int l = topLeft.column();
  int b = bottomRight.row();
  int r = bottomRight.column();

//  printMessage(tr("roiDataChanged t:%1 l:%2 b:%3 r:%4 n:%5")
//               .arg(t).arg(l).arg(b).arg(r).arg(roles.count()));

  if (r >= QxrdROICoordinatesListModel::OuterTypeCol) {
    updateROIDisplay();
  }
}

void QxrdImagePlot::roiRowsInserted(const QModelIndex &parent, int first, int last)
{
  updateROIDisplay();
}

void QxrdImagePlot::roiRowsMoved(const QModelIndex &parent, int start, int end,
                                 const QModelIndex &destination, int row)
{
  updateROIDisplay();
}

void QxrdImagePlot::roiRowsRemoved(const QModelIndex &parent, int first, int last)
{
  updateROIDisplay();
}

void QxrdImagePlot::onLegendChecked(const QVariant &itemInfo, bool on, int index)
{
  QwtPlotItem *item = infoToItem(itemInfo);

  if (item) {
    QwtPlotCurve *pc = dynamic_cast<QwtPlotCurve*>(item);

    int i = m_ROICurves.indexOf(pc);

    if (i >= 0) {
      QxrdROICoordinatesListModelPtr roiModel(m_ROIModel);

      if (m_ROISelection && roiModel) {
        if (on) {
          m_ROISelection->select(roiModel->index(i,0),
                                 QItemSelectionModel::Select | QItemSelectionModel::Rows);
        } else {
          m_ROISelection->select(roiModel->index(i,0),
                                 QItemSelectionModel::Deselect | QItemSelectionModel::Rows);
        }
      }
    }
  }

  QcepPlot::onLegendChecked(itemInfo, on, index);
}

void QxrdImagePlot::selectROIItem(int n, bool selected)
{
  QwtPlotCurve *pc = m_ROICurves.value(n);

  if (pc) {
    QPen pen = pc->pen();
    const QwtSymbol *oldsym = pc->symbol();
    QwtSymbol *sym = NULL;

    if (oldsym) {
     sym = new QwtSymbol(oldsym->style(), oldsym->brush(), oldsym->pen(), oldsym->size());
    }

    if (selected) {
      pen.setWidth(3);
      if (sym) {
        sym->setSize(9,9);
      }
    } else {
      pen.setWidth(1);
      if (sym) {
        sym->setSize(5,5);
      }
    }
    pc->setPen(pen);
    if (sym) {
      pc->setSymbol(sym);
    }
  }
}

void QxrdImagePlot::selectROILabel(int i, bool on)
{
  const QVariant itemInfo = itemToInfo(m_ROICurves.value(i));

  QwtLegendLabel *legendLabel =
      qobject_cast<QwtLegendLabel*>(
        m_Legend->legendWidget(itemInfo));

  if (legendLabel) {
    legendLabel->setChecked(on);
  }
}

void QxrdImagePlot::updateROISelection(
    const QItemSelection &selected,
    const QItemSelection &deselected)
{
  QxrdROICoordinatesListModelPtr roiModel(m_ROIModel);

  if (roiModel && m_ROISelection) {
    int n = roiModel->roiCount();

    for (int i=0; i<n; i++) {
      bool sel = m_ROISelection->rowIntersectsSelection(i, QModelIndex());

      selectROIItem(i, sel);
      selectROILabel(i, sel);
    }

    replot();
  }
}

void QxrdImagePlot::moveSelectedROICenter(double x, double y)
{
  QxrdROICoordinatesListModelPtr roiModel(m_ROIModel);

  if (roiModel && m_ROISelection) {
    int n = roiModel->roiCount();

    for (int i=0; i<n; i++) {
      if (m_ROISelection->rowIntersectsSelection(i,QModelIndex())) {
        roiModel->moveROICenter(i, x, y);
      }
    }
  }
}

void QxrdImagePlot::editSelectedROI(double x, double y)
{
  QxrdROICoordinatesListModelPtr roiModel(m_ROIModel);
  QPointF pt(x,y);

  if (roiModel) {
    int nearest = -1;
    double nearestDistance = qInf();

    int n = roiModel->roiCount();

    for (int i=0; i<n; i++) {
      QxrdROICoordinatesPtr roi = roiModel->roi(i);

      if (roi) {
        double dist = roi->nearestDistance(pt);

        if (dist < nearestDistance) {
          nearestDistance = dist;
          nearest = i;
        }
      }
    }

    if (nearest >= 0) {
      QxrdROICoordinatesPtr roi = roiModel->roi(nearest);

      QxrdROIEditorDialog editor(roi, this);

      editor.setWindowTitle(tr("Edit ROI %1").arg(nearest));

      if (editor.exec() == QDialog::Accepted) {
        roiModel->setRoi(nearest, editor.roi());
      }
    }
  }
}

//void QxrdImagePlot::roiMouseSelected(const QVector<QPointF> &p)
//{
//  QxrdROICoordinatesListModelPtr roiModel(m_ROIModel);

//  if (roiModel && p.count() == 2) {
//    QPointF del = p.value(1) - p.value(0);

//    if ((del.x() == 0) && (del.y() == 0)) {
//      int roiId, inOut, roiType, roiPtIndex;

//      if (roiModel->identifyROIPointByMouse(p.value(0),  scaledDelta(5,5), roiId, inOut, roiType, roiPtIndex)) {
//        printf("Select ROI %d:%d\n", roiId, inOut);
//      }
//    } else {
//      // Move click...

//      if (m_ROISelection) {
//        int n = roiModel->roiCount();

//        for (int i=0; i<n; i++) {
//          if (m_ROISelection->rowIntersectsSelection(i,QModelIndex())) {
//            roiModel->moveROIRelative(i, del.x(), del.y());
//          }
//        }
//      }
//    }
//  }
//}

//void QxrdImagePlot::roiMouseAdded(const QVector<QPointF> &p)
//{
//  QxrdROICoordinatesListModelPtr roiModel(m_ROIModel);

//  if (roiModel && p.count() == 2) {
//    int roiId, inOut, roiType, roiPtIndex;

//    if (roiModel->identifyROIPointByMouse(p.value(0),  scaledDelta(5,5), roiId, inOut, roiType, roiPtIndex)) {
//      printf("Add ROI Pt %d:%d Pt: %d\n", roiId, inOut, roiPtIndex);
//    }
//  }
//}

//void QxrdImagePlot::roiMouseRemoved(const QPointF &pt)
//{
//  QxrdROICoordinatesListModelPtr roiModel(m_ROIModel);

//  if (roiModel) {
//    int roiId, inOut, roiType, roiPtIndex;

//    if (roiModel->identifyROIPointByMouse(pt,  scaledDelta(5,5), roiId, inOut, roiType, roiPtIndex)) {
//      printf("Move ROI %d:%d Pt: %d\n", roiId, inOut, roiPtIndex);
//    }
//  }
//}

//void QxrdImagePlot::roiMouseRotated(const QVector<QPointF> &p)
//{
//  if (p.count() == 2) {
//    QxrdROICoordinatesListModelPtr roiModel(m_ROIModel);

//    if (roiModel && m_ROISelection) {
//      int n = roiModel->roiCount();

//      for (int i=0; i<n; i++) {
//        if (m_ROISelection->rowIntersectsSelection(i,QModelIndex())) {
//          roiModel->rotateROIByMouse(i, p.value(0), p.value(1));
//        }
//      }
//    }
//  }
//}

//void QxrdImagePlot::roiMouseResized(const QVector<QPointF> &p)
//{
//  QxrdROICoordinatesListModelPtr roiModel(m_ROIModel);

//  if (roiModel && p.count() == 2) {
//    int roiId, inOut, roiType, roiPtIndex;

//    if (roiModel->identifyROIPointByMouse(p.value(0),  scaledDelta(5,5), roiId, inOut, roiType, roiPtIndex)) {
//      printf("Resize ROI %d:%d\n", roiId, inOut);
//    }
//  }
//}

QPointF QxrdImagePlot::scaledDelta(double dx, double dy)
{
  QwtScaleMap xMap = canvasMap(QwtPlot::xBottom);
  QwtScaleMap yMap = canvasMap(QwtPlot::yLeft);

  double tdx = xMap.transform(dx) - xMap.transform(0);
  double tdy = yMap.transform(dy) - yMap.transform(0);

  return QPointF(tdx, tdy);
}
