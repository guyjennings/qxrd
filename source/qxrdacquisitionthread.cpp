#include "qxrdacquisitionthread.h"

#ifdef HAVE_PERKIN_ELMER
#include "qxrdacquisitionperkinelmer.h"
#endif

#include "qxrdacquisitionpilatus.h"
#include "qxrdacquisitionareadetector.h"
#include "qxrdacquisitionsimulated.h"
#include "qxrdacquisitionfilewatcher.h"
#include "qxrddataprocessor.h"
#include "qxrdapplication.h"

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
                                             int detectorType,
                                             QSettings *settings,
                                             QString section)
  : QxrdThread(),
    m_Debug(true),
    m_Saver(saver),
    m_Experiment(doc),
    m_Allocator(allocator),
    m_Acquisition(),
    m_Processor(proc),
    m_DetectorType(detectorType),
    m_Settings(settings)
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
  {
    QxrdExperimentPtr exp(m_Experiment);

    if (exp) {
      if (qcepDebug(DEBUG_THREADS)) {
        exp->printMessage("Starting Acquisition Thread");
      }
    } else {
      return;
    }
  }

  QxrdAcquisitionPtr p/* = NULL*/;

  switch(m_DetectorType) {
  case QxrdAcquisition::SimulatedDetector:
    p = QxrdAcquisitionPtr(new QxrdAcquisitionSimulated(m_Saver, m_Experiment, m_Processor, m_Allocator, m_Settings, m_Section));
    break;

#ifdef HAVE_PERKIN_ELMER
  case QxrdAcquisition::PerkinElmerDetector:
    if (g_PEAvailable) {
      p = QxrdAcquisitionPtr(new QxrdAcquisitionPerkinElmer(m_Saver, m_Experiment, m_Processor, m_Allocator, m_Settings, m_Section));
    }
    break;
#endif

#ifdef HAVE_PILATUS
  case QxrdAcquisition::PilatusDetector:
    p = QxrdAcquisitionPtr(new QxrdAcquisitionPilatus(m_Saver, m_Experiment, m_Processor, m_Allocator, m_Settings, m_Section));
    break;
#endif

#ifdef HAVE_AREADETECTOR
  case QxrdAcquisition::EpicsAreaDetector:
    p = QxrdAcquisitionPtr(new QxrdAcquisitionAreaDetector(m_Saver, m_Experiment, m_Processor, m_Allocator, m_Settings, m_Section));
    break;
#endif

  case QxrdAcquisition::FileWatcherDetector:
    p = QxrdAcquisitionPtr(new QxrdAcquisitionFileWatcher(m_Saver, m_Experiment, m_Processor, m_Allocator, m_Settings, m_Section));
    break;
  }

  if (p == NULL) {
    p = QxrdAcquisitionPtr(new QxrdAcquisitionFileWatcher(m_Saver, m_Experiment, m_Processor, m_Allocator, m_Settings, m_Section));
  }

  p -> initialize();

  m_Acquisition = p;

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
