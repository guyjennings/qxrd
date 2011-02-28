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

  connect(m_Allocator, SIGNAL(printMessage(QDateTime,QString)),
          this,        SIGNAL(printMessage(QDateTime,QString)));
  connect(m_Allocator, SIGNAL(statusMessage(QDateTime,QString)),
          this,        SIGNAL(statusMessage(QDateTime,QString)));
  connect(m_Allocator, SIGNAL(criticalMessage(QDateTime,QString)),
          this,        SIGNAL(criticalMessage(QDateTime,QString)));

  int rc = exec();

//  printf("Allocator thread terminated with rc %d\n", rc);
}

void QxrdAllocatorThread::shutdown()
{
  exit();

  wait();
}

QxrdAllocator *QxrdAllocatorThread::allocator() const
{
  while (m_Allocator == NULL) {
    QThread::msleep(500);
  }

  return m_Allocator;
}

void QxrdAllocatorThread::msleep(unsigned long t)
{
  QThread::msleep(t);
}
