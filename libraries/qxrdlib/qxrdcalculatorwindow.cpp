#include "qxrdcalculatorwindow.h"

QxrdCalculatorWindow::QxrdCalculatorWindow(QString name, QxrdApplicationPtr app, QxrdExperimentPtr expt) :
  QxrdMainWindow(name, app, expt)
{
  setupUi(this);

  setupMenus();
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
