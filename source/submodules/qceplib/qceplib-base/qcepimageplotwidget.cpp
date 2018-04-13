#include "qcepimageplotwidget.h"
#include "qcepzoomincommand.h"
#include "qcepzoomoutcommand.h"
#include "qcepzoomallcommand.h"
#include "qcepmaskcommandbutton.h"
#include "qcepmaskcirclescommand.h"
#include "qcepmaskpolygonscommand.h"
#include "qcepsetcentercommand.h"
#include "qceppowderpointscommand.h"
#include "qcepslicecommand.h"
#include "qcepmeasurecommand.h"
#include "qcephistogramcommand.h"
#include "qcepimageplot.h"
#include "qcepimageplotwidgetsettings.h"
#include "qcepscalingsubmenucommand.h"
#include "qcepcolormapsubmenucommand.h"
#include "qcepimageplotwidgetdialog-ptr.h"
#include "qcepimageplotwidgetdialog.h"
#include "qceprasterdata.h"
#include "qcepmaskrasterdata.h"
#include "qcepoverflowrasterdata.h"
#include "qcepcolormaplibrary.h"
#include "qcepcolormap.h"
#include "qcepmaskcolormap.h"
#include "qcepmaskstack-ptr.h"
#include "qcepmaskstack.h"
#include "qcepcentermarker.h"
#include "qceproioverlay.h"
#include "qceppowderoverlay.h"

QcepImagePlotWidget::QcepImagePlotWidget(QWidget *parent)
  : QcepPlotWidget(parent),
    m_ImageRaster(),
    m_MaskRaster(),
    m_OverflowRaster(),
    m_ImageSpectrogram(),
    m_MaskSpectrogram(),
    m_OverflowSpectrogram(),
    m_PlotRescaler(NULL),
    m_ImageSettings(),
    m_MaskStack(),
    m_CenterFinder(),
    m_PowderRings(),
    m_ROIModel()
{
  connect(&m_ImageTimer, &QTimer::timeout, this, &QcepImagePlotWidget::updateImage);
  connect(&m_ImageTimer, &QTimer::timeout, this, &QcepImagePlotWidget::updateMask);

  m_ImageTimer.start(1000);
}

QcepImagePlotWidget::~QcepImagePlotWidget()
{
}

void QcepImagePlotWidget::initialize(QcepImagePlotWidgetSettingsWPtr settings)
{
  QcepPlotWidget::initialize(settings);

  m_ImageSettings = settings;

  addPlotCommand(QcepPlotCommandPtr(new QcepPowderPointsCommand(this, settings)));
  addPlotCommand(QcepPlotCommandPtr(new QcepSliceCommand(this, settings)));
  addPlotCommand(QcepPlotCommandPtr(new QcepMeasureCommand(this, settings)));
  addPlotCommand(QcepPlotCommandPtr(new QcepHistogramCommand(this, settings)));
  addPlotCommandSpacer();

  addPlotCommand(QcepPlotCommandPtr(new QcepScalingSubmenuCommand(this, settings)));
  addPlotCommand(QcepPlotCommandPtr(new QcepColorMapSubmenuCommand("Display", this, settings)));

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

void QcepImagePlotWidget::setMaskStack(QcepMaskStackWPtr masks)
{
  m_MaskStack = masks;

  QcepMaskStackPtr m(m_MaskStack);

  if (m) {
    addPlotCommand(QcepPlotCommandPtr(new QcepMaskCommandButton("Mask Commands", this, m_ImageSettings, m)));
//      addPlotCommand(QcepPlotCommandPtr(new QcepMaskCirclesCommand("Mask Circles", this, m_ImageSettings, m)));
//      addPlotCommand(QcepPlotCommandPtr(new QcepMaskPolygonsCommand("Mask Polygons", this, m_ImageSettings, m)));

    connect(m.data(), &QcepMaskStack::maskChanged,
            this,     &QcepImagePlotWidget::maskChanged);
  }
}

void QcepImagePlotWidget::setCenterFinder(QcepCenterFinderWPtr center)
{
  m_CenterFinder = center;

  QcepCenterFinderPtr c(m_CenterFinder);

  if (c) {
    addPlotCommand(QcepPlotCommandPtr(
                     new QcepSetCenterCommand(this, m_ImageSettings, c)));

    addPlotOverlay(QcepPlotOverlayPtr(
                     new QcepCenterMarker("Center Marker", this, m_ImageSettings, c)));
  }
}

void QcepImagePlotWidget::setPowderRings(QcepPowderRingsModelWPtr powderRings)
{
  m_PowderRings = powderRings;

  QcepPowderRingsModelPtr pw(m_PowderRings);

  if (pw) {
    addPlotOverlay(QcepPlotOverlayPtr(
                     new QcepPowderOverlay("Powder Rings", this, m_ImageSettings, pw)));
  }
}

void QcepImagePlotWidget::setROIModel(QcepROIModelWPtr roiModel)
{
  m_ROIModel = roiModel;

  QcepROIModelPtr rois(m_ROIModel);

  if (rois) {
    addPlotOverlay(QcepPlotOverlayPtr(
                      new QcepROIOverlay("ROIs", this, m_ImageSettings, rois)));
  }
}

void QcepImagePlotWidget::editPreferences()
{
  QcepImagePlotWidgetDialogPtr prefs =
      QcepImagePlotWidgetDialogPtr(
        new QcepImagePlotWidgetDialog(NULL, m_ImageSettings));

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
  QcepMaskStackPtr m(m_MaskStack);

  if (m) {
    m_MaskData = m->mask();

    replotMask();
  }
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
