#include "qxrdserverthread.h"

#include "qxrdserver.h"
#include <QMetaObject>
#include <QDateTime>

QxrdServerThread::QxrdServerThread(QString name, int port)
  : m_Name(name),
    m_Port(port),
    m_Server(NULL)
{
}

QxrdServerThread::~QxrdServerThread()
{
  shutdown();

  delete m_Server;
}

QxrdServer *QxrdServerThread::server() const
{
  while (m_Server == NULL) {
    QThread::msleep(500);
  }

  return m_Server;
}

void QxrdServerThread::shutdown()
{
  INVOKE_CHECK(QMetaObject::invokeMethod(m_Server, "shutdown", Qt::QueuedConnection));

  wait();
}

void QxrdServerThread::run()
{
//  printf("start server\n");
  m_Server = new QxrdServer(m_Name, m_Port);

  connect(m_Server,             SIGNAL(printMessage(QString,QDateTime)), this,            SIGNAL(printMessage(QString,QDateTime)));
  connect(m_Server,             SIGNAL(statusMessage(QString,QDateTime)), this,            SIGNAL(statusMessage(QString,QDateTime)));
  connect(m_Server,             SIGNAL(criticalMessage(QString,QDateTime)), this,            SIGNAL(criticalMessage(QString,QDateTime)));

  m_Server -> startServer(QHostAddress::Any, m_Port);

  int rc = exec();

  emit printMessage(tr("spec server started on port %1").arg(m_Server->serverPort()));

//  printf("Server thread terminated with rc %d\n", rc);
}

void QxrdServerThread::executeScript(QString cmd)
{
  emit execute(cmd);
}
