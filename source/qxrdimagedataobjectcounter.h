#ifndef QXRDIMAGEDATAOBJECTCOUNTER_H
#define QXRDIMAGEDATAOBJECTCOUNTER_H

#include <QObject>

class QxrdAllocatorInterface;

class QxrdImageDataObjectCounter {
public:
  QxrdImageDataObjectCounter(QxrdAllocatorInterface *alloc);
  ~QxrdImageDataObjectCounter();

  int value();

  void allocate(int sz, int width, int height);
  int allocatedMemoryMB();

private:
  QxrdAllocatorInterface *m_Allocator;
  quint64                 m_Allocated;
};

#endif // QXRDIMAGEDATAOBJECTCOUNTER_H
