#include "qxrdscriptenginethread.h"

#include "qxrdscriptengine.h"

QxrdScriptEngineThread::QxrdScriptEngineThread(QxrdApplication *app, QxrdWindow *win, QxrdAcquisition *acq, QxrdDataProcessor *proc)
  : QThread(NULL),
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
    QThread::msleep(500);
  }

  return m_ScriptEngine;
}

void QxrdScriptEngineThread::run()
{
  QxrdScriptEngine *p = new QxrdScriptEngine(m_Application, m_Window, m_Acquisition, m_DataProcessor);

  p -> initialize();

  m_ScriptEngine.fetchAndStoreOrdered(p);

  exec();

//  emit printMessage("Scripting thread terminated");
}
