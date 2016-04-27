#include "qxrddebug.h"
#include "qxrdserverthread.h"

#include "qxrdserver.h"
#include <QMetaObject>
#include <QDateTime>
#include "qxrdapplication.h"
#include "qxrdexperiment.h"
#include "qcepmutexlocker.h"

QxrdServerThread::QxrdServerThread(QxrdExperimentWPtr doc, QString name) :
  QxrdThread(doc),
  m_Experiment(doc),
  m_Name(name),
  m_Server(NULL)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdServerThread::QxrdServerThread(%p)\n", this);
  }

  setObjectName("specserver");
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
  while (isRunning()) {
    if (m_Server) return m_Server;

    QThread::msleep(50);
  }

  return QxrdServerPtr();
}

void QxrdServerThread::shutdown()
{
  exit();

  wait();
}

void QxrdServerThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printf("Spec Server Thread Started\n");
  }

  m_Server = QxrdServerPtr(
        new QxrdServer(m_Experiment, m_Name));

  int rc = exec();

  m_Server = QxrdServerPtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Spec Server Thread Terminated with rc %d\n", rc);
  }
}

void QxrdServerThread::executeScript(QString cmd)
{
  emit execute(cmd);
}
