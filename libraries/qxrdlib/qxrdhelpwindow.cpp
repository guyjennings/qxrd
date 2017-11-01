#include "qxrdhelpwindow.h"
#include "ui_qxrdhelpwindow.h"

QxrdHelpWindow::QxrdHelpWindow(QString name, QxrdApplicationPtr app, QxrdExperimentPtr expt) :
  QxrdMainWindow(name, app, expt)
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
