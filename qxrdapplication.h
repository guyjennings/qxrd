/******************************************************************
*
*  $Id: qxrdapplication.h,v 1.21 2009/06/28 04:00:39 jennings Exp $
*
*******************************************************************/

#ifndef QXRDAPPLICATION_H
#define QXRDAPPLICATION_H

#include "qcepmacros.h"

#include <QApplication>
#include <QScriptEngine>

class QxrdWindow;
class QxrdServerThread;
class QxrdServer;
class QxrdAcquisitionThread;
class QxrdAcquisition;
class QxrdScriptEngineThread;
class QxrdScriptEngine;

class QxrdApplication : public QApplication
{
  Q_OBJECT;

 public:
  QxrdApplication(int &argc, char **argv);
  ~QxrdApplication();

  QxrdAcquisitionThread *acquisitionThread();
  QxrdWindow *window();

  void executeScript(QString cmd);

 public slots:
  void shutdownThreads();
  void possiblyQuit();

 signals:
  void printMessage(QString msg);

 public:
  bool wantToQuit();

 private:
  QxrdWindow                    *m_Window;
  QxrdServerThread              *m_ServerThread;
  QxrdServer                    *m_Server;
  QxrdAcquisitionThread         *m_AcquisitionThread;
  QxrdAcquisition               *m_Acquisition;
  QxrdScriptEngineThread        *m_ScriptEngineThread;
  QxrdScriptEngine              *m_ScriptEngine;
  HEADER_IDENT("$Id: qxrdapplication.h,v 1.21 2009/06/28 04:00:39 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdapplication.h,v $
*  Revision 1.21  2009/06/28 04:00:39  jennings
*  Partial implementation of separate thread for script engine
*
*  Revision 1.20  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

