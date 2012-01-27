#include "qxrdscriptenginethread.h"

#include "qxrdscriptengine.h"
#include "qxrdapplication.h"

QxrdScriptEngineThread::QxrdScriptEngineThread(QxrdApplication *app, QxrdExperiment *exp)
  : QxrdThread(NULL),
    m_ScriptEngine(NULL),
    m_Application(app),
    m_Experiment(exp)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdScriptEngineThread::QxrdScriptEngineThread\n");
  }
}

QxrdScriptEngineThread::~QxrdScriptEngineThread()
{
  shutdown();

  delete m_ScriptEngine;

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdScriptEngineThread::~QxrdScriptEngineThread\n");
  }
}

void QxrdScriptEngineThread::shutdown()
{
  exit();

  wait(1000);
}

QxrdScriptEngine *QxrdScriptEngineThread::scriptEngine() const
{
  while (m_ScriptEngine == NULL) {
    QThread::msleep(50);
  }

  return m_ScriptEngine;
}

void QxrdScriptEngineThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage("Starting Script Engine Thread");
  }

  QxrdScriptEngine *p = new QxrdScriptEngine(m_Application, m_Experiment);

  p -> initialize();

  m_ScriptEngine.fetchAndStoreOrdered(p);

  int rc = exec();

  if (qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage(tr("Script Engine Thread Terminated with rc %1").arg(rc));
  }
}
