#ifndef QXRDSIMPLESERVER_H
#define QXRDSIMPLESERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QScriptValue>
#include <QDateTime>

class QxrdSimpleServer : public QTcpServer
{
  Q_OBJECT;
public:
  QxrdSimpleServer(QString name, int port, QObject *parent=NULL);
public:
  void startServer(QHostAddress addr, int port);

public slots:
  void openNewConnection();
  void connectionClosed();
  void clientRead();
  void shutdown();

signals:
  void printMessage(QDateTime ts, QString msg);
  void statusMessage(QDateTime ts, QString msg);
  void criticalMessage(QDateTime ts, QString msg);

signals:
  void executeCommand(QString cmd);

public slots:
  void finishedCommand(QScriptValue result);

private:
  QString                  m_Name;
  int                      m_Port;
  QTcpSocket              *m_Socket;
};

#endif // QXRDSIMPLESERVER_H
