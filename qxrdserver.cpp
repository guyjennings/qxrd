#include "qxrdserver.h"

#include <QHostAddress>
#include <QVariant>
#include "qxrdapplication.h"

QxrdServer::QxrdServer(QxrdApplication *app, QxrdAcquisitionThread *acqth, QString name, QObject *parent)
  : QSpecServer(name, parent),
    m_Application(app),
    m_AcquisitionThread(acqth)
{
  startServer(QHostAddress::Any);
}

QVariant QxrdServer::executeCommand(QString cmd)
{
  return m_Application -> evaluate(cmd).toVariant();
}

QVariant QxrdServer::readProperty(QString name)
{
  QVariant res;

  if (name=="test") {
    return 42;
  }

  return QSpecServer::readProperty(name);
}
