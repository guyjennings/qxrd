#include "qxrdallocatorthread.h"

#include "qxrdallocator.h"
#include "qxrdacquisition.h"

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

  connect(m_Allocator, SIGNAL(printMessage(QString,QDateTime)),
          this,        SIGNAL(printMessage(QString,QDateTime)));
  connect(m_Allocator, SIGNAL(statusMessage(QString,QDateTime)),
          this,        SIGNAL(statusMessage(QString,QDateTime)));
  connect(m_Allocator, SIGNAL(criticalMessage(QString,QDateTime)),
          this,        SIGNAL(criticalMessage(QString,QDateTime)));

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
    QThread::msleep(50);
  }

  return m_Allocator;
}

void QxrdAllocatorThread::msleep(unsigned long t)
{
  QThread::msleep(t);
}
