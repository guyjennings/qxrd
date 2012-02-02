#include "qxrdsimpleserver.h"
#include "qxrdsimpleserverthread.h"
#include "qxrdexperiment.h"
#include "qxrdapplication.h"

QxrdSimpleServerThread::QxrdSimpleServerThread(QxrdExperiment *doc, QString name, int port) :
    m_Experiment(doc),
    m_Name(name),
    m_Port(port),
    m_Server(NULL)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdSimpleServerThread::QxrdSimpleServerThread(%p)\n", this);
  }

}

QxrdSimpleServerThread::~QxrdSimpleServerThread()
{
  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdSimpleServerThread::~QxrdSimpleServerThread(%p)\n", this);
  }
}

QxrdSimpleServerPtr QxrdSimpleServerThread::server() const
{
  while (m_Server == NULL) {
    QThread::msleep(50);
  }

  return m_Server;
}

void QxrdSimpleServerThread::shutdown()
{
  exit();

  wait();
}

void QxrdSimpleServerThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    m_Experiment->printMessage("Starting Simple Server Thread");
  }

  QxrdSimpleServerPtr server(new QxrdSimpleServer(m_Experiment, m_Name, m_Port));

  server -> startServer(QHostAddress::Any, m_Port);

  m_Server = server;

  int rc = exec();

  if (qcepDebug(DEBUG_THREADS)) {
    m_Experiment->printMessage(tr("Simple Server Thread Terminated with rc %1").arg(rc));
  }
}
