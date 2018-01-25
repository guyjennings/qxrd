#include "qxrdwatcherwindowsettings.h"
#include "qxrdwatcherwindow.h"
#include "qxrdfilebrowsersettings.h"
#include "qxrdimageplotwidgetsettings.h"
#include "qxrdcenteringplotwidgetsettings.h"

QxrdWatcherWindowSettings::QxrdWatcherWindowSettings(QString name)
  : QxrdMainWindowSettings(name),
    m_FileBrowserSettings(new QxrdFileBrowserSettings(name)),
    m_ImagePlotWidgetSettings(new QxrdImagePlotWidgetSettings(name)),
    m_CenteringPlotWidgetSettings(new QxrdCenteringPlotWidgetSettings(name))
{

}

void QxrdWatcherWindowSettings::initialize(QxrdApplicationWPtr app,
                                           QxrdExperimentWPtr  expt,
                                           QxrdAcquisitionWPtr acqw,
                                           QxrdProcessorWPtr   procw)
{
  QxrdMainWindowSettings::initialize(app,
                                     expt,
                                     acqw,
                                     procw);

//  m_Watcher = watcher;
}

QxrdMainWindowPtr QxrdWatcherWindowSettings::newWindow()
{
  QxrdWatcherWindowSettingsPtr myself =
      qSharedPointerDynamicCast<QxrdWatcherWindowSettings>(sharedFromThis());

  m_Window =
      QxrdMainWindowPtr(
        new QxrdWatcherWindow(myself,
                              "Watcher",
                              m_Application,
                              m_Experiment,
                              m_Acquisition,
                              m_Processor,
                              m_Watcher));

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

