#ifndef QXRDALLOCATORTHREAD_H
#define QXRDALLOCATORTHREAD_H

#include "qxrdthread.h"
#include <QDateTime>
#include "qxrdsettingssaver.h"
#include "qxrdallocator.h"

class QxrdAllocatorThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdAllocatorThread(QxrdSettingsSaverPtr saver);
  ~QxrdAllocatorThread();

  void shutdown();

  QxrdAllocatorPtr allocator() const;

  static void msleep(long unsigned int);

protected:
  void run();

private:
  QxrdAllocatorPtr     m_Allocator;
  QxrdSettingsSaverPtr m_Saver;
};

typedef QSharedPointer<QxrdAllocatorThread> QxrdAllocatorThreadPtr;

#endif // QXRDALLOCATORTHREAD_H
