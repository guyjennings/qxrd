#include "qxrdextraiowindowsettings.h"
#include "qxrdextraiowindow.h"
#include "qxrdextrainputsplotwidgetsettings.h"
#include "qxrdextraoutputsplotwidgetsettings.h"

QxrdExtraIOWindowSettings::QxrdExtraIOWindowSettings(QString name)
  : QxrdMainWindowSettings(name),
    m_ExtraInputsPlotWidgetSettings(new QxrdExtraInputsPlotWidgetSettings(name)),
    m_ExtraOutputsPlotWidgetSettings(new QxrdExtraOutputsPlotWidgetSettings(name))
{

}

QxrdMainWindowPtr QxrdExtraIOWindowSettings::newWindow()
{
  QxrdExtraIOWindowSettingsPtr myself = qSharedPointerDynamicCast<QxrdExtraIOWindowSettings>(sharedFromThis());

  m_Window =
      QxrdMainWindowPtr(
        new QxrdExtraIOWindow(myself, "Extra I/O", m_Application, m_Experiment));

  return m_Window;
}

QxrdExtraInputsPlotWidgetSettingsPtr QxrdExtraIOWindowSettings::extraInputsPlotWidgetSettings()
{
  return m_ExtraInputsPlotWidgetSettings;
}

QxrdExtraOutputsPlotWidgetSettingsPtr QxrdExtraIOWindowSettings::extraOutputsPlotWidgetSettings()
{
  return m_ExtraOutputsPlotWidgetSettings;
}

void QxrdExtraIOWindowSettings::readSettings(QSettings *set)
{
  QxrdMainWindowSettings::readSettings(set);

  set->beginGroup("extraInputs");
  m_ExtraInputsPlotWidgetSettings->readSettings(set);
  set->endGroup();

  set->beginGroup("extraOutputs");
  m_ExtraOutputsPlotWidgetSettings->readSettings(set);
  set->endGroup();
}

void QxrdExtraIOWindowSettings::writeSettings(QSettings *set)
{
  QxrdMainWindowSettings::writeSettings(set);

  set->beginGroup("extraInputs");
  m_ExtraInputsPlotWidgetSettings->writeSettings(set);
  set->endGroup();

  set->beginGroup("extraOutputs");
  m_ExtraOutputsPlotWidgetSettings->writeSettings(set);
  set->endGroup();
}
