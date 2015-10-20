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
  if (g_Application && qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage("Starting Allocator Thread");
  }

  QcepAllocatorPtr p(new QcepAllocator(m_Saver));

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
