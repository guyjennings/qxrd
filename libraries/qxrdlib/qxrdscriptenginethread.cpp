#include "qxrddebug.h"
#include "qxrdscriptenginethread.h"

#include "qxrdscriptengine.h"
#include "qxrdappcommon.h"
#include "qcepmutexlocker.h"
#include "qxrdexperiment.h"

#include <stdio.h>

QxrdScriptEngineThread::QxrdScriptEngineThread(QString name)
  : inherited(name),
    m_ScriptEngine(NULL),
    m_Application(),
    m_Experiment()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdScriptEngineThread::QxrdScriptEngineThread(%p)\n", this);
  }
}

void QxrdScriptEngineThread::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_Application = QxrdAppCommon::findApplication(parent);
  m_Experiment  = QxrdExperiment::findExperiment(parent);
}

QxrdScriptEngineThread::~QxrdScriptEngineThread()
{
  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdScriptEngineThread::~QxrdScriptEngineThread(%p)\n", this);
  }
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
          NEWPTR(QxrdScriptEngine("scriptEngine")));

    if (engine) {
      engine -> initialize(sharedFromThis());
    }
    m_ScriptEngine = engine;
  }

  int rc = 0;

  if (m_ScriptEngine) {
    rc = exec();
  }

  m_ScriptEngine = QxrdScriptEnginePtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Script Engine Thread Terminated with rc %d\n", rc);
  }
}
