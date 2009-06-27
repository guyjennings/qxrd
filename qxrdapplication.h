/******************************************************************
*
*  $Id: qxrdapplication.h,v 1.20 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#ifndef QXRDAPPLICATION_H
#define QXRDAPPLICATION_H

#include "qcepmacros.h"

#include <QApplication>
#include <QScriptEngine>

class QxrdWindow;
class QxrdServerThread;
class QxrdAcquisitionThread;

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
  QxrdAcquisitionThread         *m_AcquisitionThread;
  HEADER_IDENT("$Id: qxrdapplication.h,v 1.20 2009/06/27 22:50:32 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdapplication.h,v $
*  Revision 1.20  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

