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
    m_ImagePlotSettings(),
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
  setObjectName("imageGraph");
}

void QxrdImagePlot::init(QxrdImagePlotSettingsWPtr settings)
{
  QxrdPlot::init(settings);

  m_ImagePlotSettings = settings;

  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

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
  m_MaskImage -> setAlpha(set && set->get_MaskShown() ? m_MaskAlpha : 0);
  m_MaskImage -> attach(this);

  m_OverflowImage = new QwtPlotSpectrogram();
  m_OverflowImage -> setAlpha(set && set->get_OverflowShown() ? m_OverflowAlpha : 0);
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

  if (set) {
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
    connect(set->prop_DisplayColorMap(), SIGNAL(valueChanged(int,int)), this, SLOT(setColorMap(int)));
    connect(set->prop_DisplayLog(), SIGNAL(valueChanged(bool,int)), this, SLOT(redoColorMap()));
  }

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

//void QxrdImagePlot::setSaver(QxrdSettingsSaverPtr saver)
//{
//  QxrdPlot::setSaver(saver);

//  prop_ImageShown()->setSaver(saver);
//  prop_MaskShown()->setSaver(saver);
//  prop_OverflowShown()->setSaver(saver);
//  prop_DisplayMinimumPct()->setSaver(saver);
//  prop_DisplayMaximumPct()->setSaver(saver);
//  prop_DisplayMinimumVal()->setSaver(saver);
//  prop_DisplayMaximumVal()->setSaver(saver);
//  prop_DisplayMinimumPctle()->setSaver(saver);
//  prop_DisplayMaximumPctle()->setSaver(saver);
//  prop_DisplayScalingMode()->setSaver(saver);
//  prop_InterpolatePixels()->setSaver(saver);
//  prop_MaintainAspectRatio()->setSaver(saver);
//  prop_DisplayColorMap()->setSaver(saver);
//  prop_DisplayLog()->setSaver(saver);
//}

QxrdDataProcessorWPtr QxrdImagePlot::processor() const
{
  return m_DataProcessor;
}

QxrdImagePlotSettingsWPtr QxrdImagePlot::imagePlotSettings()
{
  return m_ImagePlotSettings;
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

  if (set) {
    double mindis, maxdis;

    if (set->get_DisplayScalingMode() == PercentageMode) {
      double minv = m_DataRaster.minValue();
      double maxv = m_DataRaster.maxValue();
      double del = maxv-minv;

      mindis = minv+del*set->get_DisplayMinimumPct()/100.0;
      maxdis = minv+del*set->get_DisplayMaximumPct()/100.0;
    } else if (set->get_DisplayScalingMode() == PercentileMode) {
      QwtDoubleInterval range = m_DataRaster.percentileRange(set->get_DisplayMinimumPctle(), set->get_DisplayMaximumPctle());

      mindis = range.minValue();
      maxdis = range.maxValue();
    } else {
      mindis = set->get_DisplayMinimumVal();
      maxdis = set->get_DisplayMaximumVal();
    }

    m_DataRaster.setDisplayedRange(mindis, maxdis);

    replotImage();
  }
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

void QxrdImagePlot::colorMapStart(QColor startColor, QColor endColor)
{
  m_ColorMap.setColorInterval(startColor, endColor);
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

      m_ColorMap.addColorStop(val, col);
    }
  } else {
    m_ColorMap.addColorStop(value1, color1);
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
      m_DataImage -> invalidateCache();
      m_DataImage -> itemChanged();

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
      m_MaskImage -> invalidateCache();
      m_MaskImage -> itemChanged();

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
      m_OverflowImage -> invalidateCache();
      m_OverflowImage -> itemChanged();

      replotImage();
    }
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

  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    m_Data = image;
    m_Overflow = overflow;

    if (!image ||
        image->get_Width() != m_DataRaster.width() ||
        image->get_Height() != m_DataRaster.height()) {
      m_FirstTime = true;
    }

    QxrdRasterData data(image, set->get_InterpolatePixels(), QxrdMaskDataPtr(NULL));

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
  }

  if (g_Application) {
    g_Application->printMessage(tr("plot image took %1 msec").arg(tic.elapsed()));
  }
}

void QxrdImagePlot::onMaskedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr mask)
{
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    m_Data = image;
    m_Mask = mask;

    if (!image ||
        image->get_Width() != m_DataRaster.width() ||
        image->get_Height() != m_DataRaster.height()) {
      m_FirstTime = true;
    }

    QxrdRasterData data(image, set->get_InterpolatePixels(), QxrdMaskDataPtr(NULL));
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

  if (m_MaskRaster.data()) {
    double mask = m_MaskRaster.value(pos.x(),pos.y());
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
