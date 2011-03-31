#include "qxrdsimpleserver.h"
#include "qxrdsimpleserverthread.h"

QxrdSimpleServerThread::QxrdSimpleServerThread(QString name, int port) :
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

QxrdSimpleServer *QxrdSimpleServerThread::server() const
{
  while (m_Server == NULL) {
    QThread::msleep(50);
  }

  return m_Server;
}

void QxrdSimpleServerThread::shutdown()
{
  INVOKE_CHECK(QMetaObject::invokeMethod(m_Server, "shutdown", Qt::QueuedConnection));

  wait();
}

void QxrdSimpleServerThread::run()
{
//  printf("start server\n");
  m_Server = new QxrdSimpleServer(m_Name, m_Port);

  connect(m_Server,             SIGNAL(printMessage(QString,QDateTime)), this,            SIGNAL(printMessage(QString,QDateTime)));
  connect(m_Server,             SIGNAL(statusMessage(QString,QDateTime)), this,            SIGNAL(statusMessage(QString,QDateTime)));
  connect(m_Server,             SIGNAL(criticalMessage(QString,QDateTime)), this,            SIGNAL(criticalMessage(QString,QDateTime)));

  m_Server -> startServer(QHostAddress::Any, m_Port);

  int rc = exec();

//  printf("Server thread terminated with rc %d\n", rc);
}
