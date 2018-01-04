#include "qxrdcalculatorwindow.h"

QxrdCalculatorWindow::QxrdCalculatorWindow(QxrdCalculatorWindowSettingsWPtr set, QString name, QxrdApplicationWPtr app, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acqw, QxrdProcessorWPtr procw) :
  QxrdMainWindow(name, app, expt, acqw, procw),
  m_CalculatorWindowSettings(set)
{
  setupUi(this);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);
}

QxrdCalculatorWindow::~QxrdCalculatorWindow()
{
}

void QxrdCalculatorWindow::changeEvent(QEvent *e)
{
  QMainWindow::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}
