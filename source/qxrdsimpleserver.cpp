#include "qxrdsimpleserver.h"
#include <QTextStream>
#include <QDateTime>

QxrdSimpleServer::QxrdSimpleServer(QString name, int port, QObject *parent) :
    QTcpServer(parent),
    m_Name(name),
    m_Port(port)
{
  connect(this, SIGNAL(newConnection()), this, SLOT(openNewConnection()));
}

void QxrdSimpleServer::startServer(QHostAddress addr, int port)
{
  setMaxPendingConnections(1);

  if (isListening()) {
    close();
  }

  if (!listen(addr, port)) {
    emit printMessage(QDateTime::currentDateTime(),
                      tr("Failed to bind to address %1 port %2").arg(addr.toString()).arg(port));
  }
}

void QxrdSimpleServer::openNewConnection()
{
  m_Socket = nextPendingConnection();

  connect(m_Socket, SIGNAL(disconnected()), m_Socket, SLOT(deleteLater()));
  connect(m_Socket, SIGNAL(readyRead()), this,     SLOT(clientRead()));

  emit printMessage(QDateTime::currentDateTime(),
                    tr("New connection from %1").arg(m_Socket->peerAddress().toString()) );

  connect(m_Socket, SIGNAL(disconnected()), this,     SLOT(connectionClosed()));
}

void QxrdSimpleServer::connectionClosed()
{
  emit printMessage(QDateTime::currentDateTime(), "Client closed connection");
}

void QxrdSimpleServer::clientRead()
{
  QTextStream ts( m_Socket );

  while ( m_Socket->canReadLine() ) {
    QString str = ts.readLine();

    emit printMessage(QDateTime::currentDateTime(),
                      tr("Command: %1 received").arg(str));

    emit executeCommand(str);
  }
}

void QxrdSimpleServer::finishedCommand(QScriptValue result)
{
  emit printMessage(QDateTime::currentDateTime(),
                    tr("Result: %1").arg(result.toString()));

  if (m_Socket && (m_Socket->isWritable())) {
    m_Socket -> write(qPrintable(result.toString()+"\n"));
  }
}

void QxrdSimpleServer::shutdown()
{
  close();
}
