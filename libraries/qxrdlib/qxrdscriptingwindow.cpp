#include "qxrdscriptingwindow.h"

QxrdScriptingWindow::QxrdScriptingWindow(QxrdScriptingWindowSettingsWPtr set, QString name, QxrdApplicationWPtr app, QxrdExperimentWPtr expt) :
  QxrdMainWindow(name, app, expt),
  m_ScriptingWindowSettings(set)
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
