#include "qxrdintegrationwindow.h"

QxrdIntegrationWindow::QxrdIntegrationWindow(QString name, QxrdApplicationPtr app, QxrdExperimentPtr expt) :
  QxrdMainWindow(name, app, expt)
{
  setupUi(this);

  setupMenus();
}

QxrdIntegrationWindow::~QxrdIntegrationWindow()
{
}

void QxrdIntegrationWindow::changeEvent(QEvent *e)
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
