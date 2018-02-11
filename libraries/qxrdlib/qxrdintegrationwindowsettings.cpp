#include "qxrdintegrationwindowsettings.h"
#include "qxrdintegrationwindow.h"
#include "qxrdfilebrowsersettings.h"
#include "qxrdimageplotwidgetsettings.h"
#include "qxrdintegratedplotwidgetsettings.h"
#include <QThread>

QxrdIntegrationWindowSettings::QxrdIntegrationWindowSettings(QString name, QString desc)
  : QxrdMainWindowSettings(name, desc),
    m_FileBrowserSettings(new QxrdFileBrowserSettings(name)),
    m_ImagePlotWidgetSettings(new QxrdImagePlotWidgetSettings(name)),
    m_IntegratedPlotWidgetSettings(new QxrdIntegratedPlotWidgetSettings(name))
{

}

QxrdMainWindowPtr QxrdIntegrationWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  QxrdIntegrationWindowSettingsPtr myself = qSharedPointerDynamicCast<QxrdIntegrationWindowSettings>(sharedFromThis());

  m_Window =
      QxrdMainWindowPtr(
        new QxrdIntegrationWindow(myself, "Integration", m_Application, m_Experiment, m_Acquisition, m_Processor));

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
