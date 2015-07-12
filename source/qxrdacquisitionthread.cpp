#include "qxrddebug.h"
#include "qxrdacquisitionthread.h"

#include "qxrddataprocessor.h"
#include "qxrdapplication.h"
#include "qxrdexperiment.h"
#include "qxrdacquisition.h"
#include "qcepmutexlocker.h"

#include <QFuture>
#include <QVariant>
#include <QMetaObject>

//static int g_DetectorType = -1;

QxrdAcquisitionThread::QxrdAcquisitionThread(QcepSettingsSaverWPtr saver,
                                             QxrdExperimentWPtr doc,
                                             QxrdDataProcessorWPtr proc,
                                             QcepAllocatorWPtr allocator,
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

  int rc = -1;

  if (acq) {
    acq -> initialize(acq);

    m_Mutex.lock();
    m_Acquisition = acq;
    m_Mutex.unlock();

    rc = exec();
  }

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
  while (isRunning()) {
    {
      QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

      if (m_Acquisition) return m_Acquisition;
    }

    QThread::msleep(50);
  }

  return QxrdAcquisitionPtr();
}

void QxrdAcquisitionThread::sleep(double time)
{
  QThread::usleep((int)(time*1e6));
}
