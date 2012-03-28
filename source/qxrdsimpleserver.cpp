#include "qxrdsimpleserver.h"
#include <QTextStream>
#include <QDateTime>
#include <QThread>
#include "qxrdexperiment.h"

QxrdSimpleServer::QxrdSimpleServer(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr doc, QString name) :
  QTcpServer(NULL),
  m_RunSimpleServer(saver, this,"runSimpleServer", 1, "Run Simple Socket Server?"),
  m_SimpleServerPort(saver, this,"simpleServerPort", 1234, "Port for Simple Socket Server"),
  m_Experiment(doc),
  m_Name(name)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdSimpleServer::QxrdSimpleServer(%p)\n", this);
  }

  connect(prop_RunSimpleServer(), SIGNAL(valueChanged(int,int)), this, SLOT(runModeChanged()));
  connect(prop_SimpleServerPort(), SIGNAL(valueChanged(int,int)), this, SLOT(serverPortChanged()));

  connect(this, SIGNAL(newConnection()), this, SLOT(openNewConnection()));
}

QxrdSimpleServer::~QxrdSimpleServer()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdSimpleServer::~QxrdSimpleServer(%p)\n", this);
  }

  stopServer();
}

void QxrdSimpleServer::readSettings(QSettings *settings, QString section)
{
  QcepProperty::readSettings(this, &staticMetaObject, section, settings);

  runModeChanged();
}

void QxrdSimpleServer::writeSettings(QSettings *settings, QString section)
{
  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
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

void QxrdSimpleServer::printMessage(QString msg, QDateTime ts)
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->printMessage(msg, ts);
  }
}

void QxrdSimpleServer::criticalMessage(QString msg)
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->criticalMessage(msg);
  }
}

void QxrdSimpleServer::startServer(QHostAddress addr, int port)
{
  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("Starting simple server on address %1, port %2")
                 .arg(addr.toString()).arg(port));
  }

  setMaxPendingConnections(1);

  if (isListening()) {
    close();
  }

  if (!listen(addr, port)) {
    criticalMessage(tr("Failed to bind to address %1 port %2\nIs there another copy of qxrd running already?").arg(addr.toString()).arg(port));
  }
}

void QxrdSimpleServer::stopServer()
{
  if (isListening()) {
    close();
  }
}

void QxrdSimpleServer::openNewConnection()
{
  m_Socket = nextPendingConnection();

  connect(m_Socket, SIGNAL(disconnected()), m_Socket, SLOT(deleteLater()));
  connect(m_Socket, SIGNAL(readyRead()), this,     SLOT(clientRead()));

  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("New connection from %1").arg(m_Socket->peerAddress().toString()) );
  }

  connect(m_Socket, SIGNAL(disconnected()), this,     SLOT(connectionClosed()));
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
    m_Socket -> write(qPrintable(result.toString()+"\n"));
  }
}

void QxrdSimpleServer::shutdown()
{
  close();

  thread()->exit();
}
