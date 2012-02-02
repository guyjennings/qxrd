#ifndef QXRDIMAGEDATAOBJECTCOUNTER_H
#define QXRDIMAGEDATAOBJECTCOUNTER_H

#include <QObject>
#include <QSharedPointer>

class QxrdAllocator;
typedef QSharedPointer<QxrdAllocator> QxrdAllocatorPtr;

class QxrdImageDataObjectCounter {
public:
  QxrdImageDataObjectCounter(QxrdAllocatorPtr alloc, int typ);
  ~QxrdImageDataObjectCounter();

  int value();

  void allocate(int sz, int width, int height);
  int allocatedMemoryMB();

private:
  QxrdAllocatorPtr m_Allocator;
  quint64          m_Allocated;
  int              m_Type;
};

#endif // QXRDIMAGEDATAOBJECTCOUNTER_H
