/******************************************************************
*
*  $Id: qxrdscriptenginethread.cpp,v 1.3 2009/07/17 14:00:59 jennings Exp $
*
*******************************************************************/

#include "qxrdscriptenginethread.h"

#include "qxrdscriptengine.h"

QxrdScriptEngineThread::QxrdScriptEngineThread(QxrdApplication *app, QxrdWindow *win, QxrdAcquisition *acq, QxrdDataProcessor *proc)
  : QThread(NULL),
    m_ScriptEngine(NULL),
    m_Application(app),
    m_Window(win),
    m_Acquisition(acq),
    m_DataProcessor(proc),
    SOURCE_IDENT("$Id: qxrdscriptenginethread.cpp,v 1.3 2009/07/17 14:00:59 jennings Exp $")
{
  m_ScriptEngine = new QxrdScriptEngine(m_Application, m_Window, m_Acquisition, m_DataProcessor);

  m_ScriptEngine -> moveToThread(this);
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

QxrdScriptEngine  *QxrdScriptEngineThread::scriptEngine() const
{
  return m_ScriptEngine;
}

void QxrdScriptEngineThread::run()
{
  m_ScriptEngine -> initialize();

  exec();
}

/******************************************************************
*
*  $Log: qxrdscriptenginethread.cpp,v $
*  Revision 1.3  2009/07/17 14:00:59  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.2  2009/06/28 11:21:58  jennings
*  Implemented app scripting engine connections
*
*  Revision 1.1  2009/06/28 04:00:39  jennings
*  Partial implementation of separate thread for script engine
*
*
*******************************************************************/
