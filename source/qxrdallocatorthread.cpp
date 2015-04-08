#include "qxrddebug.h"
#include "qxrdallocatorthread.h"

#include "qxrdallocator.h"
#include "qxrdacquisition.h"
#include "qxrdapplication.h"
#include "qxrdmutexlocker.h"

QxrdAllocatorThread::QxrdAllocatorThread(QxrdSettingsSaverPtr saver)
  : QxrdThread(),
    m_Allocator(NULL),
    m_Saver(saver)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAllocatorThread::QxrdAllocatorThread(%p)\n", this);
  }
}

QxrdAllocatorThread::~QxrdAllocatorThread()
{
  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAllocatorThread::~QxrdAllocatorThread(%p)\n", this);
  }
}

void QxrdAllocatorThread::run()
{
  if (g_Application && qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage("Starting Allocator Thread");
  }

  QxrdAllocatorPtr p(new QxrdAllocator(m_Saver), doDeleteLater);

  int rc = -1;

  if (p) {
    m_Mutex.lock();

    m_Allocator = p;

    m_Mutex.unlock();

    rc = exec();
  }

  if (g_Application && qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage(tr("Allocator Thread Terminated with rc %1").arg(rc));
  }
}

void QxrdAllocatorThread::shutdown()
{
  exit();

  wait();
}

QxrdAllocatorPtr QxrdAllocatorThread::allocator() const
{

  while (isRunning()) {
    {
      QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);
      if (m_Allocator) return m_Allocator;
    }

    QThread::msleep(50);
  }

  return QxrdAllocatorPtr();
}

void QxrdAllocatorThread::msleep(unsigned long t)
{
  QThread::msleep(t);
}
