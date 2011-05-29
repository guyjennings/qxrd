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
  m_Allocator.fetchAndStoreOrdered(new QxrdAllocator(/*m_Acquisition*/));

  if (qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage("Starting Allocator Thread");
  }

  int rc = exec();

  if (rc || qcepDebug(DEBUG_THREADS)) {
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
