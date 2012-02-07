#include "qxrdimagedataobjectcounter.h"
#include "qxrdallocator.h"

static QAtomicInt s_ImageDataObjectCounter;

QxrdImageDataObjectCounter::QxrdImageDataObjectCounter(QxrdAllocatorWPtr alloc, int typ)
  : m_Allocator(alloc),
    m_Allocated(0),
    m_Type(typ)
{
  s_ImageDataObjectCounter.fetchAndAddOrdered(1);
}

QxrdImageDataObjectCounter::~QxrdImageDataObjectCounter()
{
  QxrdAllocatorPtr alloc = m_Allocator.toStrongRef();

  if (alloc) {
    alloc->deallocate(m_Type, m_Allocated);
  }

  s_ImageDataObjectCounter.fetchAndAddOrdered(-1);
}

int QxrdImageDataObjectCounter::value()
{
  return s_ImageDataObjectCounter.fetchAndAddOrdered(0);
}

void QxrdImageDataObjectCounter::allocate(int sz, int width, int height)
{
  QxrdAllocatorPtr alloc = m_Allocator.toStrongRef();

  if (alloc) {
    alloc->allocate(m_Type, sz, width, height);
    m_Allocated += sz*width*height;
  }
}

int QxrdImageDataObjectCounter::allocatedMemoryMB()
{
  QxrdAllocatorPtr alloc = m_Allocator.toStrongRef();

  if (alloc) {
    return alloc->allocatedMemoryMB();
  } else {
    return -1;
  }
}
