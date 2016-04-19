#include "qcepdebug.h"
#include "qcepallocatorthread.h"

#include "qcepallocator.h"
#include "qcepapplication.h"
#include "qcepmutexlocker.h"
#include <stdio.h>

QcepAllocatorThread::QcepAllocatorThread(QcepSettingsSaverWPtr saver)
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

  m_Allocator = QcepAllocatorPtr(new QcepAllocator(m_Saver));

  int rc = exec();

  m_Allocator = QcepAllocatorPtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Allocator Thread Terminated with rc %d\n", rc);
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
    if (m_Allocator) return m_Allocator;

    QThread::msleep(50);
  }

  return QcepAllocatorPtr();
}
