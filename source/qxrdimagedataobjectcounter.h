#ifndef QXRDIMAGEDATAOBJECTCOUNTER_H
#define QXRDIMAGEDATAOBJECTCOUNTER_H

#include <QObject>
#include <QSharedPointer>

class QxrdAllocator;
typedef QWeakPointer<QxrdAllocator> QxrdAllocatorWPtr;

class QxrdImageDataObjectCounter {
public:
  QxrdImageDataObjectCounter(QxrdAllocatorWPtr alloc, int typ);
  ~QxrdImageDataObjectCounter();

  int value();

  void allocate(int sz, int width, int height);
  int allocatedMemoryMB();

private:
  QxrdAllocatorWPtr m_Allocator;
  quint64           m_Allocated;
  int               m_Type;
};

#endif // QXRDIMAGEDATAOBJECTCOUNTER_H
