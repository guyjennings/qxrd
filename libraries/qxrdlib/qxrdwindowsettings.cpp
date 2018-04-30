#include "qxrdwindowsettings.h"
#include "qxrdwindow.h"
#include "qcepimageplotsettings.h"
#include "qxrdintegratorplotsettings.h"
#include "qcepfilebrowsersettings.h"
#include "qxrdhistogramdialogsettings.h"
#include "qcepimageplotwidgetsettings.h"
#include "qxrdcenteringplotwidgetsettings.h"
#include "qxrdintegratedplotwidgetsettings.h"
#include "qxrddistortionplotwidgetsettings.h"
#include <QThread>

QxrdWindowSettings::QxrdWindowSettings(QString name) :
  inherited(name, "Main Window"),
  m_WindowGeometry(this, "windowGeometry", QByteArray(), "Window Geometry Settings"),
  m_WindowState(this, "windowState", QByteArray(), "Window State Settings"),
  m_ImagePlotWidgetSettings(NEWPTR(QcepImagePlotWidgetSettings(name+"ImagePlot"))),
  m_CenteringPlotWidgetSettings(NEWPTR(QxrdCenteringPlotWidgetSettings(name+"CenteringPlot"))),
  m_IntegratedPlotWidgetSettings(NEWPTR(QxrdIntegratedPlotWidgetSettings(name+"IntegratedPlot"))),
  m_DistortionPlotWidgetSettings(NEWPTR(QxrdDistortionPlotWidgetSettings(name+"DistortionPlot"))),
  m_ImagePlotSettings(NEWPTR(QcepImagePlotSettings(name+"ImagePlotSettings"))),
  m_IntegratorPlotSettings(NEWPTR(QxrdIntegratorPlotSettings(name+"IntegratorPlotSettings"))),
  m_FileBrowserSettings(NEWPTR(QcepFileBrowserSettings(name+"FileBrowser"))),
  m_HistogramDialogSettings(NEWPTR(QxrdHistogramDialogSettings(name+"HistogramPlot")))
{
}

void QxrdWindowSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_ImagePlotWidgetSettings               -> initialize(sharedFromThis());
  m_CenteringPlotWidgetSettings           -> initialize(sharedFromThis());
  m_IntegratedPlotWidgetSettings          -> initialize(sharedFromThis());
  m_DistortionPlotWidgetSettings          -> initialize(sharedFromThis());
  m_ImagePlotSettings                     -> initialize(sharedFromThis());
  m_IntegratorPlotSettings                -> initialize(sharedFromThis());
  m_FileBrowserSettings                   -> initialize(sharedFromThis());
  m_HistogramDialogSettings               -> initialize(sharedFromThis());
}

QcepMainWindowPtr QxrdWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  m_Window =
      QxrdMainWindowPtr(
        NEWPTR(QxrdWindow("mainWindow")));

  return m_Window;
}

void QxrdWindowSettings::registerMetaTypes()
{
  qRegisterMetaType<QxrdWindowSettings*>("QxrdWindowSettings*");
  qRegisterMetaType<QcepImagePlotSettings*>("QcepImagePlotSettings*"); //TODO: move into Qcep-related class
  qRegisterMetaType<QxrdIntegratorPlotSettings*>("QxrdIntegratorPlotSettings*");
  qRegisterMetaType<QcepFileBrowserSettings*>("QcepFileBrowserSettings*");
  qRegisterMetaType<QcepImagePlotWidgetSettings*>("QcepImagePlotWidgetSettings*");
  qRegisterMetaType<QxrdCenteringPlotWidgetSettings*>("QxrdCenteringPlotWidgetSettings*");
  qRegisterMetaType<QxrdIntegratedPlotWidgetSettings*>("QxrdIntegratedPlotWidgetSettings*");
  qRegisterMetaType<QxrdDistortionPlotWidgetSettings*>("QxrdDistortionPlotWidgetSettings*");

  QxrdHistogramDialogSettings::registerMetaTypes();
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

QcepImagePlotSettingsWPtr QxrdWindowSettings::imagePlotSettings()
{
  return m_ImagePlotSettings;
}

QxrdIntegratorPlotSettingsWPtr QxrdWindowSettings::integratorPlotSettings()
{
  return m_IntegratorPlotSettings;
}

QcepFileBrowserSettingsWPtr QxrdWindowSettings::fileBrowserSettings()
{
  return m_FileBrowserSettings;
}

QxrdHistogramDialogSettingsWPtr QxrdWindowSettings::histogramDialogSettings()
{
  return m_HistogramDialogSettings;
}

QcepImagePlotWidgetSettingsPtr QxrdWindowSettings::imagePlotWidgetSettings()
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
