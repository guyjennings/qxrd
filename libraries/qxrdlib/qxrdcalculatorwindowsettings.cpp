#include "qxrdcalculatorwindowsettings.h"
#include "qxrdcalculatorwindow.h"

QxrdCalculatorWindowSettings::QxrdCalculatorWindowSettings(QString name)
  : QxrdMainWindowSettings(name)
{

}

QxrdMainWindowPtr QxrdCalculatorWindowSettings::newWindow()
{
  m_Window =
      QxrdMainWindowPtr(
        new QxrdCalculatorWindow("Calculator", m_Application, m_Experiment));

  return m_Window;
}
