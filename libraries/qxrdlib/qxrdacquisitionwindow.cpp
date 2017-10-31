#include "qxrdacquisitionwindow.h"

QxrdAcquisitionWindow::QxrdAcquisitionWindow(QWidget *parent) :
  QxrdMainWindow(parent)
{
  setupUi(this);

  setupMenus();
}

QxrdAcquisitionWindow::~QxrdAcquisitionWindow()
{
}

void QxrdAcquisitionWindow::changeEvent(QEvent *e)
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
