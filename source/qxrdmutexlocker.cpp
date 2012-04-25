#include "qxrdmutexlocker.h"
#include <stdio.h>
#include <QThread>
#include "qxrdapplication.h"

QxrdMutexLocker::QxrdMutexLocker(const char *file, int line, QMutex * mutex)
  : QcepMutexLocker(file, line, mutex)
{
  if (g_Application) {
    g_Application->incLockerCount();
  }
}
