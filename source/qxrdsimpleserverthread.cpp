#include "qxrddebug.h"
#include "qxrdsimpleserver.h"
#include "qxrdsimpleserverthread.h"
#include "qxrdexperiment.h"
#include "qxrdapplication.h"
#include "qxrdmutexlocker.h"

QxrdSimpleServerThread::QxrdSimpleServerThread(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr doc, QString name) :
  m_Saver(saver),
  m_Experiment(doc),
  m_Name(name),
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
  while (isRunning()) {
    {
      QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

      if (m_Server) return m_Server;
    }

    QThread::msleep(50);
  }

  return QxrdSimpleServerPtr();
}

void QxrdSimpleServerThread::shutdown()
{
  exit();

  wait();
}

void QxrdSimpleServerThread::run()
{
  QxrdExperimentPtr expt(m_Experiment);

  if (expt && qcepDebug(DEBUG_THREADS)) {
    expt->printMessage("Starting Simple Server Thread");
  }

  QxrdSimpleServerPtr server(new QxrdSimpleServer(m_Saver, m_Experiment, m_Name));

  int rc = -1;

  if (server) {
    m_Mutex.lock();
    m_Server = server;
    m_Mutex.unlock();

    rc = exec();
  }

  if (expt && qcepDebug(DEBUG_THREADS)) {
    expt->printMessage(tr("Simple Server Thread Terminated with rc %1").arg(rc));
  }
}
