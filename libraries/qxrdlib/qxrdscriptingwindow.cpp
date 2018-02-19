#include "qxrdscriptingwindow.h"
#include "qxrdexperiment.h"

QxrdScriptingWindow::QxrdScriptingWindow(QxrdScriptingWindowSettingsWPtr set, QString name, QxrdAppCommonWPtr app, QxrdExperimentWPtr expt, QxrdAcqCommonWPtr acqw, QxrdProcessorWPtr procw) :
  QxrdMainWindow(name, app, expt, acqw, procw),
  m_ScriptingWindowSettings(set),
  m_CommandIndex(0)
{
  setupUi(this);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);

  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    m_ScriptEngine = exp->scriptEngine();
  }

  QxrdScriptEnginePtr eng(m_ScriptEngine);

  if (eng) {
    m_DataIndex = eng -> nextDataIndex();

    connect(this,          &QxrdScriptingWindow::evaluateCommand,
            eng.data(),    &QxrdScriptEngine::evaluateCommand);

    connect(eng.data(),    &QxrdScriptEngine::resultAvailable,
            this,          &QxrdScriptingWindow::resultAvailable);

    connect(m_EvalButton,  &QAbstractButton::clicked,
            this,          &QxrdScriptingWindow::doEvaluate);
  }
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

void QxrdScriptingWindow::doEvaluate()
{
  QString cmd = m_InputText->text();

  m_Output -> append("<span style=\"color:blue\">" +
                     tr("[ %1 ] -> ").arg(m_CommandIndex++) +
                     "</span>" +
                     cmd);

  emit evaluateCommand(m_DataIndex, cmd);
}

void QxrdScriptingWindow::resultAvailable(int index, QScriptValue res)
{
  if (index == m_DataIndex) {
    m_Output -> append(tr("  =  %1").arg(res.toString()));
  }
}
