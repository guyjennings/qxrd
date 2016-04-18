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

  setObjectName("acquisition");
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
  if (qcepDebug(DEBUG_THREADS)) {
    printf("Acquisition Thread Started\n");
  }

  {
    QxrdAcquisitionPtr acq = QxrdAcquisitionPtr(
          new QxrdAcquisition(m_Saver, m_Experiment, m_Processor, m_Allocator),
          &QObject::deleteLater);

    if (acq) {
      acq -> initialize();
    }

    m_Acquisition = acq;
  }

  int rc = exec();

  m_Acquisition = QxrdAcquisitionPtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Acquisition Thread Terminated with rc %d\n", rc);
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
    if (m_Acquisition) return m_Acquisition;

    QThread::msleep(50);
  }

  return QxrdAcquisitionPtr();
}

void QxrdAcquisitionThread::sleep(double time)
{
  QThread::usleep((int)(time*1e6));
}
