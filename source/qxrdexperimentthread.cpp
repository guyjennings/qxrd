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
  QxrdThread(QcepObjectWPtr()),
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

QxrdExperimentThreadPtr QxrdExperimentThread::newExperiment(QString path, QxrdApplicationWPtr app, QSettings *settings)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(
        new QxrdExperimentThread(path, app, settings));

  res->start();

  return res;
}

QxrdExperimentPtr QxrdExperimentThread::experiment()
{
  while (isRunning()) {
    if (m_Experiment) return m_Experiment;

    msleep(100);
  }

  return QxrdExperimentPtr();
}

void QxrdExperimentThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printf("Experiment Thread Started\n");
  }

  {
    QxrdExperimentPtr expt(
          new QxrdExperiment(sharedFromThis(), m_ExperimentPath, m_Application));

    if (expt) {
      expt -> initialize(m_Settings);
    }

    m_Experiment = expt;
  }

  int rc = exec();

  m_Experiment = QxrdExperimentPtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Experiment Thread Terminated with rc %d\n", rc);
  }
}

void QxrdExperimentThread::shutdown()
{
//  INVOKE_CHECK(QMetaObject::invokeMethod(m_Experiment, "shutdown", Qt::QueuedConnection));
  exit();

  wait();
}
