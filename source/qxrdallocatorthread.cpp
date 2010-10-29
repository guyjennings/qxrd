#include "qxrdallocatorthread.h"

#include "qxrdallocator.h"
#include "qxrdacquisition.h"

QxrdAllocatorThread::QxrdAllocatorThread(/*QxrdAcquisitionPtr acq*/)
  : QThread(),
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

  connect(m_Allocator, SIGNAL(printMessage(QString)), this, SIGNAL(printMessage(QString)));
  connect(m_Allocator, SIGNAL(statusMessage(QString)), this, SIGNAL(statusMessage(QString)));
  connect(m_Allocator, SIGNAL(criticalMessage(QString)), this, SIGNAL(criticalMessage(QString)));

  int rc = exec();

  printf("Allocator thread terminated with rc %d\n", rc);
}

void QxrdAllocatorThread::shutdown()
{
  exit();

  wait();
}

QxrdAllocatorPtr QxrdAllocatorThread::allocator() const
{
  while (m_Allocator == NULL) {
    QThread::msleep(500);
  }

  return m_Allocator;
}
