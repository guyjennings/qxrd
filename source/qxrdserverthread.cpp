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

  exit();

  wait();
}

void QxrdServerThread::run()
{
//  printf("start server\n");
  m_Server = new QxrdServer(m_Name, m_Port);

  connect(m_Server,             SIGNAL(printMessage(QDateTime,QString)), this,            SIGNAL(printMessage(QDateTime,QString)));
  connect(m_Server,             SIGNAL(statusMessage(QDateTime,QString)), this,            SIGNAL(statusMessage(QDateTime,QString)));
  connect(m_Server,             SIGNAL(criticalMessage(QDateTime,QString)), this,            SIGNAL(criticalMessage(QDateTime,QString)));

  m_Server -> startServer(QHostAddress::Any, m_Port);

  int rc = exec();

  emit printMessage(QDateTime::currentDateTime(),
                    tr("spec server started on port %1").arg(m_Server->serverPort()));

//  printf("Server thread terminated with rc %d\n", rc);
}

void QxrdServerThread::executeScript(QString cmd)
{
  emit execute(cmd);
}
