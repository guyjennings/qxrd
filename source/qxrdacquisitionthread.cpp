#include "qxrdacquisitionthread.h"

#ifdef HAVE_PERKIN_ELMER
#include "qxrdacquisitionperkinelmer.h"
#endif

#include "qxrdacquisitionpilatus.h"
#include "qxrdacquisitionareadetector.h"
#include "qxrdacquisitionsimulated.h"
#include "qxrddataprocessor.h"
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

QxrdAcquisitionThread::QxrdAcquisitionThread(QxrdDataProcessorPtr proc,
                                             QxrdAllocatorPtr allocator,
                                             int detectorType)
  : QThread(),
    m_Debug(true),
    m_Allocator(NULL),
    m_Acquisition(NULL),
    m_Processor(NULL),
    m_DetectorType(detectorType)
{
  m_Allocator.fetchAndStoreOrdered(allocator);
  m_Processor.fetchAndStoreOrdered(proc);

#ifdef HAVE_PERKIN_ELMER
  HINSTANCE xisllib;

  xisllib = LoadLibrary(L"XISL.dll");

  if (xisllib == NULL) {
    printf("XISL library is not available - cannot use PE detector\n");
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
  QxrdAcquisitionPtr p;

  switch(m_DetectorType) {
  case 0:
  default:
    p = QxrdAcquisitionPtr(new QxrdAcquisitionSimulated(m_Processor, m_Allocator));
    g_DetectorType = 0;
    break;

#ifdef HAVE_PERKIN_ELMER
  case 1:
    if (g_PEAvailable) {
      p = QxrdAcquisitionPtr(new QxrdAcquisitionPerkinElmer(m_Processor, m_Allocator));
      g_DetectorType = 1;
    } else {
      p = QxrdAcquisitionPtr(new QxrdAcquisitionSimulated(m_Processor, m_Allocator));
      g_DetectorType = 0;
    }
    break;
#endif

#ifdef HAVE_PILATUS
  case 2:
    p = QxrdAcquisitionPtr(new QxrdAcquisitionPilatus(m_Processor, m_Allocator));
    g_DetectorType = 2;
    break;
#endif

#ifdef HAVE_AREADETECTOR
  case 3:
    p = QxrdAcquisitionPtr(new QxrdAcquisitionAreaDetector(m_Processor, m_Allocator));
    g_DetectorType = 3;
    break;
#endif
  }


//  m_Processor -> setAcquisition(m_Acquisition);

  m_Acquisition.fetchAndStoreOrdered(p);


  int rc = exec();

  printf("Acquisition thread terminated with rc %d\n", rc);
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
  exit();

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

QxrdAcquisitionPtr QxrdAcquisitionThread::acquisition() const
{
  while (m_Acquisition == NULL) {
    QThread::msleep(500);
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
      << "EPICS Area Detector";

  return res;
}

int QxrdAcquisitionThread::detectorType()
{
  return g_DetectorType;
}
