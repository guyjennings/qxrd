#include "qxrdmaskingwindow.h"

QxrdMaskingWindow::QxrdMaskingWindow(QWidget *parent) :
  QxrdMainWindow(parent)
{
  setupUi(this);

  setupMenus();
}

QxrdMaskingWindow::~QxrdMaskingWindow()
{
}

void QxrdMaskingWindow::changeEvent(QEvent *e)
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
