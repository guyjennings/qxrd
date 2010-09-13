/******************************************************************
*
*  $Id: qxrdscriptenginethread.h,v 1.2 2010/09/13 20:00:42 jennings Exp $
*
*******************************************************************/

#ifndef QXRDSCRIPTENGINETHREAD_H
#define QXRDSCRIPTENGINETHREAD_H

#include "qcepmacros.h"

#include <QThread>
#include <QSharedPointer>

#include "qxrdforwardtypes.h"
#include "qxrddataprocessor.h"

class QxrdScriptEngineThread : public QThread
{
  Q_OBJECT;

public:
  QxrdScriptEngineThread(QxrdApplicationPtr app, QxrdWindowPtr win, QxrdAcquisitionPtr acq, QxrdDataProcessorPtr proc);
  ~QxrdScriptEngineThread();

  void shutdown();
  QxrdScriptEnginePtr scriptEngine() const;

protected:
  void run();

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

private:
  QAtomicPointer<QxrdScriptEngine>  m_ScriptEngine;
  QxrdApplicationPtr   m_Application;
  QxrdWindowPtr        m_Window;
  QxrdAcquisitionPtr   m_Acquisition;
  QxrdDataProcessorPtr m_DataProcessor;

  HEADER_IDENT("$Id: qxrdscriptenginethread.h,v 1.2 2010/09/13 20:00:42 jennings Exp $");
};

#endif // QXRDSCRIPTENGINETHREAD_H

/******************************************************************
*
*  $Log: qxrdscriptenginethread.h,v $
*  Revision 1.2  2010/09/13 20:00:42  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:42  jennings
*  Moving files into source subdirectory
*
*  Revision 1.3.4.4  2010/04/27 01:31:26  jennings
*  Starting to get QSharedPointer working
*
*  Revision 1.3.4.3  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.3.4.2  2010/04/26 20:53:26  jennings
*  More attempts to get QSharedPointers to work...
*
*  Revision 1.3.4.1  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.3  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
*
*  Revision 1.2  2009/07/17 14:00:59  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.1  2009/06/28 04:00:39  jennings
*  Partial implementation of separate thread for script engine
*
*
*******************************************************************/

