#include "qcepdebug.h"
#include "qcepallocatorthread.h"

#include "qcepallocator.h"
#include "qcepapplication.h"
#include "qcepmutexlocker.h"
#include <stdio.h>

QcepAllocatorThread::QcepAllocatorThread(QcepSettingsSaverPtr saver)
  : QcepThread(),
    m_Allocator(NULL),
    m_Saver(saver)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QcepAllocatorThread::QcepAllocatorThread(%p)\n", this);
  }
}

QcepAllocatorThread::~QcepAllocatorThread()
{
  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QcepAllocatorThread::~QcepAllocatorThread(%p)\n", this);
  }
}

void QcepAllocatorThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printf("Starting Allocator Thread\n");
  }

  QcepAllocatorPtr p(new QcepAllocator(m_Saver));

  int rc = -1;

  if (p) {
    m_Mutex.lock();

    m_Allocator = p;

    m_Mutex.unlock();

    rc = exec();
  }

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Allocator Thread Terminated with rc %d\n", rc);
  }

  {
    QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    m_Allocator = QcepAllocatorPtr();
  }
}

void QcepAllocatorThread::shutdown()
{
  exit();

  wait();
}

QcepAllocatorPtr QcepAllocatorThread::allocator() const
{

  while (isRunning()) {
    {
      QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);
      if (m_Allocator) return m_Allocator;
    }

    QThread::msleep(50);
  }

  return QcepAllocatorPtr();
}
