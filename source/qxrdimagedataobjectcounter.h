#ifndef QXRDIMAGEDATAOBJECTCOUNTER_H
#define QXRDIMAGEDATAOBJECTCOUNTER_H

#include "qxrdallocator-ptr.h"

class QxrdImageDataObjectCounter {
public:
  QxrdImageDataObjectCounter(QxrdAllocatorWPtr alloc, int typ);
  ~QxrdImageDataObjectCounter();

  int value();

  void allocate(int sz, int width, int height);
  int allocatedMemoryMB();

  QxrdAllocatorWPtr allocator();

private:
  QxrdAllocatorWPtr m_Allocator;
  quint64           m_Allocated;
  int               m_Type;
};

#endif // QXRDIMAGEDATAOBJECTCOUNTER_H
