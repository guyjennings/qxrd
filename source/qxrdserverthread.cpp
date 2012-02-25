#include "qxrdserverthread.h"

#include "qxrdserver.h"
#include <QMetaObject>
#include <QDateTime>
#include "qxrdapplication.h"

QxrdServerThread::QxrdServerThread(QxrdExperimentWPtr doc, QString name, int port)
    : m_Experiment(doc),
    m_Name(name),
    m_Port(port),
    m_Server(NULL)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdServerThread::QxrdServerThread(%p)\n", this);
  }
}

QxrdServerThread::~QxrdServerThread()
{
  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdServerThread::~QxrdServerThread(%p)\n", this);
  }
}

QxrdServerPtr QxrdServerThread::server() const
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
  QxrdExperimentPtr expt(m_Experiment);

  if (expt && qcepDebug(DEBUG_THREADS)) {
    expt->printMessage("Starting Spec Server Thread");
  }

  QxrdServerPtr server(new QxrdServer(m_Experiment, m_Name, m_Port));

  server -> startServer(QHostAddress::Any, m_Port);

  m_Server = server;

  if (expt) {
    expt->printMessage(tr("spec server started on port %1").arg(m_Server->serverPort()));
  }

  int rc = exec();

  if (expt && qcepDebug(DEBUG_THREADS)) {
    expt->printMessage(tr("Spec Server Thread Terminated with rc %1").arg(rc));
  }
}

void QxrdServerThread::executeScript(QString cmd)
{
  emit execute(cmd);
}
