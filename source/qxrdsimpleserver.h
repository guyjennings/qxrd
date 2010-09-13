/******************************************************************
*
*  $Id: qxrdsimpleserver.h,v 1.2 2010/09/13 20:00:42 jennings Exp $
*
*******************************************************************/

#ifndef QXRDSIMPLESERVER_H
#define QXRDSIMPLESERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QScriptValue>
#include "qcepmacros.h"
#include "qxrdforwardtypes.h"

class QxrdSimpleServer : public QTcpServer
{
  Q_OBJECT;
public:
  QxrdSimpleServer(QxrdAcquisitionThreadPtr acqth, QString name, int port, QObject *parent=NULL);
public:
  void startServer(QHostAddress addr, int port);

public slots:
  void openNewConnection();
  void connectionClosed();
  void clientRead();
  void shutdown();

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

signals:
  void executeCommand(QString cmd);

public slots:
  void finishedCommand(QScriptValue result);

private:
  QxrdAcquisitionThreadPtr m_AcquisitionThread;
  QString                  m_Name;
  int                      m_Port;
  QTcpSocket              *m_Socket;
  HEADER_IDENT("$Id: qxrdsimpleserver.h,v 1.2 2010/09/13 20:00:42 jennings Exp $");
};

#endif // QXRDSIMPLESERVER_H

/******************************************************************
*
*  $Log: qxrdsimpleserver.h,v $
*  Revision 1.2  2010/09/13 20:00:42  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/08/04 20:22:58  jennings
*  Added simple socket server, and prefs to control which servers are run, and on which ports
*
*
*******************************************************************/

