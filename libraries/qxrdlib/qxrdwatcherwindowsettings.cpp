#include "qxrdwatcherwindowsettings.h"
#include "qxrdwatcherwindow.h"
#include "qxrdfilebrowsersettings.h"
#include "qcepimageplotwidgetsettings.h"
#include "qxrdcenteringplotwidgetsettings.h"
#include <QThread>

QxrdWatcherWindowSettings::QxrdWatcherWindowSettings(QString name)
  : inherited(name, "Watcher Window"),
    m_FileBrowserSettings(NEWPTR(QxrdFileBrowserSettings(name))),
    m_ImagePlotWidgetSettings(NEWPTR(QcepImagePlotWidgetSettings(name))),
    m_CenteringPlotWidgetSettings(NEWPTR(QxrdCenteringPlotWidgetSettings(name))),
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
        NEWPTR(QxrdWatcherWindow("Watcher")));

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

QcepImagePlotWidgetSettingsPtr QxrdWatcherWindowSettings::imagePlotWidgetSettings()
{
  return m_ImagePlotWidgetSettings;
}

QxrdCenteringPlotWidgetSettingsPtr QxrdWatcherWindowSettings::centeringPlotWidgetSettings()
{
  return m_CenteringPlotWidgetSettings;
}

