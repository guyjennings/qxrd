#include "qxrddebug.h"
#include "qxrdexperimentthread.h"
#include "qxrdapplication.h"
#include "qxrdexperiment.h"
//#include "qxrdexperimentperkinelmeranalysis.h"
//#include "qxrdexperimentpilatusacquisition.h"
//#include "qxrdexperimentpilatusanalysis.h"
//#include "qxrdexperimentsimulatedacquisition.h"
//#include "qxrdexperimentgenericanalysis.h"
#include "qcepmutexlocker.h"

#include <stdio.h>

QxrdExperimentThread::QxrdExperimentThread(QString path, QxrdApplicationWPtr app, QSettings *settings) :
  QxrdThread(),
  m_ExperimentPath(path),
  m_Application(app),
  m_Settings(settings)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperimentThread::QxrdExperimentThread(%p)\n", this);
  }

  setObjectName("experiment");
}

QxrdExperimentThread::~QxrdExperimentThread()
{
  if (qcepDebug(DEBUG_APP)) {
    QxrdApplicationPtr app(m_Application);

    if (app) {
      app->printMessage("QxrdExperimentThread::~QxrdExperimentThread");
    }
  }

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperimentThread::~QxrdExperimentThread(%p)\n", this);
  }

  ::fflush(stdout);

  shutdown();
}

void QxrdExperimentThread::init(QxrdExperimentThreadWPtr expThread)
{
  m_ExperimentThread = expThread;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperiment(QString path, QxrdApplicationWPtr app, QSettings *settings)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(path, app, settings));

  res->start();

  return res;
}

QxrdExperimentPtr QxrdExperimentThread::experiment()
{
  while (isRunning()) {
    {
      QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

      if (m_Experiment) return m_Experiment;
    }

    msleep(100);
  }

  return QxrdExperimentPtr();
}

void QxrdExperimentThread::run()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperimentThread::run(%p)\n", this);
  }

  {
    QxrdExperimentPtr doc;

    doc = QxrdExperimentPtr(
          new QxrdExperiment(sharedFromThis(), m_ExperimentPath, m_Application));

    if (doc) {
      doc -> initialize(m_Settings);

      m_Mutex.lock();
      m_Experiment = doc;
      m_Mutex.unlock();
    }
  }

  int rc = -1;

  rc = exec();

  {
    if (qcepDebug(DEBUG_THREADS)) {
      QxrdApplicationPtr app(m_Application);

      if (app) {
        app->printMessage(tr("Experiment Thread Terminated with rc %1").arg(rc));
      }
    }
  }

  {
    QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    m_Experiment = QxrdExperimentPtr();
  }
}

void QxrdExperimentThread::shutdown()
{
//  INVOKE_CHECK(QMetaObject::invokeMethod(m_Experiment, "shutdown", Qt::QueuedConnection));
  exit();

  wait();
}
