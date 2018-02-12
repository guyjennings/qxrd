#include "qxrddebug.h"
#include "qxrdscriptenginethread.h"

#include "qxrdscriptengine.h"
#include "qxrdapplication.h"
#include "qcepmutexlocker.h"
#include "qxrdexperiment.h"

#include <stdio.h>

QxrdScriptEngineThread::QxrdScriptEngineThread(QxrdApplicationWPtr app, QxrdExperimentWPtr exp)
  : QxrdThread(exp),
    m_ScriptEngine(NULL),
    m_Application(app),
    m_Experiment(exp)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdScriptEngineThread::QxrdScriptEngineThread(%p)\n", this);
  }

  setObjectName("scriptEngine");
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

  wait();
}

QxrdScriptEnginePtr QxrdScriptEngineThread::scriptEngine() const
{
  while (isRunning()) {
    if (m_ScriptEngine) return m_ScriptEngine;

    msleep(50);
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
          new QxrdScriptEngine(m_Application, m_Experiment));

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
