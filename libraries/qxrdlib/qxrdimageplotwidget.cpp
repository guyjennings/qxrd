#include "qxrdimageplotwidget.h"
#include "qxrdzoomincommand.h"
#include "qxrdzoomoutcommand.h"
#include "qxrdzoomallcommand.h"
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

QxrdImagePlotWidget::QxrdImagePlotWidget(QWidget *parent)
  : QxrdPlotWidget(parent),
    m_ImageRaster(),
    m_MaskRaster(),
    m_OverflowRaster(),
    m_ImageSpectrogram(),
    m_MaskSpectrogram(),
    m_OverflowSpectrogram()
{
  connect(&m_ImageTimer, &QTimer::timeout, this, &QxrdImagePlotWidget::updateImage);
  connect(&m_ImageTimer, &QTimer::timeout, this, &QxrdImagePlotWidget::updateMask);

  m_ImageTimer.start(1000);
}

QxrdImagePlotWidget::~QxrdImagePlotWidget()
{
}

void QxrdImagePlotWidget::initialize(QxrdImagePlotWidgetSettingsWPtr settings)
{
  QxrdPlotWidget::initialize(settings);

  m_ImageSettings = settings;

  addPlotCommand(QxrdPlotCommandPtr(new QxrdMaskCirclesCommand("Mask Circles", this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdMaskPolygonsCommand("Mask Polygons", this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdSetCenterCommand("Set Center", this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdPowderPointsCommand("Powder Points", this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdSliceCommand("Slice", this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdMeasureCommand("Measure", this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdHistogramCommand("Histogram", this, settings)));
  addPlotCommandSpacer();

  addPlotCommand(QxrdPlotCommandPtr(new QxrdScalingSubmenuCommand("Display", this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdColorMapSubmenuCommand("Display", this, settings)));
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

void QxrdImagePlotWidget::newImage(QcepDoubleImageDataPtr img)
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

void QxrdImagePlotWidget::replotImage()
{
  QxrdImagePlotWidgetSettingsPtr set(m_ImageSettings);

  if (set) {
    if (m_ImageData) {
      if (m_ImageRaster == NULL) {
        m_ImageRaster =
            new QxrdRasterData(m_ImageData,
                               set->get_InterpolatePixels());
      } else {
        m_ImageRaster -> setImage(m_ImageData);
      }

      if (m_OverflowRaster == NULL) {
        m_OverflowRaster =
            new QxrdOverflowRasterData(m_ImageData,
                                       set->get_OverflowLevel());
      }

      if (m_ImageSpectrogram == NULL) {
        m_ImageSpectrogram = new QwtPlotSpectrogram();

        m_ImageSpectrogram -> attach(m_Plot);
        m_ImageSpectrogram -> setRenderThreadCount(0);
      }

      m_ImageSpectrogram -> setData(m_ImageRaster/*.data()*/);

      if (m_OverflowSpectrogram == NULL) {
        m_OverflowSpectrogram = new QwtPlotSpectrogram();

        m_OverflowSpectrogram -> attach(m_Plot);
        m_OverflowSpectrogram -> setAlpha(256);
        m_OverflowSpectrogram -> setRenderThreadCount(0);
      }

      m_OverflowSpectrogram -> setData(m_OverflowRaster);
    }
  }
}

void QxrdImagePlotWidget::replotMask()
{
  if (m_MaskData) {
    if (m_MaskRaster == NULL ||
        m_MaskData->get_Width()  != m_MaskRaster->width() ||
        m_MaskData->get_Height() != m_MaskRaster->height()) {

      if(m_MaskRaster == NULL) {
        m_MaskRaster =
            new QxrdMaskRasterData(m_MaskData);
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
    }
  }
}
