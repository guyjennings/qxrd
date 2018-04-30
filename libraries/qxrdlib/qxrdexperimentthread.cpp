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
#ifndef QT_NO_DEBUG
  printf("Shutting down experiment thread\n");
#endif

  shutdown();

#ifndef QT_NO_DEBUG
  printf("Deleting experiment thread\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperimentThread::~QxrdExperimentThread(%p)\n", this);
  }
}

void QxrdExperimentThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printf("Experiment thread started\n");
  }

  {
    QxrdExperimentPtr expt =
        QxrdExperimentPtr(
          NEWPTR(QxrdExperiment("experiment")));

    if (expt) {
      expt -> initialize(sharedFromThis(),
                         m_Path,
                         m_ExperimentMode);

      printMessage("Start reading experiment settings");

      expt -> readSettings(m_Settings.data());

      printMessage("Finished reading experiment settings");

      m_Experiment = expt;
    }
  }

  int rc = exec();

  m_Experiment = QxrdExperimentPtr();
  m_Settings   = QxrdExperimentSettingsPtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Experiment thread terminated with rd %d\n", rc);
  }
}

void QxrdExperimentThread::haltExperiment()
{
  if (m_Experiment) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    INVOKE_CHECK(
          QMetaObject::invokeMethod(m_Experiment.data(),
                                    &QxrdExperiment::haltExperiment,
                                    Qt::BlockingQueuedConnection))
#else
    INVOKE_CHECK(
          QMetaObject::invokeMethod(m_Experiment.data(),
                                    "haltExperiment",
                                    Qt::BlockingQueuedConnection))
#endif
  }
}
QxrdExperimentPtr QxrdExperimentThread::experiment() const
{
  while (isRunning()) {
    if (m_Experiment) return m_Experiment;

    if (g_Application) {
      g_Application -> processEvents();
    }

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
