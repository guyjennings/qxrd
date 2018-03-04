#include "qxrdcenteringwindowsettings.h"
#include "qxrdcenteringwindow.h"
#include "qxrdfilebrowsersettings.h"
#include "qxrdimageplotwidgetsettings.h"
#include "qxrdcenteringplotwidgetsettings.h"
#include "qxrdintegratedplotwidgetsettings.h"
#include <QThread>

QxrdCenteringWindowSettings::QxrdCenteringWindowSettings(QString name, QString desc)
  : inherited(name, desc),
    m_FileBrowserSettings(new QxrdFileBrowserSettings(name)),
    m_ImagePlotWidgetSettings(new QxrdImagePlotWidgetSettings(name)),
    m_CenteringPlotWidgetSettings(new QxrdCenteringPlotWidgetSettings(name)),
    m_IntegratedPlotWidgetSettings(new QxrdIntegratedPlotWidgetSettings(name))
{

}

void QxrdCenteringWindowSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_FileBrowserSettings          -> initialize(sharedFromThis());
  m_ImagePlotWidgetSettings      -> initialize(sharedFromThis());
  m_CenteringPlotWidgetSettings  -> initialize(sharedFromThis());
  m_IntegratedPlotWidgetSettings -> initialize(sharedFromThis());
}

QcepMainWindowPtr QxrdCenteringWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  m_Window =
      QxrdMainWindowPtr(
        new QxrdCenteringWindow("Centering"),
        &QObject::deleteLater);

  return m_Window;
}

void QxrdCenteringWindowSettings::readSettings(QSettings *set)
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

  set->beginGroup("integratedPlotWidget");
  m_IntegratedPlotWidgetSettings->readSettings(set);
  set->endGroup();
}

void QxrdCenteringWindowSettings::writeSettings(QSettings *set)
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

  set->beginGroup("integratedPlotWidget");
  m_IntegratedPlotWidgetSettings->writeSettings(set);
  set->endGroup();
}

QxrdFileBrowserSettingsPtr QxrdCenteringWindowSettings::fileBrowserSettings()
{
  return m_FileBrowserSettings;
}

QxrdImagePlotWidgetSettingsPtr QxrdCenteringWindowSettings::imagePlotWidgetSettings()
{
  return m_ImagePlotWidgetSettings;
}

QxrdCenteringPlotWidgetSettingsPtr QxrdCenteringWindowSettings::centeringPlotWidgetSettings()
{
  return m_CenteringPlotWidgetSettings;
}

QxrdIntegratedPlotWidgetSettingsPtr QxrdCenteringWindowSettings::integratedPlotWidgetSettings()
{
  return m_IntegratedPlotWidgetSettings;
}
