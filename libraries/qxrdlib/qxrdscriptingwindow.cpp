#include "qxrdscriptingwindow.h"

QxrdScriptingWindow::QxrdScriptingWindow(QxrdScriptingWindowSettingsWPtr set, QString name, QxrdAppCommonWPtr app, QxrdExperimentWPtr expt, QxrdAcqCommonWPtr acqw, QxrdProcessorWPtr procw) :
  QxrdMainWindow(name, app, expt, acqw, procw),
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
