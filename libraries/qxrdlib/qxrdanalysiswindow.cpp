#include "qxrdanalysiswindow.h"

QxrdAnalysisWindow::QxrdAnalysisWindow(QWidget *parent) :
  QxrdMainWindow(parent)
{
  setupUi(this);

  setupMenus();
}

QxrdAnalysisWindow::~QxrdAnalysisWindow()
{
}

void QxrdAnalysisWindow::changeEvent(QEvent *e)
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
