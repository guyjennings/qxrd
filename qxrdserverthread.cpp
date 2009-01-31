#include "qxrdserverthread.h"

#include "qxrdserver.h"

QxrdServerThread::QxrdServerThread(QxrdApplication *app, QxrdAcquisitionThread *acqth)
  : QThread(),
    m_Application(app),
    m_Server(NULL),
    m_AcquisitionThread(acqth)
{
}

QxrdServerThread::~QxrdServerThread()
{
  shutdown();

  delete m_Server;
}

void QxrdServerThread::run()
{
  printf("Server thread %p\n", QThread::currentThread());

  m_Server = new QxrdServer(m_Application, m_AcquisitionThread, "qxrd", NULL);

  connect(m_Server, SIGNAL(print_message(QString)), this, SIGNAL(print_message(QString)));

  emit serverRunning();

  exec();
}

void QxrdServerThread::shutdown()
{
  exit();

  wait(1000);
}
