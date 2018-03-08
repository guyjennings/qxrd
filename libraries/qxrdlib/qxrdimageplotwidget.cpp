#include "qxrdimageplotwidget.h"
#include "qxrdzoomincommand.h"
#include "qxrdzoomoutcommand.h"
#include "qxrdzoomallcommand.h"
#include "qxrdmaskcommandbutton.h"
#include "qxrdmaskcirclescommand.h"
#include "qxrdmaskpolygonscommand.h"
#include "qxrdsetcentercommand.h"
#include "qxrdpowderpointscommand.h"
#include "qxrdslicecommand.h"
#include "qxrdmeasurecommand.h"
#include "qxrdhistogramcommand.h"
#include "qxrdimageplot.h"
#include "qxrdimageplotwidgetsettings.h"
#include "qxrdscalingsubmenucommand.h"
#include "qxrdcolormapsubmenucommand.h"
#include "qxrdimageplotwidgetdialog-ptr.h"
#include "qxrdimageplotwidgetdialog.h"
#include "qxrdrasterdata.h"
#include "qxrdmaskrasterdata.h"
#include "qxrdoverflowrasterdata.h"
#include "qxrdcolormaplibrary.h"
#include "qxrdcolormap.h"
#include "qxrdmaskcolormap.h"
#include "qxrdprocessor.h"
#include "qxrdmaskstack-ptr.h"
#include "qxrdmaskstack.h"
#include "qxrdcentermarker.h"
#include "qxrdroioverlay.h"
#include "qxrdpowderoverlay.h"

QxrdImagePlotWidget::QxrdImagePlotWidget(QWidget *parent)
  : QxrdPlotWidget(parent),
    m_ImageRaster(),
    m_MaskRaster(),
    m_OverflowRaster(),
    m_ImageSpectrogram(),
    m_MaskSpectrogram(),
    m_OverflowSpectrogram(),
    m_PlotRescaler(NULL)
{
  connect(&m_ImageTimer, &QTimer::timeout, this, &QxrdImagePlotWidget::updateImage);
  connect(&m_ImageTimer, &QTimer::timeout, this, &QxrdImagePlotWidget::updateMask);

  m_ImageTimer.start(1000);
}

QxrdImagePlotWidget::~QxrdImagePlotWidget()
{
}

void QxrdImagePlotWidget::initialize(QxrdImagePlotWidgetSettingsWPtr settings,
                                     QxrdProcessorWPtr processor)
{
  QxrdPlotWidget::initialize(settings);

  m_ImageSettings = settings;
  m_Processor     = processor;

  QxrdProcessorPtr p(m_Processor);

  if (p) {
    QxrdMaskStackPtr m(p->maskStack());

    if (m) {
      addPlotCommand(QxrdPlotCommandPtr(new QxrdMaskCommandButton("Mask Commands", this, settings, m)));
//      addPlotCommand(QxrdPlotCommandPtr(new QxrdMaskCirclesCommand("Mask Circles", this, settings, m)));
//      addPlotCommand(QxrdPlotCommandPtr(new QxrdMaskPolygonsCommand("Mask Polygons", this, settings, m)));

      connect(m.data(), &QxrdMaskStack::maskChanged,
              this,     &QxrdImagePlotWidget::maskChanged);
    }

    QxrdCenterFinderPtr c(p->centerFinder());

    if (c) {
      addPlotCommand(QxrdPlotCommandPtr(
                       new QxrdSetCenterCommand(this, settings, c)));

      addPlotOverlay(QxrdPlotOverlayPtr(
                       new QxrdCenterMarker("Center Marker", this, settings, c)));
    }

    QxrdPowderRingsModelPtr pw(p->powderRings());

    if (pw) {
      addPlotOverlay(QxrdPlotOverlayPtr(
                       new QxrdPowderOverlay("Powder Rings", this, settings, pw)));
    }

    QxrdROIModelPtr rois(p->roiModel());

    if (rois) {
      addPlotOverlay((QxrdPlotOverlayPtr(
                        new QxrdROIOverlay("ROIs", this, settings, rois))));
    }
  }

  addPlotCommand(QxrdPlotCommandPtr(new QxrdPowderPointsCommand(this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdSliceCommand(this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdMeasureCommand(this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdHistogramCommand(this, settings)));
  addPlotCommandSpacer();

  addPlotCommand(QxrdPlotCommandPtr(new QxrdScalingSubmenuCommand(this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdColorMapSubmenuCommand("Display", this, settings)));

  QxrdImagePlotWidgetSettingsPtr set(m_ImageSettings);

  if (set) {
    connect(set->prop_DisplayColorMap(),     &QcepIntProperty::valueChanged, this, &QxrdImagePlotWidget::replotImage);
    connect(set->prop_DisplayLog(),          &QcepBoolProperty::valueChanged, this, &QxrdImagePlotWidget::replotImage);
    connect(set->prop_DisplayScalingMode(),  &QcepIntProperty::valueChanged, this, &QxrdImagePlotWidget::replotImage);
    connect(set->prop_DisplayMaximumPct(),   &QcepDoubleProperty::valueChanged, this, &QxrdImagePlotWidget::replotImage);
    connect(set->prop_DisplayMaximumPct(),   &QcepDoubleProperty::valueChanged, this, &QxrdImagePlotWidget::replotImage);
    connect(set->prop_DisplayMaximumPctle(), &QcepDoubleProperty::valueChanged, this, &QxrdImagePlotWidget::replotImage);
    connect(set->prop_DisplayMaximumPctle(), &QcepDoubleProperty::valueChanged, this, &QxrdImagePlotWidget::replotImage);
    connect(set->prop_DisplayMaximumVal(),   &QcepDoubleProperty::valueChanged, this, &QxrdImagePlotWidget::replotImage);
    connect(set->prop_DisplayMaximumVal(),   &QcepDoubleProperty::valueChanged, this, &QxrdImagePlotWidget::replotImage);
    connect(set->prop_ImageShown(),          &QcepBoolProperty::valueChanged, this, &QxrdImagePlotWidget::replotImage);
    connect(set->prop_MaskShown(),           &QcepBoolProperty::valueChanged, this, &QxrdImagePlotWidget::replotMask);
    connect(set->prop_OverflowShown(),       &QcepBoolProperty::valueChanged, this, &QxrdImagePlotWidget::replotImage);
    connect(set->prop_OverflowLevel(),       &QcepDoubleProperty::valueChanged, this, &QxrdImagePlotWidget::replotImage);
    connect(set->prop_InterpolatePixels(),   &QcepBoolProperty::valueChanged, this, &QxrdImagePlotWidget::replotImage);
    connect(set->prop_MaintainAspectRatio(), &QcepBoolProperty::valueChanged, this, &QxrdImagePlotWidget::replotImage);

    m_PlotRescaler = new QwtPlotRescaler(m_Plot->canvas(), QwtPlot::yLeft, QwtPlotRescaler::Expanding);
    m_PlotRescaler -> setEnabled(set->prop_MaintainAspectRatio());
    m_PlotRescaler -> setExpandingDirection(QwtPlotRescaler::ExpandBoth);
  }

  updateColorMap();
}

void QxrdImagePlotWidget::editPreferences()
{
  QxrdImagePlotWidgetDialogPtr prefs =
      QxrdImagePlotWidgetDialogPtr(
        new QxrdImagePlotWidgetDialog(NULL, m_ImageSettings));

  if (prefs) {
    prefs -> exec();
  }
}

QString QxrdImagePlotWidget::scalingModeName(int n)
{
  QString res = "";

  switch (n) {
  case PercentageScalingMode:
    res = "Percentage Scaling";
    break;

  case PercentileScalingMode:
    res = "Percentile Scaling";
    break;

  case AbsoluteScalingMode:
    res = "Absolute Scaling";
    break;
  }

  return res;
}
void QxrdImagePlotWidget::newImage(QcepImageDataBasePtr img)
{
  m_NewImageData = img;
}

void QxrdImagePlotWidget::newMask(QcepMaskDataPtr msk)
{
  m_NewMaskData = msk;
}

void QxrdImagePlotWidget::updateImage()
{
  if (m_NewImageData) {
    m_ImageData    = m_NewImageData;
    m_NewImageData = QcepDoubleImageDataPtr();

    replotImage();
  }
}

void QxrdImagePlotWidget::updateMask()
{
  if (m_NewMaskData) {
    m_MaskData    = m_NewMaskData;
    m_NewMaskData = QcepMaskDataPtr();

    replotMask();
  }
}

void QxrdImagePlotWidget::maskChanged()
{
  QxrdProcessorPtr p(m_Processor);

  if (p) {
    QxrdMaskStackPtr m(p->maskStack());

    if (m) {
      m_MaskData = m->mask();
    }
  }

  replotMask();
}

void QxrdImagePlotWidget::replotImage()
{
  QxrdImagePlotWidgetSettingsPtr set(m_ImageSettings);

  if (set && m_ImageData) {
    if (m_Plot) {
      m_Plot -> setData(m_ImageData);
    }

    if (m_ImageRaster == NULL) {
      m_ImageRaster =
          new QxrdRasterData(m_ImageData,
                             m_ImageSettings);
    } else {
      m_ImageRaster -> setImage(m_ImageData);
    }

    if (m_OverflowRaster == NULL) {
      m_OverflowRaster =
          new QxrdOverflowRasterData(m_ImageData,
                                     m_ImageSettings);
    }

    if (m_ImageSpectrogram == NULL) {
      m_ImageSpectrogram = new QwtPlotSpectrogram();

      m_ImageSpectrogram -> attach(m_Plot);
      m_ImageSpectrogram -> setRenderThreadCount(0);
    }

    m_ImageSpectrogram -> setData(m_ImageRaster/*.data()*/);
    m_ImageSpectrogram -> setVisible(set->get_ImageShown());

    if (m_OverflowSpectrogram == NULL) {
      m_OverflowSpectrogram = new QwtPlotSpectrogram();

      m_OverflowSpectrogram -> attach(m_Plot);
      m_OverflowSpectrogram -> setAlpha(256);
      m_OverflowSpectrogram -> setRenderThreadCount(0);
    }

    m_OverflowSpectrogram -> setData(m_OverflowRaster);
    m_OverflowSpectrogram -> setVisible(set->get_OverflowShown());

    m_PlotRescaler -> setEnabled(set->get_MaintainAspectRatio());

    double mindis, maxdis, minv, maxv, del;

    switch (set->get_DisplayScalingMode()) {
    case PercentageScalingMode:
      minv = m_ImageData->minValue();
      maxv = m_ImageData->maxValue();
      del  = maxv - minv;

      mindis = minv+del*set->get_DisplayMinimumPct()/100.0;
      maxdis = minv+del*set->get_DisplayMaximumPct()/100.0;
      break;

    case PercentileScalingMode:
      mindis = m_ImageData->percentileValue(set->get_DisplayMinimumPctle());
      maxdis = m_ImageData->percentileValue(set->get_DisplayMaximumPctle());
      break;

    default:
    case AbsoluteScalingMode:
      mindis = set->get_DisplayMinimumVal();
      maxdis = set->get_DisplayMaximumVal();
      break;
    }

    m_ImageRaster->setDisplayedRange(mindis, maxdis);
  }

  updateColorMap();
}

void QxrdImagePlotWidget::replotMask()
{
  QxrdImagePlotWidgetSettingsPtr set(m_ImageSettings);

  if (set && m_MaskData) {
    if (m_Plot) {
      m_Plot -> setMask(m_MaskData);
    }

    if(m_MaskRaster == NULL) {
      m_MaskRaster =
          new QxrdMaskRasterData(m_MaskData,
                                 m_ImageSettings);
    } else {
      m_MaskRaster -> setMask(m_MaskData);
    }

    if (m_MaskSpectrogram == NULL) {
      m_MaskSpectrogram =
          new QwtPlotSpectrogram();

      m_MaskSpectrogram -> attach(m_Plot);
      m_MaskSpectrogram -> setAlpha(80);
      m_MaskSpectrogram -> setRenderThreadCount(0);
    }

    m_MaskSpectrogram -> setData(m_MaskRaster);
    m_MaskSpectrogram -> setVisible(set->get_MaskShown());
  }

  updateColorMap();
}

void QxrdImagePlotWidget::updateColorMap()
{
  QxrdImagePlotWidgetSettingsPtr set(m_ImageSettings);

  if (set) {
    int mapIndex = set->get_DisplayColorMap();

    if (m_ImageSpectrogram) {
      m_ImageSpectrogram    -> setColorMap(QxrdColorMapLibrary::newImageColorMap(mapIndex, set));
    }

    if (m_OverflowSpectrogram) {
      m_OverflowSpectrogram -> setColorMap(QxrdColorMapLibrary::newOverflowColorMap(mapIndex, set));
    }

    if (m_MaskSpectrogram) {
      m_MaskSpectrogram     -> setColorMap(QxrdColorMapLibrary::newMaskColorMap(mapIndex, set));
    }

    updateTrackerPen(mapIndex);
  }

  replotGraph();
}

void QxrdImagePlotWidget::replotGraph()
{
  m_Plot->replot();
}
