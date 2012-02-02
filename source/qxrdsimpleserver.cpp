#include "qxrdsimpleserver.h"
#include <QTextStream>
#include <QDateTime>
#include <QThread>
#include "qxrdexperiment.h"

QxrdSimpleServer::QxrdSimpleServer(QxrdExperiment *doc, QString name, int port, QObject *parent) :
    QTcpServer(parent),
    m_Experiment(doc),
    m_Name(name),
    m_Port(port)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdSimpleServer::QxrdSimpleServer(%p)\n", this);
  }

  connect(this, SIGNAL(newConnection()), this, SLOT(openNewConnection()));
}

QxrdSimpleServer::~QxrdSimpleServer()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdSimpleServer::~QxrdSimpleServer(%p)\n", this);
  }
}

void QxrdSimpleServer::startServer(QHostAddress addr, int port)
{
  if (qcepDebug(DEBUG_SERVER)) {
    m_Experiment->printMessage(tr("Starting simple server on address %1, port %2")
                                .arg(addr.toString()).arg(port));
  }

  setMaxPendingConnections(1);

  if (isListening()) {
    close();
  }

  if (!listen(addr, port)) {
    m_Experiment->criticalMessage(tr("Failed to bind to address %1 port %2\nIs there another copy of qxrd running already?").arg(addr.toString()).arg(port));
  }
}

void QxrdSimpleServer::openNewConnection()
{
  m_Socket = nextPendingConnection();

  connect(m_Socket, SIGNAL(disconnected()), m_Socket, SLOT(deleteLater()));
  connect(m_Socket, SIGNAL(readyRead()), this,     SLOT(clientRead()));

  if (qcepDebug(DEBUG_SERVER)) {
    m_Experiment->printMessage(tr("New connection from %1").arg(m_Socket->peerAddress().toString()) );
  }

  connect(m_Socket, SIGNAL(disconnected()), this,     SLOT(connectionClosed()));
}

void QxrdSimpleServer::connectionClosed()
{
  if (qcepDebug(DEBUG_SERVER)) {
    m_Experiment->printMessage("Client closed connection");
  }
}

void QxrdSimpleServer::clientRead()
{
  QTextStream ts( m_Socket );

  while ( m_Socket->canReadLine() ) {
    QString str = ts.readLine();

    if (qcepDebug(DEBUG_SERVER)) {
      m_Experiment->printMessage(tr("Command: %1 received").arg(str));
    }

    emit executeCommand(str);
  }
}

void QxrdSimpleServer::finishedCommand(QScriptValue result)
{
  if (qcepDebug(DEBUG_SERVER)) {
    m_Experiment->printMessage(tr("Result: %1").arg(result.toString()));
  }

  if (m_Socket && (m_Socket->isWritable())) {
    m_Socket -> write(qPrintable(result.toString()+"\n"));
  }
}

void QxrdSimpleServer::shutdown()
{
  close();

  thread()->exit();
}
