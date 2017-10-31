#include "qxrdintegrationwindow.h"

QxrdIntegrationWindow::QxrdIntegrationWindow(QWidget *parent) :
  QxrdMainWindow(parent)
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
