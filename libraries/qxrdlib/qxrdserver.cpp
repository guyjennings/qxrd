#include "qxrddebug.h"
#include "qxrdserver.h"

#include <QHostAddress>
#include <QVariant>
#include <QScriptEngine>
#include <QThread>
#include "qxrdapplication.h"
#include "qcepexperiment.h"
#include "qxrdexperiment.h"

QxrdServer::QxrdServer(QString name) :
  QcepObject(name),
  m_RunSpecServer(this,"runSpecServer", 1, "Run SPEC Server?"),
  m_SpecServerPort(this,"specServerPort", -1, "Port for SPEC Server"),
  m_Server(name)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdServer::QxrdServer(%p)\n", this);
  }

  connect(prop_RunSpecServer(), &QcepIntProperty::valueChanged, this, &QxrdServer::runModeChanged);
  connect(prop_SpecServerPort(), &QcepIntProperty::valueChanged, this, &QxrdServer::serverPortChanged);

  connect(&m_Server, &QSpecServer::executeCommand,
          this,      &QxrdServer::executeCommand);
}

void QxrdServer::initialize(QcepObjectWPtr parent)
{
  m_Server.initialize(parent);
}

QxrdServer::~QxrdServer()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdServer::~QxrdServer(%p)\n", this);
  }
}

void QxrdServer::readSettings(QSettings *settings)
{
  QcepObject::readSettings(settings);

  runModeChanged();
}

void QxrdServer::writeSettings(QSettings *settings)
{
  QcepObject::writeSettings(settings);
}

void QxrdServer::runModeChanged()
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "runModeChanged");
  } else {
    if (get_RunSpecServer()) {
      m_Server.startServer(QHostAddress::Any, get_SpecServerPort());
    } else {
      m_Server.stopServer();
    }
  }
}

void QxrdServer::serverPortChanged()
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "serverPortChanged");
  } else {
    m_Server.stopServer();

    if (get_RunSpecServer()) {
      m_Server.startServer(QHostAddress::Any, get_SpecServerPort());
    }
  }
}

void QxrdServer::shutdown()
{
  m_Server.close();

  thread()->exit();
}

void QxrdServer::finishedCommand(QScriptValue result)
{
  m_Server.finishedCommand(result);
}
