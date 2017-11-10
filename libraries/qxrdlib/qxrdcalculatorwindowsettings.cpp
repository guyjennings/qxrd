#include "qxrdcalculatorwindowsettings.h"
#include "qxrdcalculatorwindow.h"

QxrdCalculatorWindowSettings::QxrdCalculatorWindowSettings(QString name)
  : QxrdMainWindowSettings(name)
{

}

QxrdMainWindowPtr QxrdCalculatorWindowSettings::newWindow()
{
  QxrdCalculatorWindowSettingsPtr myself = qSharedPointerDynamicCast<QxrdCalculatorWindowSettings>(sharedFromThis());

  m_Window =
      QxrdMainWindowPtr(
        new QxrdCalculatorWindow(myself, "Calculator", m_Application, m_Experiment));

  return m_Window;
}
