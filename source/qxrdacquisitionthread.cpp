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

static int g_DetectorType = -1;

#ifdef HAVE_PERKIN_ELMER
static int g_PEAvailable = false;
#endif

QxrdAcquisitionThread::QxrdAcquisitionThread(QxrdSettingsSaver *saver,
                                             QxrdExperiment *doc,
                                             QxrdDataProcessor *proc,
                                             QxrdAllocator *allocator,
                                             int detectorType,
                                             QSettings *settings,
                                             QString section)
  : QxrdThread(),
    m_Debug(true),
    m_Saver(saver),
    m_Experiment(doc),
    m_Allocator(allocator),
    m_Acquisition(NULL),
    m_Processor(proc),
    m_DetectorType(detectorType),
    m_Settings(settings)
{
#ifdef HAVE_PERKIN_ELMER
  HINSTANCE xisllib;

  xisllib = LoadLibrary(L"XISL.dll");

  if (xisllib == NULL) {
    m_Experiment->criticalMessage("XISL library is not available - cannot use PE detector");
  } else {
    g_PEAvailable = true;
  }
#endif
}

QxrdAcquisitionThread::~QxrdAcquisitionThread()
{
  shutdown();

//  delete m_Acquisition;

  m_Acquisition->deleteLater();
}

void QxrdAcquisitionThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    m_Experiment->printMessage("Starting Acquisition Thread");
  }

  QxrdAcquisition *p;

  switch(m_DetectorType) {
  case 0:
  default:
    p = new QxrdAcquisitionSimulated(m_Saver, m_Experiment, m_Processor, m_Allocator, m_Settings, m_Section);
    g_DetectorType = 0;
    break;

#ifdef HAVE_PERKIN_ELMER
  case 1:
    if (g_PEAvailable) {
      p = new QxrdAcquisitionPerkinElmer(m_Saver, m_Experiment, m_Processor, m_Allocator, m_Settings, m_Section);
      g_DetectorType = 1;
    } else {
      p = new QxrdAcquisitionSimulated(m_Saver, m_Experiment, m_Processor, m_Allocator, m_Settings, m_Section);
      g_DetectorType = 0;
    }
    break;
#endif

#ifdef HAVE_PILATUS
  case 2:
    p = new QxrdAcquisitionPilatus(m_Saver, m_Experiment, m_Processor, m_Allocator, m_Settings, m_Section);
    g_DetectorType = 2;
    break;
#endif

#ifdef HAVE_AREADETECTOR
  case 3:
    p = new QxrdAcquisitionAreaDetector(m_Saver, m_Experiment, m_Processor, m_Allocator, m_Settings, m_Section);
    g_DetectorType = 3;
    break;
#endif

  case 4:
    p = new QxrdAcquisitionFileWatcher(m_Saver, m_Experiment, m_Processor, m_Allocator, m_Settings, m_Section);
    g_DetectorType = 4;
    break;
  }

  m_Acquisition.fetchAndStoreOrdered(p);

  int rc = exec();

  if (qcepDebug(DEBUG_THREADS)) {
    m_Experiment->printMessage(tr("Acquisition Thread Terminated with rc %1").arg(rc));
  }
}

void QxrdAcquisitionThread::initialize()
{
  if (m_Acquisition) {
    INVOKE_CHECK(QMetaObject::invokeMethod(m_Acquisition,"initialize",Qt::BlockingQueuedConnection));
    m_Acquisition->set_DetectorType(g_DetectorType);
    m_Acquisition->set_DetectorTypeName(detectorTypeNames()[g_DetectorType]);
  }
}

void QxrdAcquisitionThread::shutdown()
{
  INVOKE_CHECK(QMetaObject::invokeMethod(m_Acquisition,"shutdown",Qt::QueuedConnection));

  wait();
}

void QxrdAcquisitionThread::doAcquire()
{
  INVOKE_CHECK(QMetaObject::invokeMethod(m_Acquisition, "acquire", Qt::QueuedConnection));
}

void QxrdAcquisitionThread::doAcquireDark()
{
  INVOKE_CHECK(QMetaObject::invokeMethod(m_Acquisition, "acquireDark", Qt::QueuedConnection));
}

void QxrdAcquisitionThread::msleep(int msec)
{
  QThread::msleep(msec);
}

void QxrdAcquisitionThread::cancel()
{
  INVOKE_CHECK(QMetaObject::invokeMethod(m_Acquisition, "cancel", Qt::QueuedConnection));
}

void QxrdAcquisitionThread::cancelDark()
{
  INVOKE_CHECK(QMetaObject::invokeMethod(m_Acquisition, "cancelDark", Qt::QueuedConnection));
}

QxrdAcquisition *QxrdAcquisitionThread::acquisition() const
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

QStringList QxrdAcquisitionThread::detectorTypeNames()
{
  QStringList res;

  res << "Simulated Detector"
      << "Perkin Elmer Flat Panel"
      << "Pilatus"
      << "EPICS Area Detector"
      << "Files in Directory";

  return res;
}

int QxrdAcquisitionThread::detectorType()
{
  return g_DetectorType;
}
