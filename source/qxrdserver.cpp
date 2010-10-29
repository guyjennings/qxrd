#include "qxrdserver.h"

#include <QHostAddress>
#include <QVariant>
#include <QScriptEngine>
#include "qxrdapplication.h"
#include "qxrdacquisitionthread.h"

QxrdServer::QxrdServer(QxrdAcquisitionThreadPtr acqth, QString name, int port, QObject *parent)
  : QSpecServer(name, port, parent),
    m_AcquisitionThread(acqth)
{
  emit printMessage("QxrdServer::QxrdServer");
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
}
