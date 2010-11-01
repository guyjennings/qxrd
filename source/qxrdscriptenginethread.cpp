#include "qxrdscriptenginethread.h"

#include "qxrdscriptengine.h"

QxrdScriptEngineThread::QxrdScriptEngineThread(QxrdApplicationPtr app, QxrdWindowPtr win, QxrdAcquisitionPtr acq, QxrdDataProcessorPtr proc)
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

QxrdScriptEnginePtr QxrdScriptEngineThread::scriptEngine() const
{
  while (m_ScriptEngine == NULL) {
    QThread::msleep(500);
  }

  return m_ScriptEngine;
}

void QxrdScriptEngineThread::run()
{
  QxrdScriptEnginePtr p = QxrdScriptEnginePtr(new QxrdScriptEngine(m_Application, m_Window, m_Acquisition, m_DataProcessor));

  p -> initialize();

  m_ScriptEngine.fetchAndStoreOrdered(p);

  exec();

  emit printMessage(QDateTime::currentDateTime(), "Scripting thread terminated");
}
