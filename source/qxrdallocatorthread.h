#ifndef QXRDALLOCATORTHREAD_H
#define QXRDALLOCATORTHREAD_H

#include "qcepmacros.h"

#include <QThread>
#include "qxrdforwardtypes.h"
#include "qxrdallocator.h"

class QxrdAllocatorThread : public QThread
{
  Q_OBJECT;

public:
  QxrdAllocatorThread();
  ~QxrdAllocatorThread();

  void shutdown();

  QxrdAllocatorPtr allocator() const;

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
