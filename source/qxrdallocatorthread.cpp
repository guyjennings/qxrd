#include "qxrdallocatorthread.h"

#include "qxrdallocator.h"
#include "qxrdacquisition.h"
#include "qxrdapplication.h"

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

  QxrdAllocatorPtr p(new QxrdAllocator(m_Saver));

  int rc = -1;

  if (p) {
    m_Allocator = p;

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
  while (isRunning() && m_Allocator == NULL) {
    QThread::msleep(50);
  }

  return m_Allocator;
}

void QxrdAllocatorThread::msleep(unsigned long t)
{
  QThread::msleep(t);
}
