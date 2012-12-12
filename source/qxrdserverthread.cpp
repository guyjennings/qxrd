#include "qxrddebug.h"
#include "qxrdserverthread.h"

#include "qxrdserver.h"
#include <QMetaObject>
#include <QDateTime>
#include "qxrdapplication.h"
#include "qxrdexperiment.h"
#include "qxrdmutexlocker.h"

QxrdServerThread::QxrdServerThread(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr doc, QString name) :
  m_Saver(saver),
  m_Experiment(doc),
  m_Name(name),
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
  while (isRunning()) {
    {
      QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

      if (m_Server) return m_Server;
    }

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
  {
    QxrdExperimentPtr expt(m_Experiment);

    if (expt && qcepDebug(DEBUG_THREADS)) {
      expt->printMessage("Starting Spec Server Thread");
    }

    QxrdServerPtr server(new QxrdServer(m_Saver, m_Experiment, m_Name));


    if (server) {
      m_Mutex.lock();
      server -> moveToThread(g_Application->thread());
      m_Server = server;
      m_Mutex.unlock();
    }
  }

  int rc = -1;

  if (m_Server) {
    rc = exec();
  }

  {
    QxrdExperimentPtr expt(m_Experiment);

    if (expt && qcepDebug(DEBUG_THREADS)) {
      expt->printMessage(tr("Spec Server Thread Terminated with rc %1").arg(rc));
    }
  }
}

void QxrdServerThread::executeScript(QString cmd)
{
  emit execute(cmd);
}
