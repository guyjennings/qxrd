#include "qswscriptenginethread.h"
#include "qswscriptengine.h"

QswScriptEngineThread::QswScriptEngineThread(QString name)
  : inherited(name)
{

}

QswScriptEngineThread::~QswScriptEngineThread()
{
}

void QswScriptEngineThread::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

void QswScriptEngineThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printMessage("Script Engine Thread Started");
  }

  m_ScriptEngine =
      QswScriptEnginePtr(
        new QswScriptEngine("scriptEngine"));

  m_ScriptEngine -> initialize(sharedFromThis());

  int rc = exec();

  m_ScriptEngine = QswScriptEnginePtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printMessage(tr("Script Engine Thread terminated with rc %1").arg(rc));
  }
}

QswScriptEngineWPtr QswScriptEngineThread::scriptEngine()
{
  while (isRunning()) {
    if (m_ScriptEngine) return m_ScriptEngine;

    msleep(50);
  }

  return m_ScriptEngine;
}
