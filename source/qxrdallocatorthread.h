#ifndef QXRDALLOCATORTHREAD_H
#define QXRDALLOCATORTHREAD_H

#include "qxrdthread.h"
#include <QDateTime>
class QxrdAllocator;

class QxrdAllocatorThread : public QxrdThread
{
  Q_OBJECT;

public:
  QxrdAllocatorThread();
  ~QxrdAllocatorThread();

  void shutdown();

  QxrdAllocator *allocator() const;

  static void msleep(long unsigned int);

signals:
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

protected:
  void run();

private:
  QAtomicPointer<QxrdAllocator>     m_Allocator;
};

#endif // QXRDALLOCATORTHREAD_H
