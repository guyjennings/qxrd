#include "qxrdimagedataobjectcounter.h"
#include "qxrdallocatorinterface.h"

static QAtomicInt s_ImageDataObjectCounter;

QxrdImageDataObjectCounter::QxrdImageDataObjectCounter(QxrdAllocatorInterface *alloc)
  : m_Allocator(alloc),
    m_Allocated(0)
{
  s_ImageDataObjectCounter.fetchAndAddOrdered(1);
}

QxrdImageDataObjectCounter::~QxrdImageDataObjectCounter()
{
  if (m_Allocator) {
    m_Allocator->deallocate(m_Allocated);
  }

  s_ImageDataObjectCounter.fetchAndAddOrdered(-1);
}

int QxrdImageDataObjectCounter::value()
{
  return s_ImageDataObjectCounter.fetchAndAddOrdered(0);
}

void QxrdImageDataObjectCounter::allocate(int sz, int width, int height)
{
  m_Allocator->allocate(sz, width, height);
  m_Allocated += sz*width*height;
}

int QxrdImageDataObjectCounter::allocatedMemoryMB()
{
  if (m_Allocator) {
    return m_Allocator->allocatedMemoryMB();
  } else {
    return -1;
  }
}
