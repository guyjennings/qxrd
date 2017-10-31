#include "qxrdcalculatorwindow.h"

QxrdCalculatorWindow::QxrdCalculatorWindow(QWidget *parent) :
  QxrdMainWindow(parent)
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
