#include "qxrdcalibrantwindow.h"

QxrdCalibrantWindow::QxrdCalibrantWindow(QWidget *parent) :
  QxrdMainWindow(parent)
{
  setupUi(this);

  setupMenus();
}

QxrdCalibrantWindow::~QxrdCalibrantWindow()
{
}

void QxrdCalibrantWindow::changeEvent(QEvent *e)
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
