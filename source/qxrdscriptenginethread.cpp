#include "qxrdscriptenginethread.h"

#include "qxrdscriptengine.h"
#include "qxrdapplication.h"

QxrdScriptEngineThread::QxrdScriptEngineThread(QxrdApplication *app, QxrdWindow *win, QxrdAcquisition *acq, QxrdDataProcessor *proc)
  : QxrdThread(NULL),
    m_ScriptEngine(NULL),
    m_Application(app),
    m_Window(win),
    m_Acquisition(acq),
    m_DataProcessor(proc)
{
}

QxrdScriptEngineThread::~QxrdScriptEngineThread()
{
  shutdown();

  delete m_ScriptEngine;
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

  QxrdScriptEngine *p = new QxrdScriptEngine(m_Application, m_Window, m_Acquisition, m_DataProcessor);

  p -> initialize();

  m_ScriptEngine.fetchAndStoreOrdered(p);

  int rc = exec();

  if (rc || qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage(tr("Script Engine Thread Terminated with rc %1").arg(rc));
  }
}
