#include "qxrdacquisitionthread.h"

#include "qxrddataprocessor.h"
#include "qxrdapplication.h"
#include "qxrdexperiment.h"
#include "qxrdacquisition.h"

#include <QFuture>
#include <QVariant>
#include <QMetaObject>

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

//static int g_DetectorType = -1;

#ifdef HAVE_PERKIN_ELMER
static int g_PEAvailable = false;
#endif

QxrdAcquisitionThread::QxrdAcquisitionThread(QxrdSettingsSaverWPtr saver,
                                             QxrdExperimentWPtr doc,
                                             QxrdDataProcessorWPtr proc,
                                             QxrdAllocatorWPtr allocator,
                                             int detectorType)
  : QxrdThread(),
    m_Debug(true),
    m_Saver(saver),
    m_Experiment(doc),
    m_Allocator(allocator),
    m_Acquisition(),
    m_Processor(proc),
    m_DetectorType(detectorType)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisitionThread::QxrdAcquisitionThread(%p)\n", this);
  }

#ifdef HAVE_PERKIN_ELMER
  HINSTANCE xisllib;

  xisllib = LoadLibrary(L"XISL.dll");

  if (xisllib == NULL) {
    QxrdExperimentPtr exp(m_Experiment);

    if (exp) {
      exp->criticalMessage("XISL library is not available - cannot use PE detector");
    }
  } else {
    g_PEAvailable = true;
  }
#endif
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
  m_Acquisition = QxrdAcquisitionPtr(
        new QxrdAcquisition(m_Saver, m_Experiment, m_Processor, m_Allocator));

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
