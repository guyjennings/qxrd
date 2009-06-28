/******************************************************************
*
*  $Id: qxrdscriptenginethread.h,v 1.1 2009/06/28 04:00:39 jennings Exp $
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

class QxrdScriptEngineThread : public QThread
{
  Q_OBJECT;

public:
  QxrdScriptEngineThread(QxrdApplication *app, QxrdWindow *win, QxrdAcquisition *acq);
  ~QxrdScriptEngineThread();

  void shutdown();
  QxrdScriptEngine  *scriptEngine() const;

protected:
  void run();

signals:
  void printMessage(QString msg);

private:
  QxrdScriptEngine    *m_ScriptEngine;
  QxrdApplication     *m_Application;
  QxrdWindow          *m_Window;
  QxrdAcquisition     *m_Acquisition;

  HEADER_IDENT("$Id: qxrdscriptenginethread.h,v 1.1 2009/06/28 04:00:39 jennings Exp $");
};

#endif // QXRDSCRIPTENGINETHREAD_H

/******************************************************************
*
*  $Log: qxrdscriptenginethread.h,v $
*  Revision 1.1  2009/06/28 04:00:39  jennings
*  Partial implementation of separate thread for script engine
*
*
*******************************************************************/

