#ifndef QXRDMUTEXLOCKER_H
#define QXRDMUTEXLOCKER_H

#include "qcepmutexlocker.h"

class QxrdMutexLocker : public QcepMutexLocker
{
public:
  QxrdMutexLocker(const char *file, int line, QMutex * mutex);
};

#endif // QXRDMUTEXLOCKER_H
