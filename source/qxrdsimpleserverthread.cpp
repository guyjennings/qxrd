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
    if (m_Server) return m_Server;

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
  if (qcepDebug(DEBUG_THREADS)) {
    printf("Simple Server Thread Started\n");
  }

  m_Server = QxrdSimpleServerPtr(
        new QxrdSimpleServer(m_Saver, m_Experiment, m_Name),
        &QObject::deleteLater);

  int rc = exec();

  m_Server = QxrdSimpleServerPtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Simple Server Thread Terminated with rc %d\n", rc);
  }
}
