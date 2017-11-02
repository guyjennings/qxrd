#include "qxrdscriptingwindow.h"

QxrdScriptingWindow::QxrdScriptingWindow(QString name, QxrdApplicationWPtr app, QxrdExperimentWPtr expt) :
  QxrdMainWindow(name, app, expt)
{
  setupUi(this);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);
}

QxrdScriptingWindow::~QxrdScriptingWindow()
{
}

void QxrdScriptingWindow::changeEvent(QEvent *e)
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
