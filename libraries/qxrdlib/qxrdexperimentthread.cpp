#include "qxrddebug.h"
#include "qxrdexperimentthread.h"
#include "qxrdexperiment.h"

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentThread(
    QString path, QxrdApplicationWPtr parent, QxrdExperimentSettingsPtr set)
{
  QxrdExperimentThreadPtr expth(
        new QxrdExperimentThread(path, parent, set));

  expth -> start();

  return expth;
}

QxrdExperimentThread::QxrdExperimentThread(
    QString path, QxrdApplicationWPtr parent, QxrdExperimentSettingsPtr set) :
  QxrdThread(QcepObjectWPtr()),
  m_Path(path),
  m_Parent(parent),
  m_Settings(set)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperimentThread::QxrdExperimentThread(%p)\n", this);
  }

  setObjectName("experimentThread");
}

QxrdExperimentThread::~QxrdExperimentThread()
{
  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperimentThread::~QxrdExperimentThread(%p)\n", this);
  }
}

void QxrdExperimentThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printf("Experiment thread started\n");
  }

  m_Experiment = QxrdExperiment::newExperiment(m_Path, m_Parent, m_Settings);

  int rc = exec();

  m_Experiment = QxrdExperimentPtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Experiment thread terminated with rd %d\n", rc);
  }
}

void QxrdExperimentThread::shutdown()
{
  exit();

  wait();
}

QxrdExperimentPtr QxrdExperimentThread::experiment() const
{
  while (isRunning()) {
    if (m_Experiment) return m_Experiment;

    QThread::msleep(50);
  }

  return QxrdExperimentPtr();
}