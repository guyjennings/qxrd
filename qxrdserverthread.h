/******************************************************************
*
*  $Id: qxrdserverthread.h,v 1.8 2009/06/28 04:00:39 jennings Exp $
*
*******************************************************************/

#ifndef QXRDSERVERTHREAD_H
#define QXRDSERVERTHREAD_H

#include "qcepmacros.h"

#include <QThread>

class QxrdServer;
class QxrdAcquisitionThread;

class QxrdServerThread : public QThread
{
  Q_OBJECT;

public:
  QxrdServerThread(QxrdAcquisitionThread *acq, QString name);
  ~QxrdServerThread();

  QxrdServer *server() const;

  void shutdown();
  void executeScript(QString cmd);

signals:
  void printMessage(QString msg);
  void execute(QString cmd);

protected:
  void run();

private:
  QxrdAcquisitionThread *m_AcquisitionThread;
  QString                m_Name;
  QxrdServer            *m_Server;
  HEADER_IDENT("$Id: qxrdserverthread.h,v 1.8 2009/06/28 04:00:39 jennings Exp $");
};

#endif // QXRDSERVERTHREAD_H

/******************************************************************
*
*  $Log: qxrdserverthread.h,v $
*  Revision 1.8  2009/06/28 04:00:39  jennings
*  Partial implementation of separate thread for script engine
*
*  Revision 1.7  2009/06/27 22:50:33  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

