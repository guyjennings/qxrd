#include "qxrdserverthread.h"

#include "qxrdserver.h"
#include <QMetaObject>
#include <QDateTime>
#include "qxrdapplication.h"

QxrdServerThread::QxrdServerThread(QxrdExperiment *doc, QString name, int port)
    : m_Experiment(doc),
    m_Name(name),
    m_Port(port),
    m_Server(NULL)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdServerThread::QxrdServerThread\n");
  }
}

QxrdServerThread::~QxrdServerThread()
{
  shutdown();

  delete m_Server;

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdServerThread::~QxrdServerThread\n");
  }
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
  exit();

  wait();
}

void QxrdServerThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    m_Experiment->printMessage("Starting Spec Server Thread");
  }

  QxrdServer *server = new QxrdServer(m_Experiment, m_Name, m_Port);

  server -> startServer(QHostAddress::Any, m_Port);

  m_Server.fetchAndStoreOrdered(server);

  m_Experiment->printMessage(tr("spec server started on port %1").arg(m_Server->serverPort()));

  int rc = exec();

  if (qcepDebug(DEBUG_THREADS)) {
    m_Experiment->printMessage(tr("Spec Server Thread Terminated with rc %1").arg(rc));
  }
}

void QxrdServerThread::executeScript(QString cmd)
{
  emit execute(cmd);
}
