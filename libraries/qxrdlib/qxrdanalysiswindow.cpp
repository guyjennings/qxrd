#include "qxrdanalysiswindow.h"

QxrdAnalysisWindow::QxrdAnalysisWindow(QString name, QxrdApplicationWPtr app, QxrdExperimentWPtr expt) :
  QxrdMainWindow(name, app, expt)
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
