#include "qxrdallocatorthread.h"

#include "qxrdallocator.h"
#include "qxrdacquisition.h"
#include "qxrdapplication.h"

QxrdAllocatorThread::QxrdAllocatorThread(/*QxrdAcquisitionPtr acq*/)
  : QxrdThread(),
    m_Allocator(NULL)
{
}

QxrdAllocatorThread::~QxrdAllocatorThread()
{
  shutdown();

  delete m_Allocator;
}

void QxrdAllocatorThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage("Starting Allocator Thread");
  }

  m_Allocator.fetchAndStoreOrdered(new QxrdAllocator(/*m_Acquisition*/));

  int rc = exec();

  if (qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage(tr("Allocator Thread Terminated with rc %1").arg(rc));
  }
}

void QxrdAllocatorThread::shutdown()
{
  exit();

  wait();
}

QxrdAllocator *QxrdAllocatorThread::allocator() const
{
  while (m_Allocator == NULL) {
    QThread::msleep(50);
  }

  return m_Allocator;
}

void QxrdAllocatorThread::msleep(unsigned long t)
{
  QThread::msleep(t);
}
