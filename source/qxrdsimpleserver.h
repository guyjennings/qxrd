#ifndef QXRDSIMPLESERVER_H
#define QXRDSIMPLESERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QScriptValue>
#include <QDateTime>

class QxrdExperiment;

class QxrdSimpleServer : public QTcpServer
{
  Q_OBJECT
public:
  QxrdSimpleServer(QxrdExperiment *doc, QString name, int port, QObject *parent=NULL);
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

typedef QSharedPointer<QxrdSimpleServer> QxrdSimpleServerPtr;

#endif // QXRDSIMPLESERVER_H
