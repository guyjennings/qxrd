/******************************************************************
*
*  $Id: qxrdsimpleserverthread.h,v 1.2 2010/09/13 20:00:42 jennings Exp $
*
*******************************************************************/

#ifndef QXRDSIMPLESERVERTHREAD_H
#define QXRDSIMPLESERVERTHREAD_H

#include "qcepmacros.h"
#include <QThread>
#include "qxrdforwardtypes.h"
//#include "qxrdsimpleserver.h"
//#include "qxrdscriptengine.h"

class QxrdSimpleServerThread : public QThread
{
  Q_OBJECT;
public:
  QxrdSimpleServerThread(QxrdAcquisitionThreadPtr acq, QString name, int port);
  ~QxrdSimpleServerThread();

  void shutdown();

  QxrdSimpleServerPtr server() const;

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

protected:
  void run();

private:
  QxrdAcquisitionThreadPtr m_AcquisitionThread;
  QString                  m_Name;
  int                      m_Port;
  QxrdSimpleServerPtr      m_Server;

  HEADER_IDENT("$Id: qxrdsimpleserverthread.h,v 1.2 2010/09/13 20:00:42 jennings Exp $");
};

#endif // QXRDSIMPLESERVERTHREAD_H

/******************************************************************
*
*  $Log: qxrdsimpleserverthread.h,v $
*  Revision 1.2  2010/09/13 20:00:42  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/08/04 20:22:58  jennings
*  Added simple socket server, and prefs to control which servers are run, and on which ports
*
*
*******************************************************************/

