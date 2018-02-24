#include "qxrdextraiowindowsettings.h"
#include "qxrdextraiowindow.h"
#include "qxrdextrainputsplotwidgetsettings.h"
#include "qxrdextraoutputsplotwidgetsettings.h"
#include <QThread>

QxrdExtraIOWindowSettings::QxrdExtraIOWindowSettings(QString name, QString desc)
  : inherited(name, desc),
    m_ExtraInputsPlotWidgetSettings(new QxrdExtraInputsPlotWidgetSettings(name)),
    m_ExtraOutputsPlotWidgetSettings(new QxrdExtraOutputsPlotWidgetSettings(name))
{

}

void QxrdExtraIOWindowSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_ExtraInputsPlotWidgetSettings  -> initialize(sharedFromThis());
  m_ExtraOutputsPlotWidgetSettings -> initialize(sharedFromThis());
}

QcepMainWindowPtr QxrdExtraIOWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  m_Window =
      QxrdMainWindowPtr(
        new QxrdExtraIOWindow("Extra I/O"));

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
