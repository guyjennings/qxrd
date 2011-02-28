#ifndef QXRDALLOCATORTHREAD_H
#define QXRDALLOCATORTHREAD_H

#include <QThread>
#include <QDateTime>
class QxrdAllocator;

class QxrdAllocatorThread : public QThread
{
  Q_OBJECT;

public:
  QxrdAllocatorThread();
  ~QxrdAllocatorThread();

  void shutdown();

  QxrdAllocator *allocator() const;

  static void msleep(long unsigned int);

signals:
  void printMessage(QDateTime ts, QString msg);
  void statusMessage(QDateTime ts, QString msg);
  void criticalMessage(QDateTime ts, QString msg);

protected:
  void run();

private:
  QAtomicPointer<QxrdAllocator>     m_Allocator;
};

#endif // QXRDALLOCATORTHREAD_H
