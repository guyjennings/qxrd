#include "qxrdcenteringwindow.h"

QxrdCenteringWindow::QxrdCenteringWindow(QString name, QxrdApplicationPtr app, QxrdExperimentPtr expt) :
  QxrdMainWindow(name, app, expt)
{
  setupUi(this);

  setupMenus();
}

QxrdCenteringWindow::~QxrdCenteringWindow()
{
}

void QxrdCenteringWindow::changeEvent(QEvent *e)
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
