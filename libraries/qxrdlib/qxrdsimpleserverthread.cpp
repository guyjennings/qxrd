#include "qxrddebug.h"
#include "qxrdsimpleserver.h"
#include "qxrdsimpleserverthread.h"
#include "qxrdexperiment.h"
#include "qxrdapplication.h"
#include "qcepmutexlocker.h"

QxrdSimpleServerThread::QxrdSimpleServerThread(QString name) :
  inherited(name),
  m_Experiment(),
  m_Name(name),
  m_Server(NULL)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdSimpleServerThread::QxrdSimpleServerThread(%p)\n", this);
  }
}

void QxrdSimpleServerThread::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_Experiment = QxrdExperiment::findExperiment(parent);
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

    msleep(50);
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
        new QxrdSimpleServer("simpleServer"));

  m_Server -> initialize(sharedFromThis());

  int rc = exec();

  m_Server = QxrdSimpleServerPtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Simple Server Thread Terminated with rc %d\n", rc);
  }
}
