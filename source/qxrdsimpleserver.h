#ifndef QXRDSIMPLESERVER_H
#define QXRDSIMPLESERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QScriptValue>
#include <QDateTime>
#include "qxrdexperiment.h"

class QxrdSimpleServer : public QTcpServer
{
  Q_OBJECT
public:
  QxrdSimpleServer(QxrdExperiment *doc, QString name, int port);
  virtual ~QxrdSimpleServer();

public:
  void startServer(QHostAddress addr, int port);

public slots:
  void openNewConnection();
  void connectionClosed();
  void clientRead();
  void shutdown();

signals:
  void executeCommand(QString cmd);

public slots:
  void finishedCommand(QScriptValue result);

private:
  QxrdExperiment          *m_Experiment;
  QString                  m_Name;
  int                      m_Port;
  QTcpSocket              *m_Socket;
};

#endif // QXRDSIMPLESERVER_H

class QxrdSimpleServer;
typedef QSharedPointer<QxrdSimpleServer> QxrdSimpleServerPtr;
