#include "qxrdserverthread.h"

#include "qxrdserver.h"

QxrdServerThread::QxrdServerThread(QxrdAcquisitionThread *acq, QString name)
  : m_AcquisitionThread(acq),
    m_Name(name),
    m_Server(NULL)
{
}

QxrdServerThread::~QxrdServerThread()
{
  shutdown();

  delete m_Server;
}

void QxrdServerThread::shutdown()
{
}

void QxrdServerThread::run()
{
  m_Server = new QxrdServer(m_AcquisitionThread, m_Name);

  connect(m_Server, SIGNAL(printMessage(QString)), this, SIGNAL(printMessage(QString)));

  exec();
}
