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

QxrdMainWindowPtr QxrdCalculatorWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  QxrdCalculatorWindowSettingsPtr myself = qSharedPointerDynamicCast<QxrdCalculatorWindowSettings>(sharedFromThis());

  m_Window =
      QxrdMainWindowPtr(
        new QxrdCalculatorWindow(myself, "Calculator", m_Application, m_Experiment, m_Acquisition, m_Processor));

  return m_Window;
}
