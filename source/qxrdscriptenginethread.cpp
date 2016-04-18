#include "qxrddebug.h"
#include "qxrdscriptenginethread.h"

#include "qxrdscriptengine.h"
#include "qxrdapplication.h"
#include "qcepmutexlocker.h"

#include <stdio.h>

QxrdScriptEngineThread::QxrdScriptEngineThread(QxrdApplicationWPtr app, QxrdExperimentWPtr exp)
  : QxrdThread(NULL),
    m_ScriptEngine(NULL),
    m_Application(app),
    m_Experiment(exp)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdScriptEngineThread::QxrdScriptEngineThread(%p)\n", this);
  }

  setObjectName("script");
}

QxrdScriptEngineThread::~QxrdScriptEngineThread()
{
  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdScriptEngineThread::~QxrdScriptEngineThread(%p)\n", this);
  }
}

void QxrdScriptEngineThread::shutdown()
{
  exit();

  wait(1000);
}

QxrdScriptEnginePtr QxrdScriptEngineThread::scriptEngine() const
{
  while (isRunning()) {
    if (m_ScriptEngine) return m_ScriptEngine;

    QThread::msleep(50);
  }

  return QxrdScriptEnginePtr();
}

void QxrdScriptEngineThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printf("Script Engine Thread Started\n");
  }

  {
    QxrdScriptEnginePtr engine = QxrdScriptEnginePtr(
          new QxrdScriptEngine(m_Application, m_Experiment),
          &QObject::deleteLater);

    if (engine) {
      engine -> initialize();
    }
    m_ScriptEngine = engine;
  }

  int rc = exec();

  m_ScriptEngine = QxrdScriptEnginePtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Script Engine Thread Terminated with rc %d\n", rc);
  }
}
