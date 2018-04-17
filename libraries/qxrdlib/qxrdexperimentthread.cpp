#include "qxrddebug.h"
#include "qxrdexperimentthread.h"
#include "qxrdexperiment.h"
#include "qxrdexperimentsettings.h"
#include "qxrdappcommon.h"

QxrdExperimentThread::QxrdExperimentThread(QString name) :
  inherited(name),
  m_Path(),
  m_Settings(),
  m_ExperimentMode()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperimentThread::QxrdExperimentThread(%p)\n", this);
  }
}

void QxrdExperimentThread::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

void QxrdExperimentThread::initialize(QcepObjectWPtr parent,
                                      QString path,
                                      QxrdExperimentSettingsPtr set,
                                      int mode)
{
  initialize(parent);

  m_Path            = path;
  m_Settings        = set;
  m_ExperimentMode  = mode;
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
        new QxrdExperiment("experiment"));

  expt -> initialize(sharedFromThis(),
                     m_Path,
                     m_ExperimentMode);

  m_Experiment = expt;

  printMessage("Start reading experiment settings");

  expt -> readSettings(m_Settings.data());

  printMessage("Finished reading experiment settings");

  int rc = exec();

  m_Experiment = QxrdExperimentPtr();
  m_Settings   = QxrdExperimentSettingsPtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Experiment thread terminated with rd %d\n", rc);
  }
}

QxrdExperimentPtr QxrdExperimentThread::experiment() const
{
  while (isRunning()) {
    if (m_Experiment) return m_Experiment;

    msleep(50);
  }

  return QxrdExperimentPtr();
}

QxrdExperimentThreadWPtr QxrdExperimentThread::findExperimentThread(QcepObjectWPtr p)
{
  QxrdExperimentThreadWPtr res =
      qSharedPointerDynamicCast<QxrdExperimentThread>(p);

  if (res == NULL) {
    QcepObjectPtr objp(p);

    if (objp) {
      res = findExperimentThread(objp->parentPtr());
    }
  }

  return res;
}
