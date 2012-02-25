#include "qxrdserver.h"

#include <QHostAddress>
#include <QVariant>
#include <QScriptEngine>
#include "qxrdapplication.h"

QxrdServer::QxrdServer(QxrdExperimentWPtr doc, QString name, int port)
  : QSpecServer(doc, name, port)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdServer::QxrdServer(%p)\n", this);
  }
}

QxrdServer::~QxrdServer()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdServer::~QxrdServer(%p)\n", this);
  }
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
