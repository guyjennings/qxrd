#include "qxrdscriptingwindow.h"
#include "qxrdexperiment.h"
#include <QThread>

QxrdScriptingWindow::QxrdScriptingWindow(QString name) :
  inherited(name),
  m_CommandIndex(0)
{
}

void QxrdScriptingWindow::initialize(QcepObjectWPtr parent)
{
  GUI_THREAD_CHECK;

  setupUi(this);

  inherited::initialize(parent);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);

  QxrdExperimentPtr exp(QxrdExperiment::findExperiment(m_Parent));

  if (exp) {
    m_ScriptEngine = exp->scriptEngine();
  }

  QxrdScriptEnginePtr eng(m_ScriptEngine);

  if (eng) {
    m_DataIndex = eng -> nextDataIndex();

    CONNECT_CHECK(
          connect(this,          &QxrdScriptingWindow::evaluateCommand,
                  eng.data(),    &QxrdScriptEngine::evaluateCommand));

    CONNECT_CHECK(
          connect(eng.data(),    &QxrdScriptEngine::resultAvailable,
                  this,          &QxrdScriptingWindow::resultAvailable));

    CONNECT_CHECK(
          connect(m_EvalButton,  &QAbstractButton::clicked,
                  this,          &QxrdScriptingWindow::doEvaluate));
  }
}

QxrdScriptingWindow::~QxrdScriptingWindow()
{
}

void QxrdScriptingWindow::changeEvent(QEvent *e)
{
  inherited::changeEvent(e);
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
