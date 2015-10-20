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
    {
      QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

      if (m_ScriptEngine) return m_ScriptEngine;
    }

    QThread::msleep(50);
  }

  return QxrdScriptEnginePtr();
}

void QxrdScriptEngineThread::run()
{
  if (g_Application && qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage("Starting Script Engine Thread");
  }

  QxrdScriptEnginePtr p(new QxrdScriptEngine(m_Application, m_Experiment));

  int rc = -1;

  if (p) {
    p -> initialize();

    m_Mutex.lock();
    m_ScriptEngine = p;
    m_Mutex.unlock();

    rc = exec();
  }

  if (g_Application && qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage(tr("Script Engine Thread Terminated with rc %1").arg(rc));
  }

  {
    QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    m_ScriptEngine = QxrdScriptEnginePtr();
  }
}
