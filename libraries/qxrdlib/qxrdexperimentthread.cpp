#include "qxrddebug.h"
#include "qxrdexperimentthread.h"
#include "qxrdexperiment.h"
#include "qxrdexperimentsettings.h"
#include "qxrdappcommon.h"

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentThread(QString path,
                                                                  QxrdAppCommonWPtr parent,
                                                                  QxrdExperimentSettingsPtr set,
                                                                  int mode)
{
  QxrdExperimentThreadPtr expth(
        new QxrdExperimentThread(path, parent, set, mode));

  expth -> start();

  return expth;
}

QxrdExperimentThread::QxrdExperimentThread(QString path,
                                           QxrdAppCommonWPtr parent,
                                           QxrdExperimentSettingsPtr set,
                                           int mode) :
  QxrdThread(QcepObjectWPtr()),
  m_Path(path),
  m_Parent(parent),
  m_Settings(set),
  m_ExperimentMode(mode)
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

  QxrdExperimentPtr expt =
      QxrdExperimentPtr(
        new QxrdExperiment(m_Path, "experiment", m_ExperimentMode));

  expt -> initialize(m_Parent);

  expt -> readSettings(m_Settings.data());

  m_Experiment = expt;

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

    msleep(50);
  }

  return QxrdExperimentPtr();
}

QxrdExperimentThreadWPtr QxrdExperimentThread::findExperimentThread(QObjectWPtr p)
{
  QxrdExperimentThreadWPtr res =
      qSharedPointerDynamicCast<QxrdExperimentThread>(p);

  if (res == NULL) {
    QcepObjectPtr objp =
        qSharedPointerDynamicCast<QcepObject>(p);

    if (objp) {
      res = findExperimentThread(objp->parentPtr());
    }
  }

  return res;
}
