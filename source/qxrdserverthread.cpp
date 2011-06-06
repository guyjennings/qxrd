#include "qxrdserverthread.h"

#include "qxrdserver.h"
#include <QMetaObject>
#include <QDateTime>
#include "qxrdapplication.h"

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
    QThread::msleep(50);
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
  if (qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage("Starting Spec Server Thread");
  }

  m_Server = new QxrdServer(m_Name, m_Port);

  m_Server -> startServer(QHostAddress::Any, m_Port);
  g_Application->printMessage(tr("spec server started on port %1").arg(m_Server->serverPort()));

  int rc = exec();

  if (qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage(tr("Spec Server Thread Terminated with rc %1").arg(rc));
  }
}

void QxrdServerThread::executeScript(QString cmd)
{
  emit execute(cmd);
}
