#include "qxrdserver.h"

#include <QHostAddress>
#include <QVariant>
#include <QScriptEngine>
#include "qxrdapplication.h"
#include "qxrdacquisitionthread.h"

QxrdServer::QxrdServer(QxrdAcquisitionThread *acqth, QString name, QObject *parent)
  : QSpecServer(name, parent),
    m_AcquisitionThread(acqth)
{
  printf("QxrdServer::QxrdServer\n");

  startServer(QHostAddress::Any);
}

QVariant QxrdServer::executeCommand(QString cmd)
{
  printf("QxrdServer::executeCommand(%s)\n", qPrintable(cmd));

  QVariant result = m_AcquisitionThread->evaluate(cmd);

  return result;
}

QVariant QxrdServer::readProperty(QString name)
{
  QVariant res;

  if (name=="test") {
    return 42;
  }

  return QSpecServer::readProperty(name);
}

