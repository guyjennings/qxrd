#include "qxrdserverthread.h"

#include "qxrdserver.h"
#include <QMetaObject>

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
  QMetaObject::invokeMethod(m_Server, "shutdown", Qt::QueuedConnection);

  exit();

  wait(1000);
}

void QxrdServerThread::run()
{
  m_Server = new QxrdServer(m_AcquisitionThread, m_Name);

  connect(m_Server, SIGNAL(printMessage(QString)), this, SIGNAL(printMessage(QString)));
  connect(this, SIGNAL(execute(QString)), m_Server, SLOT(executeCommand(QString)));

  exec();
}

void QxrdServerThread::executeScript(QString cmd)
{
  emit execute(cmd);
}
