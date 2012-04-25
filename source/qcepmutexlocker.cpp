#include "qcepmutexlocker.h"
#include <stdio.h>
#include <QThread>

QcepMutexLocker::QcepMutexLocker(const char *file, int line, QMutex * mutex)
  : QMutexLocker(mutex),
    m_File(file),
    m_Line(line)
{
  m_LockTime.start();
}

QcepMutexLocker::~QcepMutexLocker()
{
  if (m_LockTime.elapsed() > 1000) {
    printf("Lock held for %d msec, file %s, line %d, thread %p\n",
           m_LockTime.elapsed(), m_File, m_Line, QThread::currentThread());
  }
}
