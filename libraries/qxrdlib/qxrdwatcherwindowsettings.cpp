#include "qxrdwatcherwindowsettings.h"
#include "qxrdwatcherwindow.h"
#include "qxrdfilebrowsersettings.h"
#include "qxrdimageplotwidgetsettings.h"
#include "qxrdcenteringplotwidgetsettings.h"
#include <QThread>

QxrdWatcherWindowSettings::QxrdWatcherWindowSettings(QString name, QString desc)
  : inherited(name, desc),
    m_FileBrowserSettings(new QxrdFileBrowserSettings(name)),
    m_ImagePlotWidgetSettings(new QxrdImagePlotWidgetSettings(name)),
    m_CenteringPlotWidgetSettings(new QxrdCenteringPlotWidgetSettings(name)),
    m_Pattern(this, "pattern", "", "Watched Pattern")
{

}

void QxrdWatcherWindowSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_FileBrowserSettings         -> initialize(sharedFromThis());
  m_ImagePlotWidgetSettings     -> initialize(sharedFromThis());
  m_CenteringPlotWidgetSettings -> initialize(sharedFromThis());
}

QcepMainWindowPtr QxrdWatcherWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  m_Window =
      QxrdMainWindowPtr(
        new QxrdWatcherWindow("Watcher"),
        &QObject::deleteLater);

  return m_Window;
}

void QxrdWatcherWindowSettings::readSettings(QSettings *set)
{
  QxrdMainWindowSettings::readSettings(set);

  set->beginGroup("browser");
  m_FileBrowserSettings->readSettings(set);
  set->endGroup();

  set->beginGroup("imagePlotWidget");
  m_ImagePlotWidgetSettings->readSettings(set);
  set->endGroup();

  set->beginGroup("centeringsPlotWidget");
  m_CenteringPlotWidgetSettings->readSettings(set);
  set->endGroup();

//  set->beginGroup("integratedPlotWidget");
//  m_IntegratedPlotWidgetSettings->readSettings(set);
//  set->endGroup();
}

void QxrdWatcherWindowSettings::writeSettings(QSettings *set)
{
  QxrdMainWindowSettings::writeSettings(set);

  set->beginGroup("browser");
  m_FileBrowserSettings->writeSettings(set);
  set->endGroup();

  set->beginGroup("imagePlotWidget");
  m_ImagePlotWidgetSettings->writeSettings(set);
  set->endGroup();

  set->beginGroup("centeringsPlotWidget");
  m_CenteringPlotWidgetSettings->writeSettings(set);
  set->endGroup();

//  set->beginGroup("integratedPlotWidget");
//  m_IntegratedPlotWidgetSettings->writeSettings(set);
//  set->endGroup();
}

QxrdFileBrowserSettingsPtr QxrdWatcherWindowSettings::fileBrowserSettings()
{
  return m_FileBrowserSettings;
}

QxrdImagePlotWidgetSettingsPtr QxrdWatcherWindowSettings::imagePlotWidgetSettings()
{
  return m_ImagePlotWidgetSettings;
}

QxrdCenteringPlotWidgetSettingsPtr QxrdWatcherWindowSettings::centeringPlotWidgetSettings()
{
  return m_CenteringPlotWidgetSettings;
}

