#include "qxrdhelpwindow.h"
#include "ui_qxrdhelpwindow.h"

QxrdHelpWindow::QxrdHelpWindow(QWidget *parent) :
  QxrdMainWindow(parent)
{
  setupUi(this);

  setupMenus();
}

QxrdHelpWindow::~QxrdHelpWindow()
{
}

void QxrdHelpWindow::changeEvent(QEvent *e)
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
