#include "qxrdacquisitiontriggerthread.h"
#include "qxrdacquisitiontrigger.h"
#include "qxrdacquisition.h"
#include "qxrdexperiment.h"

QxrdAcquisitionTriggerThread::QxrdAcquisitionTriggerThread(QxrdSettingsSaverWPtr saver,
                                                           QxrdExperimentWPtr    doc,
                                                           QxrdAcquisitionWPtr   acq)
  : QxrdThread(),
    m_Debug(true),
    m_Saver(saver),
    m_Experiment(doc),
    m_Acquisition(acq)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisitionTriggerThread::QxrdAcquisitionTriggerThread(%p)\n", this);
  }
}

QxrdAcquisitionTriggerThread::~QxrdAcquisitionTriggerThread()
{
  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisitionTriggerThread::~QxrdAcquisitionTriggerThread(%p)\n", this);
  }
}

void QxrdAcquisitionTriggerThread::run()
{
  QxrdAcquisitionTriggerPtr p = QxrdAcquisitionTriggerPtr(new QxrdAcquisitionTrigger(m_Saver, m_Experiment, m_Acquisition));

  int rc = -1;

  if (p) {
    m_AcquisitionTrigger = p;

    rc = exec();
  }

  {
    QxrdExperimentPtr exp(m_Experiment);

    if (exp && qcepDebug(DEBUG_THREADS)) {
      exp->printMessage(tr("Acquisition Trigger Thread Terminated with rc %1").arg(rc));
    }
  }
}

void QxrdAcquisitionTriggerThread::shutdown()
{
  exit();

  wait();
}

void QxrdAcquisitionTriggerThread::msleep(int msec)
{
  QThread::msleep(msec);
}

QxrdAcquisitionTriggerPtr QxrdAcquisitionTriggerThread::acquisitionTrigger() const
{
  while (isRunning() && m_AcquisitionTrigger == NULL) {
    QThread::msleep(50);
  }

  return m_AcquisitionTrigger;
}

void QxrdAcquisitionTriggerThread::sleep(double time)
{
  QThread::usleep((int)(time*1e6));
}
