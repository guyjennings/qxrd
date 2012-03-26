#include "qxrdacquisitionthread.h"

#include "qxrddataprocessor.h"
#include "qxrdapplication.h"
#include "qxrdexperiment.h"
#include "qxrdacquisition.h"

#include <QFuture>
#include <QVariant>
#include <QMetaObject>

//static int g_DetectorType = -1;

QxrdAcquisitionThread::QxrdAcquisitionThread(QxrdSettingsSaverWPtr saver,
                                             QxrdExperimentWPtr doc,
                                             QxrdDataProcessorWPtr proc,
                                             QxrdAllocatorWPtr allocator,
                                             int detectorType)
  : QxrdThread(),
    m_Saver(saver),
    m_Experiment(doc),
    m_Allocator(allocator),
    m_Processor(proc),
    m_DetectorType(detectorType),
    m_Acquisition()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisitionThread::QxrdAcquisitionThread(%p)\n", this);
  }
}

QxrdAcquisitionThread::~QxrdAcquisitionThread()
{
  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisitionThread::~QxrdAcquisitionThread(%p)\n", this);
  }
}

void QxrdAcquisitionThread::run()
{
  QxrdAcquisitionPtr acq = QxrdAcquisitionPtr(
        new QxrdAcquisition(m_Saver, m_Experiment, m_Processor, m_Allocator));

  acq -> initialize();

  m_Acquisition = acq;

  int rc = exec();

  {
    QxrdExperimentPtr exp(m_Experiment);

    if (exp && qcepDebug(DEBUG_THREADS)) {
      exp->printMessage(tr("Acquisition Thread Terminated with rc %1").arg(rc));
    }
  }
}

void QxrdAcquisitionThread::shutdown()
{
  exit();

  wait();
}

void QxrdAcquisitionThread::msleep(int msec)
{
  QThread::msleep(msec);
}

QxrdAcquisitionPtr QxrdAcquisitionThread::acquisition() const
{
  while (m_Acquisition == NULL) {
    QThread::msleep(50);
  }

  return m_Acquisition;
}

void QxrdAcquisitionThread::sleep(double time)
{
  QThread::usleep((int)(time*1e6));
}
