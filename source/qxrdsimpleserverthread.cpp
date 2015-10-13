#include "qxrddebug.h"
#include "qxrdsimpleserver.h"
#include "qxrdsimpleserverthread.h"
#include "qxrdexperiment.h"
#include "qxrdapplication.h"
#include "qcepmutexlocker.h"

QxrdSimpleServerThread::QxrdSimpleServerThread(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr doc, QString name) :
  m_Saver(saver),
  m_Experiment(doc),
  m_Name(name),
  m_Server(NULL)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdSimpleServerThread::QxrdSimpleServerThread(%p)\n", this);
  }

  setObjectName("simpleserver");
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
      QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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
  {
    QxrdExperimentPtr expt(m_Experiment);

    if (expt && qcepDebug(DEBUG_THREADS)) {
      expt->printMessage("Starting Simple Server Thread");
    }

    QxrdSimpleServerPtr server(new QxrdSimpleServer(m_Saver, m_Experiment, m_Name));

    if (server) {
      m_Mutex.lock();
//      server->moveToThread(g_Application->thread());
      m_Server = server;
      m_Mutex.unlock();
    }
  }

  int rc = -1;

  rc = exec();

  {
    QxrdExperimentPtr expt(m_Experiment);
    if (expt && qcepDebug(DEBUG_THREADS)) {
      expt->printMessage(tr("Simple Server Thread Terminated with rc %1").arg(rc));
    }
  }
}
