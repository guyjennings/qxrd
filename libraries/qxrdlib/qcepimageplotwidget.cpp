#include "qcepimageplotwidget.h"
#include "qcepzoomincommand.h"
#include "qcepzoomoutcommand.h"
#include "qcepzoomallcommand.h"
#include "qxrdmaskcommandbutton.h"
#include "qxrdmaskcirclescommand.h"
#include "qxrdmaskpolygonscommand.h"
#include "qxrdsetcentercommand.h"
#include "qxrdpowderpointscommand.h"
#include "qxrdslicecommand.h"
#include "qxrdmeasurecommand.h"
#include "qxrdhistogramcommand.h"
#include "qxrdimageplot.h"
#include "qcepimageplotwidgetsettings.h"
#include "qxrdscalingsubmenucommand.h"
#include "qxrdcolormapsubmenucommand.h"
#include "qxrdimageplotwidgetdialog-ptr.h"
#include "qxrdimageplotwidgetdialog.h"
#include "qceprasterdata.h"
#include "qcepmaskrasterdata.h"
#include "qcepoverflowrasterdata.h"
#include "qcepcolormaplibrary.h"
#include "qcepcolormap.h"
#include "qcepmaskcolormap.h"
#include "qxrdprocessor.h"
#include "qxrdmaskstack-ptr.h"
#include "qxrdmaskstack.h"
#include "qxrdcentermarker.h"
#include "qxrdroioverlay.h"
#include "qxrdpowderoverlay.h"

QcepImagePlotWidget::QcepImagePlotWidget(QWidget *parent)
  : QcepPlotWidget(parent),
    m_ImageRaster(),
    m_MaskRaster(),
    m_OverflowRaster(),
    m_ImageSpectrogram(),
    m_MaskSpectrogram(),
    m_OverflowSpectrogram(),
    m_PlotRescaler(NULL)
{
  connect(&m_ImageTimer, &QTimer::timeout, this, &QcepImagePlotWidget::updateImage);
  connect(&m_ImageTimer, &QTimer::timeout, this, &QcepImagePlotWidget::updateMask);

  m_ImageTimer.start(1000);
}

QcepImagePlotWidget::~QcepImagePlotWidget()
{
}

void QcepImagePlotWidget::initialize(QcepImagePlotWidgetSettingsWPtr settings,
                                     QxrdProcessorWPtr processor)
{
  QcepPlotWidget::initialize(settings);

  m_ImageSettings = settings;
  m_Processor     = processor;

  QxrdProcessorPtr p(m_Processor);

  if (p) {
    QxrdMaskStackPtr m(p->maskStack());

    if (m) {
      addPlotCommand(QcepPlotCommandPtr(new QxrdMaskCommandButton("Mask Commands", this, settings, m)));
//      addPlotCommand(QcepPlotCommandPtr(new QxrdMaskCirclesCommand("Mask Circles", this, settings, m)));
//      addPlotCommand(QcepPlotCommandPtr(new QxrdMaskPolygonsCommand("Mask Polygons", this, settings, m)));

      connect(m.data(), &QxrdMaskStack::maskChanged,
              this,     &QcepImagePlotWidget::maskChanged);
    }

    QxrdCenterFinderPtr c(p->centerFinder());

    if (c) {
      addPlotCommand(QcepPlotCommandPtr(
                       new QxrdSetCenterCommand(this, settings, c)));

      addPlotOverlay(QcepPlotOverlayPtr(
                       new QxrdCenterMarker("Center Marker", this, settings, c)));
    }

    QxrdPowderRingsModelPtr pw(p->powderRings());

    if (pw) {
      addPlotOverlay(QcepPlotOverlayPtr(
                       new QxrdPowderOverlay("Powder Rings", this, settings, pw)));
    }

    QxrdROIModelPtr rois(p->roiModel());

    if (rois) {
      addPlotOverlay(QcepPlotOverlayPtr(
                        new QxrdROIOverlay("ROIs", this, settings, rois)));
    }
  }

  addPlotCommand(QcepPlotCommandPtr(new QxrdPowderPointsCommand(this, settings)));
  addPlotCommand(QcepPlotCommandPtr(new QxrdSliceCommand(this, settings)));
  addPlotCommand(QcepPlotCommandPtr(new QxrdMeasureCommand(this, settings)));
  addPlotCommand(QcepPlotCommandPtr(new QxrdHistogramCommand(this, settings)));
  addPlotCommandSpacer();

  addPlotCommand(QcepPlotCommandPtr(new QxrdScalingSubmenuCommand(this, settings)));
  addPlotCommand(QcepPlotCommandPtr(new QxrdColorMapSubmenuCommand("Display", this, settings)));

  QcepImagePlotWidgetSettingsPtr set(m_ImageSettings);

  if (set) {
    connect(set->prop_DisplayColorMap(),     &QcepIntProperty::valueChanged, this, &QcepImagePlotWidget::replotImage);
    connect(set->prop_DisplayLog(),          &QcepBoolProperty::valueChanged, this, &QcepImagePlotWidget::replotImage);
    connect(set->prop_DisplayScalingMode(),  &QcepIntProperty::valueChanged, this, &QcepImagePlotWidget::replotImage);
    connect(set->prop_DisplayMaximumPct(),   &QcepDoubleProperty::valueChanged, this, &QcepImagePlotWidget::replotImage);
    connect(set->prop_DisplayMaximumPct(),   &QcepDoubleProperty::valueChanged, this, &QcepImagePlotWidget::replotImage);
    connect(set->prop_DisplayMaximumPctle(), &QcepDoubleProperty::valueChanged, this, &QcepImagePlotWidget::replotImage);
    connect(set->prop_DisplayMaximumPctle(), &QcepDoubleProperty::valueChanged, this, &QcepImagePlotWidget::replotImage);
    connect(set->prop_DisplayMaximumVal(),   &QcepDoubleProperty::valueChanged, this, &QcepImagePlotWidget::replotImage);
    connect(set->prop_DisplayMaximumVal(),   &QcepDoubleProperty::valueChanged, this, &QcepImagePlotWidget::replotImage);
    connect(set->prop_ImageShown(),          &QcepBoolProperty::valueChanged, this, &QcepImagePlotWidget::replotImage);
    connect(set->prop_MaskShown(),           &QcepBoolProperty::valueChanged, this, &QcepImagePlotWidget::replotMask);
    connect(set->prop_OverflowShown(),       &QcepBoolProperty::valueChanged, this, &QcepImagePlotWidget::replotImage);
    connect(set->prop_OverflowLevel(),       &QcepDoubleProperty::valueChanged, this, &QcepImagePlotWidget::replotImage);
    connect(set->prop_InterpolatePixels(),   &QcepBoolProperty::valueChanged, this, &QcepImagePlotWidget::replotImage);
    connect(set->prop_MaintainAspectRatio(), &QcepBoolProperty::valueChanged, this, &QcepImagePlotWidget::replotImage);

    m_PlotRescaler = new QwtPlotRescaler(m_Plot->canvas(), QwtPlot::yLeft, QwtPlotRescaler::Expanding);
    m_PlotRescaler -> setEnabled(set->prop_MaintainAspectRatio());
    m_PlotRescaler -> setExpandingDirection(QwtPlotRescaler::ExpandBoth);
  }

  updateColorMap();
}

void QcepImagePlotWidget::editPreferences()
{
  QxrdImagePlotWidgetDialogPtr prefs =
      QxrdImagePlotWidgetDialogPtr(
        new QxrdImagePlotWidgetDialog(NULL, m_ImageSettings));

  if (prefs) {
    prefs -> exec();
  }
}

QString QcepImagePlotWidget::scalingModeName(int n)
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
void QcepImagePlotWidget::newImage(QcepImageDataBasePtr img)
{
  m_NewImageData = img;
}

void QcepImagePlotWidget::newMask(QcepMaskDataPtr msk)
{
  m_NewMaskData = msk;
}

void QcepImagePlotWidget::updateImage()
{
  if (m_NewImageData) {
    m_ImageData    = m_NewImageData;
    m_NewImageData = QcepDoubleImageDataPtr();

    replotImage();
  }
}

void QcepImagePlotWidget::updateMask()
{
  if (m_NewMaskData) {
    m_MaskData    = m_NewMaskData;
    m_NewMaskData = QcepMaskDataPtr();

    replotMask();
  }
}

void QcepImagePlotWidget::maskChanged()
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

void QcepImagePlotWidget::replotImage()
{
  QcepImagePlotWidgetSettingsPtr set(m_ImageSettings);
  QcepImageDataBasePtr img(m_ImageData);

  if (set && img) {
    if (m_Plot) {
      m_Plot -> setData(img);
    }

    if (m_ImageRaster == NULL) {
      m_ImageRaster =
          new QcepRasterData(img,
                             m_ImageSettings);
    } else {
      m_ImageRaster -> setImage(img);
    }

    if (m_OverflowRaster == NULL) {
      m_OverflowRaster =
          new QcepOverflowRasterData(img,
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
      minv = img->minValue();
      maxv = img->maxValue();
      del  = maxv - minv;

      mindis = minv+del*set->get_DisplayMinimumPct()/100.0;
      maxdis = minv+del*set->get_DisplayMaximumPct()/100.0;
      break;

    case PercentileScalingMode:
      mindis = img->percentileValue(set->get_DisplayMinimumPctle());
      maxdis = img->percentileValue(set->get_DisplayMaximumPctle());
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

void QcepImagePlotWidget::replotMask()
{
  QcepImagePlotWidgetSettingsPtr set(m_ImageSettings);
  QcepMaskDataPtr mask(m_MaskData);

  if (set && mask) {
    if (m_Plot) {
      m_Plot -> setMask(mask);
    }

    if(m_MaskRaster == NULL) {
      m_MaskRaster =
          new QcepMaskRasterData(mask,
                                 m_ImageSettings);
    } else {
      m_MaskRaster -> setMask(mask);
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

void QcepImagePlotWidget::updateColorMap()
{
  QcepImagePlotWidgetSettingsPtr set(m_ImageSettings);

  if (set) {
    int mapIndex = set->get_DisplayColorMap();

    if (m_ImageSpectrogram) {
      m_ImageSpectrogram    -> setColorMap(QcepColorMapLibrary::newImageColorMap(mapIndex, set));
    }

    if (m_OverflowSpectrogram) {
      m_OverflowSpectrogram -> setColorMap(QcepColorMapLibrary::newOverflowColorMap(mapIndex, set));
    }

    if (m_MaskSpectrogram) {
      m_MaskSpectrogram     -> setColorMap(QcepColorMapLibrary::newMaskColorMap(mapIndex, set));
    }

    updateTrackerPen(mapIndex);
  }

  replotGraph();
}

void QcepImagePlotWidget::replotGraph()
{
  if (m_Plot) {
    m_Plot->replot();
  }
}
