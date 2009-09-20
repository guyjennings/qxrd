/******************************************************************
*
*  $Id: qxrdscriptenginethread.h,v 1.3 2009/09/20 21:18:53 jennings Exp $
*
*******************************************************************/

#ifndef QXRDSCRIPTENGINETHREAD_H
#define QXRDSCRIPTENGINETHREAD_H

#include "qcepmacros.h"

#include <QThread>

class QxrdApplication;
class QxrdWindow;
class QxrdAcquisition;
class QxrdScriptEngine;
class QxrdDataProcessor;

class QxrdScriptEngineThread : public QThread
{
  Q_OBJECT;

public:
  QxrdScriptEngineThread(QxrdApplication *app, QxrdWindow *win, QxrdAcquisition *acq, QxrdDataProcessor *proc);
  ~QxrdScriptEngineThread();

  void shutdown();
  QxrdScriptEngine  *scriptEngine() const;

protected:
  void run();

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

private:
  QxrdScriptEngine    *m_ScriptEngine;
  QxrdApplication     *m_Application;
  QxrdWindow          *m_Window;
  QxrdAcquisition     *m_Acquisition;
  QxrdDataProcessor   *m_DataProcessor;

  HEADER_IDENT("$Id: qxrdscriptenginethread.h,v 1.3 2009/09/20 21:18:53 jennings Exp $");
};

#endif // QXRDSCRIPTENGINETHREAD_H

/******************************************************************
*
*  $Log: qxrdscriptenginethread.h,v $
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

