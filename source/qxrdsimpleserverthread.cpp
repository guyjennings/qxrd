#include "qxrdsimpleserver.h"
#include "qxrdsimpleserverthread.h"

QxrdSimpleServerThread::QxrdSimpleServerThread(QxrdAcquisitionThreadPtr acq, QString name, int port) :
    m_AcquisitionThread(acq),
    m_Name(name),
    m_Port(port),
    m_Server(NULL)
{
}

QxrdSimpleServerThread::~QxrdSimpleServerThread()
{
  shutdown();

  delete m_Server;
}

QxrdSimpleServerPtr QxrdSimpleServerThread::server() const
{
  while (m_Server == NULL) {
    QThread::msleep(500);
  }

  return m_Server;
}

void QxrdSimpleServerThread::shutdown()
{
  INVOKE_CHECK(QMetaObject::invokeMethod(m_Server, "shutdown", Qt::QueuedConnection));

  exit();

  wait();
}

void QxrdSimpleServerThread::run()
{
//  printf("start server\n");
  m_Server = QxrdSimpleServerPtr(new QxrdSimpleServer(QxrdAcquisitionThreadPtr(NULL), m_Name, m_Port));

  connect(m_Server,             SIGNAL(printMessage(QString)), this,            SIGNAL(printMessage(QString)));
  connect(m_Server,             SIGNAL(statusMessage(QString)), this,            SIGNAL(statusMessage(QString)));
  connect(m_Server,             SIGNAL(criticalMessage(QString)), this,            SIGNAL(criticalMessage(QString)));

  m_Server -> startServer(QHostAddress::Any, m_Port);

  int rc = exec();

//  printf("Server thread terminated with rc %d\n", rc);
}
