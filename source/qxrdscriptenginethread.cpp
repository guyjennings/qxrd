/******************************************************************
*
*  $Id: qxrdscriptenginethread.cpp,v 1.2 2010/09/13 20:00:42 jennings Exp $
*
*******************************************************************/

#include "qxrdscriptenginethread.h"

#include "qxrdscriptengine.h"

QxrdScriptEngineThread::QxrdScriptEngineThread(QxrdApplicationPtr app, QxrdWindowPtr win, QxrdAcquisitionPtr acq, QxrdDataProcessorPtr proc)
  : QThread(NULL),
    m_ScriptEngine(NULL),
    m_Application(app),
    m_Window(win),
    m_Acquisition(acq),
    m_DataProcessor(proc),
    SOURCE_IDENT("$Id: qxrdscriptenginethread.cpp,v 1.2 2010/09/13 20:00:42 jennings Exp $")
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

  emit printMessage("Scripting thread terminated");
}

/******************************************************************
*
*  $Log: qxrdscriptenginethread.cpp,v $
*  Revision 1.2  2010/09/13 20:00:42  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:41  jennings
*  Moving files into source subdirectory
*
*  Revision 1.3.4.6  2010/06/09 19:44:21  jennings
*  Print out termination messages when threads quit
*
*  Revision 1.3.4.5  2010/04/27 19:37:51  jennings
*  Explicitly delete some data no longer protected by QSharedPointer s
*
*  Revision 1.3.4.4  2010/04/27 01:31:26  jennings
*  Starting to get QSharedPointer working
*
*  Revision 1.3.4.3  2010/04/26 02:43:31  jennings
*  Called msleep(500) rather than sleep(0.5)
*
*  Revision 1.3.4.2  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.3.4.1  2010/04/21 20:50:41  jennings
*  Modified thread initialization so that objects are created from run method to get correct thread affinity.
*
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
