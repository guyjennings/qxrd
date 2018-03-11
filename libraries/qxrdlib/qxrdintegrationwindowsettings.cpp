#include "qxrdintegrationwindowsettings.h"
#include "qxrdintegrationwindow.h"
#include "qxrdfilebrowsersettings.h"
#include "qxrdimageplotwidgetsettings.h"
#include "qxrdintegratedplotwidgetsettings.h"
#include <QThread>

QxrdIntegrationWindowSettings::QxrdIntegrationWindowSettings(QString name)
  : inherited(name, "Integration Window"),
    m_FileBrowserSettings(new QxrdFileBrowserSettings(name+"Browser")),
    m_ImagePlotWidgetSettings(new QxrdImagePlotWidgetSettings(name+"ImagePlot")),
    m_IntegratedPlotWidgetSettings(new QxrdIntegratedPlotWidgetSettings(name+"IntegratedPlot"))
{

}

void QxrdIntegrationWindowSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_FileBrowserSettings          -> initialize(sharedFromThis());
  m_ImagePlotWidgetSettings      -> initialize(sharedFromThis());
  m_IntegratedPlotWidgetSettings -> initialize(sharedFromThis());
}

QcepMainWindowPtr QxrdIntegrationWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  m_Window =
      QxrdMainWindowPtr(
        new QxrdIntegrationWindow("Integration"),
        &QcepObject::deleteLater);

  return m_Window;
}

void QxrdIntegrationWindowSettings::readSettings(QSettings *set)
{
  QxrdMainWindowSettings::readSettings(set);

  set->beginGroup("browser");
  m_FileBrowserSettings->readSettings(set);
  set->endGroup();

  set->beginGroup("imagePlot");
  m_ImagePlotWidgetSettings->readSettings(set);
  set->endGroup();

  set->beginGroup("integratedPlot");
  m_IntegratedPlotWidgetSettings->readSettings(set);
  set->endGroup();
}

void QxrdIntegrationWindowSettings::writeSettings(QSettings *set)
{
  QxrdMainWindowSettings::writeSettings(set);

  set->beginGroup("browser");
  m_FileBrowserSettings->writeSettings(set);
  set->endGroup();

  set->beginGroup("imagePlot");
  m_ImagePlotWidgetSettings->writeSettings(set);
  set->endGroup();

  set->beginGroup("integratedPlot");
  m_IntegratedPlotWidgetSettings->writeSettings(set);
  set->endGroup();
}

QxrdFileBrowserSettingsPtr QxrdIntegrationWindowSettings::fileBrowserSettings()
{
  return m_FileBrowserSettings;
}

QxrdImagePlotWidgetSettingsPtr QxrdIntegrationWindowSettings::imagePlotWidgetSettings()
{
  return m_ImagePlotWidgetSettings;
}

QxrdIntegratedPlotWidgetSettingsPtr QxrdIntegrationWindowSettings::integratedPlotWidgetSettings()
{
  return m_IntegratedPlotWidgetSettings;
}
