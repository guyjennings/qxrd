#include "qcepmutexlocker.h"
#include <stdio.h>
#include <QThread>
#include "qcepdebug.h"
#include <QString>
#include <QObject>

QcepMutexLocker::QcepMutexLocker(const char *file, int line, QMutex * mutex)
  : QMutexLocker(mutex),
    m_File(file),
    m_Line(line)
{
  if (qcepDebug(DEBUG_LOCKING)) {
    m_LockTime.start();
    traceLock(file, line);
  }
}

QcepMutexLocker::~QcepMutexLocker()
{
  if (qcepDebug(DEBUG_LOCKING)) {
    if (m_LockTime.elapsed() > 1000) {
      printf("Lock held for %d msec, file %s, line %d, thread %p\n",
             m_LockTime.elapsed(), m_File, m_Line, QThread::currentThread());
    }
  }
}

QMap<QString,int> g_LockMap;

void QcepMutexLocker::traceLock(const char *file, int line)
{
  QString key = QString(file)+":"+QString::number(line);

  g_LockMap[key] += 1;
}

void QcepMutexLocker::dumpLocks()
{
  QMapIterator<QString, int> i(g_LockMap);

  while (i.hasNext()) {
    i.next();

    printf("Key: %s : Count %d\n", qPrintable(i.key()),  i.value());
  }
}
