#include "qxrdcalculatorwindowsettings.h"
#include "qxrdcalculatorwindow.h"
#include <QThread>

QxrdCalculatorWindowSettings::QxrdCalculatorWindowSettings(QString name)
  : inherited(name, "Calculator Window")
{

}

void QxrdCalculatorWindowSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

QcepMainWindowPtr QxrdCalculatorWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  m_Window =
      QxrdMainWindowPtr(
        NEWPTR(QxrdCalculatorWindow("Calculator")));

  return m_Window;
}
