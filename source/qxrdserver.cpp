#include "qxrdserver.h"

#include <QHostAddress>
#include <QVariant>
#include <QScriptEngine>
#include <QThread>
#include "qxrdapplication.h"

QxrdServer::QxrdServer(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr doc, QString name) :
  QSpecServer(doc, name),
  m_RunSpecServer(saver, this,"runSpecServer", 1, "Run SPEC Server?"),
  m_SpecServerPort(saver, this,"specServerPort", -1, "Port for SPEC Server")
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdServer::QxrdServer(%p)\n", this);
  }

  connect(prop_RunSpecServer(), SIGNAL(valueChanged(int,int)), this, SLOT(runModeChanged()));
  connect(prop_SpecServerPort(), SIGNAL(valueChanged(int,int)), this, SLOT(serverPortChanged()));
}

QxrdServer::~QxrdServer()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdServer::~QxrdServer(%p)\n", this);
  }
}

void QxrdServer::readSettings(QSettings *settings, QString section)
{
  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdServer::writeSettings(QSettings *settings, QString section)
{
  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdServer::runModeChanged()
{
  printf("Need to implement QxrdServer::runModeChanged()\n");
}

void QxrdServer::serverPortChanged()
{
  printf("Need to implement QxrdServer::serverPortChanged()\n");
}

QVariant QxrdServer::executeCommand(QString /*cmd*/)
{
//  printf("QxrdServer::executeCommand(%s)\n", qPrintable(cmd));

//  QVariant result = m_AcquisitionThread->evaluate(cmd);

  return 42;
}

QVariant QxrdServer::readProperty(QString name)
{
  QVariant res;

  if (name=="test") {
    return 42;
  }

  return QSpecServer::readProperty(name);
}

void QxrdServer::shutdown()
{
  close();

  thread()->exit();
}
