#include "qxrdcalculatorwindowsettings.h"
#include "qxrdcalculatorwindow.h"
#include <QThread>

QxrdCalculatorWindowSettings::QxrdCalculatorWindowSettings(QString name, QString desc)
  : inherited(name, desc)
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
        new QxrdCalculatorWindow("Calculator"),
        &QObject::deleteLater);

  return m_Window;
}
