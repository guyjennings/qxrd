/******************************************************************
*
*  $Id: qxrdscriptenginethread.cpp,v 1.2 2009/06/28 11:21:58 jennings Exp $
*
*******************************************************************/

#include "qxrdscriptenginethread.h"

#include "qxrdscriptengine.h"

QxrdScriptEngineThread::QxrdScriptEngineThread(QxrdApplication *app, QxrdWindow *win, QxrdAcquisition *acq)
  : QThread(NULL),
    m_ScriptEngine(NULL),
    m_Application(app),
    m_Window(win),
    m_Acquisition(acq),
    SOURCE_IDENT("$Id: qxrdscriptenginethread.cpp,v 1.2 2009/06/28 11:21:58 jennings Exp $")
{
  m_ScriptEngine = new QxrdScriptEngine(m_Application, m_Window, m_Acquisition);

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
*  Revision 1.2  2009/06/28 11:21:58  jennings
*  Implemented app scripting engine connections
*
*  Revision 1.1  2009/06/28 04:00:39  jennings
*  Partial implementation of separate thread for script engine
*
*
*******************************************************************/
