#include "qxrddebug.h"
#include "qxrdsimpleserver.h"
#include "qxrdsimpleserverthread.h"
#include <QTextStream>
#include <QDateTime>
#include <QThread>
#include "qxrdexperiment.h"
#include <QScriptValueIterator>

QxrdSimpleServer::QxrdSimpleServer(QString name) :
  QcepSerializableObject(name),
  m_RunSimpleServer(this,"runSimpleServer", 1, "Run Simple Socket Server?"),
  m_SimpleServerPort(this,"simpleServerPort", 1234, "Port for Simple Socket Server")
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdSimpleServer::QxrdSimpleServer(%p)\n", this);
  }

  connect(prop_RunSimpleServer(), &QcepIntProperty::valueChanged, this, &QxrdSimpleServer::runModeChanged);
  connect(prop_SimpleServerPort(), &QcepIntProperty::valueChanged, this, &QxrdSimpleServer::serverPortChanged);

  connect(&m_Server, &QTcpServer::newConnection, this, &QxrdSimpleServer::openNewConnection);
}

QxrdSimpleServer::~QxrdSimpleServer()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdSimpleServer::~QxrdSimpleServer(%p)\n", this);
  }

  stopServer();
}

void QxrdSimpleServer::readSettings(QSettings *settings)
{
  QcepSerializableObject::readSettings(settings);

  runModeChanged();
}

void QxrdSimpleServer::writeSettings(QSettings *settings)
{
  QcepSerializableObject::writeSettings(settings);
}

void QxrdSimpleServer::runModeChanged()
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "runModeChanged");
  } else {
    if (get_RunSimpleServer()) {
      startServer(QHostAddress::Any, get_SimpleServerPort());
    } else {
      stopServer();
    }
  }
}

void QxrdSimpleServer::serverPortChanged()
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "serverPortChanged");
  } else {
    stopServer();

    if (get_RunSimpleServer()) {
      startServer(QHostAddress::Any, get_SimpleServerPort());
    }
  }
}

void QxrdSimpleServer::startServer(QHostAddress addr, int port)
{
  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("Starting simple server on address %1, port %2")
                 .arg(addr.toString()).arg(port));
  }

  m_Server.setMaxPendingConnections(1);

  if (m_Server.isListening()) {
    m_Server.close();
  }

  if (!m_Server.listen(addr, port)) {
    criticalMessage(tr("Failed to bind to address %1 port %2\nIs there another copy of qxrd running already?").arg(addr.toString()).arg(port));
  }
}

void QxrdSimpleServer::stopServer()
{
  if (m_Server.isListening()) {
    m_Server.close();
  }
}

void QxrdSimpleServer::openNewConnection()
{
  m_Socket = m_Server.nextPendingConnection();

  connect(m_Socket, &QIODevice::readyRead, this,     &QxrdSimpleServer::clientRead);

  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("New connection from %1").arg(m_Socket->peerAddress().toString()) );
  }
}

void QxrdSimpleServer::connectionClosed()
{
  if (qcepDebug(DEBUG_SERVER)) {
    printMessage("Client closed connection");
  }
}

void QxrdSimpleServer::clientRead()
{
  QTextStream ts( m_Socket );

  while ( m_Socket->canReadLine() ) {
    QString str = ts.readLine();

    if (qcepDebug(DEBUG_SERVER)) {
      printMessage(tr("Command: %1 received").arg(str));
    }

    emit executeCommand(str);
  }
}

void QxrdSimpleServer::finishedCommand(QScriptValue result)
{
  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("Result: %1").arg(result.toString()));
  }

  if (m_Socket && (m_Socket->isWritable())) {
    m_Socket -> write(qPrintable(QxrdScriptEngine::convertToString(result)+"\n"));
  }
}

void QxrdSimpleServer::shutdown()
{
  m_Server.close();

  thread()->exit();
}
