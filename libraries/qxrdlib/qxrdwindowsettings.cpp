#include "qxrdwindowsettings.h"
#include "qxrdwindow.h"
#include "qxrdimageplotsettings.h"
#include "qxrdcenterfinderplotsettings.h"
#include "qxrdintegratorplotsettings.h"
#include "qxrdfilebrowsersettings.h"
#include "qxrdhistogramdialogsettings.h"
#include "qxrddistortioncorrectionplotsettings.h"
#include "qxrdsliceplotsettings.h"
#include "qxrdsynchronizedacquisitionplotsettings.h"
#include "qxrdhistogramplotsettings.h"
#include "qxrdimageplotwidgetsettings.h"
#include "qxrdcenteringplotwidgetsettings.h"
#include "qxrdintegratedplotwidgetsettings.h"
#include "qxrddistortionplotwidgetsettings.h"
#include <QThread>

QxrdWindowSettings::QxrdWindowSettings(QString name, QString desc) :
  inherited(name, desc),
  m_WindowGeometry(this, "windowGeometry", QByteArray(), "Window Geometry Settings"),
  m_WindowState(this, "windowState", QByteArray(), "Window State Settings"),
  m_ImagePlotWidgetSettings(new QxrdImagePlotWidgetSettings(name)),
  m_CenteringPlotWidgetSettings(new QxrdCenteringPlotWidgetSettings(name)),
  m_IntegratedPlotWidgetSettings(new QxrdIntegratedPlotWidgetSettings(name)),
  m_DistortionPlotWidgetSettings(new QxrdDistortionPlotWidgetSettings(name))
{
  m_ImagePlotSettings = QxrdImagePlotSettings::newImagePlotSettings();
  m_CenterFinderPlotSettings = QxrdCenterFinderPlotSettings::newCenterFinderPlotSettings();
  m_IntegratorPlotSettings = QxrdIntegratorPlotSettings::newIntegratorPlotSettings();
  m_FileBrowserSettings = QxrdFileBrowserSettings::newFileBrowserSettings();
  m_HistogramDialogSettings = QxrdHistogramDialogSettings::newHistogramDialogSettings();
  m_DistortionCorrectionPlotSettings = QxrdDistortionCorrectionPlotSettings::newDistortionCorrectionPlotSettings();
}

void QxrdWindowSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_ImagePlotWidgetSettings               -> initialize(sharedFromThis());
  m_CenteringPlotWidgetSettings           -> initialize(sharedFromThis());
  m_IntegratedPlotWidgetSettings          -> initialize(sharedFromThis());
  m_DistortionPlotWidgetSettings          -> initialize(sharedFromThis());
  m_ImagePlotSettings                     -> initialize(sharedFromThis());
  m_CenterFinderPlotSettings              -> initialize(sharedFromThis());
  m_IntegratorPlotSettings                -> initialize(sharedFromThis());
  m_FileBrowserSettings                   -> initialize(sharedFromThis());
  m_HistogramDialogSettings               -> initialize(sharedFromThis());
  m_DistortionCorrectionPlotSettings      -> initialize(sharedFromThis());
}

QxrdWindowSettingsPtr QxrdWindowSettings::newWindowSettings()
{
  QxrdWindowSettingsPtr set(new QxrdWindowSettings("windowSettings", "Window"));

  return set;
}

QcepMainWindowPtr QxrdWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  m_Window =
      QxrdMainWindowPtr(
        new QxrdWindow("mainWindow"));

  return m_Window;
}

void QxrdWindowSettings::registerMetaTypes()
{
  qRegisterMetaType<QxrdWindowSettings*>("QxrdWindowSettings*");
  qRegisterMetaType<QxrdImagePlotSettings*>("QxrdImagePlotSettings*");
  qRegisterMetaType<QxrdCenterFinderPlotSettings*>("QxrdCenterFinderPlotSettings*");
  qRegisterMetaType<QxrdIntegratorPlotSettings*>("QxrdIntegratorPlotSettings*");
  qRegisterMetaType<QxrdFileBrowserSettings*>("QxrdFileBrowserSettings*");
  qRegisterMetaType<QxrdHistogramDialogSettings*>("QxrdHistogramDialogSettings*");
  qRegisterMetaType<QxrdHistogramPlotSettings*>("QxrdHistogramPlotSettings*");
  qRegisterMetaType<QxrdSlicePlotSettings*>("QxrdSlicePlotSettings*");
  qRegisterMetaType<QxrdSynchronizedAcquisitionPlotSettings*>("QxrdSynchronizedAcquisitionPlotSettings*");
  qRegisterMetaType<QxrdDistortionCorrectionPlotSettings*>("QxrdDistortionCorrectionPlotSettings*");
}

void QxrdWindowSettings::readSettings(QSettings *settings)
{
  if (settings) {
    inherited::readSettings(settings);

    if (m_ImagePlotSettings) {
      settings->beginGroup("plot");
      m_ImagePlotSettings->readSettings(settings);
      settings->endGroup();
    }

    if (m_CenterFinderPlotSettings) {
      settings->beginGroup("centerFinderPlot");
      m_CenterFinderPlotSettings->readSettings(settings);
      settings->endGroup();
    }

    if (m_IntegratorPlotSettings) {
      settings->beginGroup("integratorPlot");
      m_IntegratorPlotSettings->readSettings(settings);
      settings->endGroup();
    }

    if (m_FileBrowserSettings) {
      settings->beginGroup("browser");
      m_FileBrowserSettings->readSettings(settings);
      settings->endGroup();
    }

    if (m_HistogramDialogSettings) {
      settings->beginGroup("histogramDialog");
      m_HistogramDialogSettings->readSettings(settings);
      settings->endGroup();
    }

    if (m_DistortionCorrectionPlotSettings) {
      settings->beginGroup("distortionCorrectionPlot");
      m_DistortionCorrectionPlotSettings->readSettings(settings);
      settings->endGroup();
    }

    if (m_ImagePlotWidgetSettings) {
      settings->beginGroup("imagePlotWidget");
      m_ImagePlotWidgetSettings->readSettings(settings);
      settings->endGroup();
    }

    if (m_CenteringPlotWidgetSettings) {
      settings->beginGroup("centeringPlotWidget");
      m_CenteringPlotWidgetSettings->readSettings(settings);
      settings->endGroup();
    }

    if (m_IntegratedPlotWidgetSettings) {
      settings->beginGroup("integratedPlotWidget");
      m_IntegratedPlotWidgetSettings->readSettings(settings);
      settings->endGroup();
    }

    if (m_DistortionPlotWidgetSettings) {
      settings->beginGroup("distortionPlotWidget");
      m_DistortionPlotWidgetSettings->readSettings(settings);
      settings->endGroup();
    }
  }
}

void QxrdWindowSettings::writeSettings(QSettings *settings)
{
  if (settings) {
    inherited::writeSettings(settings);

    if (m_ImagePlotSettings) {
      settings->beginGroup("plot");
      m_ImagePlotSettings->writeSettings(settings);
      settings->endGroup();
    }

    if (m_CenterFinderPlotSettings) {
      settings->beginGroup("centerFinderPlot");
      m_CenterFinderPlotSettings->writeSettings(settings);
      settings->endGroup();
    }

    if (m_IntegratorPlotSettings) {
      settings->beginGroup("integratorPlot");
      m_IntegratorPlotSettings->writeSettings(settings);
      settings->endGroup();
    }

    if (m_FileBrowserSettings) {
      settings->beginGroup("browser");
      m_FileBrowserSettings->writeSettings(settings);
      settings->endGroup();
    }

    if (m_HistogramDialogSettings) {
      settings->beginGroup("histogramDialog");
      m_HistogramDialogSettings->writeSettings(settings);
      settings->endGroup();
    }

    if (m_DistortionCorrectionPlotSettings) {
      settings->beginGroup("distortionCorrectionPlot");
      m_DistortionCorrectionPlotSettings->writeSettings(settings);
      settings->endGroup();
    }

    if (m_ImagePlotWidgetSettings) {
      settings->beginGroup("imagePlotWidget");
      m_ImagePlotWidgetSettings->writeSettings(settings);
      settings->endGroup();
    }

    if (m_CenteringPlotWidgetSettings) {
      settings->beginGroup("centeringPlotWidget");
      m_CenteringPlotWidgetSettings->writeSettings(settings);
      settings->endGroup();
    }

    if (m_IntegratedPlotWidgetSettings) {
      settings->beginGroup("integratedPlotWidget");
      m_IntegratedPlotWidgetSettings->writeSettings(settings);
      settings->endGroup();
    }

    if (m_DistortionPlotWidgetSettings) {
      settings->beginGroup("distortionPlotWidget");
      m_DistortionPlotWidgetSettings->writeSettings(settings);
      settings->endGroup();
    }
  }
}

QxrdImagePlotSettingsWPtr QxrdWindowSettings::imagePlotSettings()
{
  return m_ImagePlotSettings;
}

QxrdCenterFinderPlotSettingsWPtr QxrdWindowSettings::centerFinderPlotSettings()
{
  return m_CenterFinderPlotSettings;
}

QxrdIntegratorPlotSettingsWPtr QxrdWindowSettings::integratorPlotSettings()
{
  return m_IntegratorPlotSettings;
}

QxrdFileBrowserSettingsWPtr QxrdWindowSettings::fileBrowserSettings()
{
  return m_FileBrowserSettings;
}

QxrdHistogramDialogSettingsWPtr QxrdWindowSettings::histogramDialogSettings()
{
  return m_HistogramDialogSettings;
}

QxrdDistortionCorrectionPlotSettingsWPtr QxrdWindowSettings::distortionCorrectionPlotSettings()
{
  return m_DistortionCorrectionPlotSettings;
}

QxrdImagePlotWidgetSettingsPtr QxrdWindowSettings::imagePlotWidgetSettings()
{
  return m_ImagePlotWidgetSettings;
}

QxrdCenteringPlotWidgetSettingsPtr QxrdWindowSettings::centeringPlotWidgetSettings()
{
  return m_CenteringPlotWidgetSettings;
}

QxrdDistortionPlotWidgetSettingsPtr QxrdWindowSettings::distortionPlotWidgetSettings()
{
  return m_DistortionPlotWidgetSettings;
}

QxrdIntegratedPlotWidgetSettingsPtr QxrdWindowSettings::integratedPlotWidgetSettings()
{
  return m_IntegratedPlotWidgetSettings;
}
