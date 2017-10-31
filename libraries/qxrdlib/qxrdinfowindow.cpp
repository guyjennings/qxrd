#include "qxrdinfowindow.h"

QxrdInfoWindow::QxrdInfoWindow(QWidget *parent) :
  QxrdMainWindow(parent)
{
  setupUi(this);

  setupMenus();
}

QxrdInfoWindow::~QxrdInfoWindow()
{
}

void QxrdInfoWindow::changeEvent(QEvent *e)
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
