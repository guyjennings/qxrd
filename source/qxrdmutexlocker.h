#ifndef QXRDMUTEXLOCKER_H
#define QXRDMUTEXLOCKER_H

#include <QMutexLocker>
#include <QTime>
#include "qcepmacros.h"

class QxrdMutexLocker : public QMutexLocker
{
public:
  QxrdMutexLocker(const char *file, int line, QMutex * mutex);
  ~QxrdMutexLocker();

private:
  QTime m_LockTime;
  const char* m_File;
  int         m_Line;
};

#endif // QXRDMUTEXLOCKER_H
