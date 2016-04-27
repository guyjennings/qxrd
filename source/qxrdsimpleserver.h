#ifndef QXRDSIMPLESERVER_H
#define QXRDSIMPLESERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QScriptValue>
#include <QDateTime>

#include "qxrdexperiment-ptr.h"
#include "qcepproperty.h"
#include "qcepsettingssaver-ptr.h"
#include "qcepobject.h"

class QxrdSimpleServer : public QcepObject
{
  Q_OBJECT
public:
  QxrdSimpleServer(QxrdExperimentWPtr doc, QString name);
  virtual ~QxrdSimpleServer();

  Q_PROPERTY(int    runSimpleServer    READ get_RunSimpleServer WRITE set_RunSimpleServer)
  QCEP_INTEGER_PROPERTY(RunSimpleServer)

  Q_PROPERTY(int    simpleServerPort    READ get_SimpleServerPort WRITE set_SimpleServerPort)
  QCEP_INTEGER_PROPERTY(SimpleServerPort)

public:
  void startServer(QHostAddress addr, int port);
  void stopServer();

  virtual void readSettings(QSettings *settings, QString section);
  virtual void writeSettings(QSettings *settings, QString section);

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

protected:
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void criticalMessage(QString msg);

private:
  QxrdExperimentWPtr       m_Experiment;
  QString                  m_Name;
  int                      m_Port;
  QTcpServer               m_Server;
  QTcpSocket              *m_Socket;
};

#endif // QXRDSIMPLESERVER_H
