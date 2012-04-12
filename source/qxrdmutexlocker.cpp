#include "qxrdmutexlocker.h"
#include <stdio.h>
#include <QThread>
#include "qxrdapplication.h"

QxrdMutexLocker::QxrdMutexLocker(const char *file, int line, QMutex * mutex)
  : QMutexLocker(mutex),
    m_File(file),
    m_Line(line)
{
  m_LockTime.start();

  if (g_Application) {
    g_Application->incLockerCount();
  }
}

QxrdMutexLocker::~QxrdMutexLocker()
{
  if (m_LockTime.elapsed() > 100) {
    printf("Lock held for %d msec, file %s, line %d, thread %p\n",
           m_LockTime.elapsed(), m_File, m_Line, QThread::currentThread());
  }
}
