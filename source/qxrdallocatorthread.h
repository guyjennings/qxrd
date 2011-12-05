#ifndef QXRDALLOCATORTHREAD_H
#define QXRDALLOCATORTHREAD_H

#include "qxrdthread.h"
#include <QDateTime>
#include "qxrdsettingssaver.h"

class QxrdAllocator;

class QxrdAllocatorThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdAllocatorThread(QxrdSettingsSaver *saver);
  ~QxrdAllocatorThread();

  void shutdown();

  QxrdAllocator *allocator() const;

  static void msleep(long unsigned int);

protected:
  void run();

private:
  QAtomicPointer<QxrdAllocator>     m_Allocator;
  QxrdSettingsSaver                *m_Saver;
};

#endif // QXRDALLOCATORTHREAD_H
