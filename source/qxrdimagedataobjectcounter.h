#ifndef QXRDIMAGEDATAOBJECTCOUNTER_H
#define QXRDIMAGEDATAOBJECTCOUNTER_H

#include <QObject>

class QxrdAllocatorInterface;

class QxrdImageDataObjectCounter {
public:
  QxrdImageDataObjectCounter(QxrdAllocatorInterface *alloc, int typ);
  ~QxrdImageDataObjectCounter();

  int value();

  void allocate(int sz, int width, int height);
  int allocatedMemoryMB();

private:
  QxrdAllocatorInterface *m_Allocator;
  quint64                 m_Allocated;
  int                     m_Type;
};

#endif // QXRDIMAGEDATAOBJECTCOUNTER_H
