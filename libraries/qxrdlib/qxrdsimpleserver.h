#ifndef QXRDSIMPLESERVER_H
#define QXRDSIMPLESERVER_H

#include "qxrdlib_global.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QScriptValue>
#include <QDateTime>

#include "qxrdexperiment-ptr.h"
#include "qcepproperty.h"
#include "qcepserializableobject.h"

class QXRD_EXPORT QxrdSimpleServer : public QcepSerializableObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdSimpleServer(QString name);
  virtual ~QxrdSimpleServer();

  Q_PROPERTY(int    runSimpleServer    READ get_RunSimpleServer WRITE set_RunSimpleServer)
  QCEP_INTEGER_PROPERTY(RunSimpleServer)

  Q_PROPERTY(int    simpleServerPort    READ get_SimpleServerPort WRITE set_SimpleServerPort)
  QCEP_INTEGER_PROPERTY(SimpleServerPort)

public:
  void startServer(QHostAddress addr, int port);
  void stopServer();

  virtual void readSettings(QSettings *settings);
  virtual void writeSettings(QSettings *settings);

public slots:
  void runModeChanged();
  void serverPortChanged();
  void openNewConnection();
  void connectionClosed();
  void clientRead();
  void shutdown();

signals:
  void executeCommand(QString cmd);

public slots:
  void finishedCommand(QScriptValue result);

private:
  int                      m_Port;
  QTcpServer               m_Server;
  QTcpSocket              *m_Socket;
};

Q_DECLARE_METATYPE(QxrdSimpleServer*)

#endif // QXRDSIMPLESERVER_H
