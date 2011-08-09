#include "qxrdsimpleserver.h"
#include "qxrdsimpleserverthread.h"
#include "qxrdapplication.h"

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
  if (qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage("Starting Simple Server Thread");
  }

  QxrdSimpleServer *server = new QxrdSimpleServer(m_Name, m_Port);

  server -> startServer(QHostAddress::Any, m_Port);

  m_Server.fetchAndStoreOrdered(server);

  int rc = exec();

  if (qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage(tr("Simple Server Thread Terminated with rc %1").arg(rc));
  }
}
