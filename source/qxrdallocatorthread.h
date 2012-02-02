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
  QxrdAllocatorThread(QxrdSettingsSaver *saver);
  ~QxrdAllocatorThread();

  void shutdown();

  QxrdAllocatorPtr allocator() const;

  static void msleep(long unsigned int);

protected:
  void run();

private:
  QxrdAllocatorPtr    m_Allocator;
  QxrdSettingsSaver  *m_Saver;
};

typedef QSharedPointer<QxrdAllocatorThread> QxrdAllocatorThreadPtr;

#endif // QXRDALLOCATORTHREAD_H
