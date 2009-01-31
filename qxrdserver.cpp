#include "qxrdserver.h"

#include <QHostAddress>

QxrdServer::QxrdServer(QxrdApplication *app, QxrdAcquisitionThread *acqth, QString name, QObject *parent)
  : QSpecServer(name, parent),
    m_Application(app),
    m_AcquisitionThread(acqth)
{
  startServer(QHostAddress::Any);
}
