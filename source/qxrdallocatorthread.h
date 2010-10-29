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
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

protected:
  void run();

private:
  QAtomicPointer<QxrdAllocator>     m_Allocator;
};

#endif // QXRDALLOCATORTHREAD_H
