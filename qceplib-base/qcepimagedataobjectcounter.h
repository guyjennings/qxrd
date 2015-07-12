#ifndef QCEPIMAGEDATAOBJECTCOUNTER_H
#define QCEPIMAGEDATAOBJECTCOUNTER_H

#include "qcepallocator-ptr.h"

class QcepImageDataObjectCounter {
public:
  QcepImageDataObjectCounter(QcepAllocatorWPtr alloc, int typ);
  ~QcepImageDataObjectCounter();

  int value();

  void allocate(int sz, int width, int height);
  int allocatedMemoryMB();

  QcepAllocatorWPtr allocator();

private:
  QcepAllocatorWPtr m_Allocator;
  quint64           m_Allocated;
  int               m_Type;
};

#endif // QCEPIMAGEDATAOBJECTCOUNTER_H
