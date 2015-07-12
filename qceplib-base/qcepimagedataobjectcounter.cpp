#include "qcepimagedataobjectcounter.h"
#include "qcepallocator.h"

static QAtomicInt s_ImageDataObjectCounter;

QcepImageDataObjectCounter::QcepImageDataObjectCounter(QcepAllocatorWPtr alloc, int typ)
  : m_Allocator(alloc),
    m_Allocated(0),
    m_Type(typ)
{
  s_ImageDataObjectCounter.fetchAndAddOrdered(1);
}

QcepImageDataObjectCounter::~QcepImageDataObjectCounter()
{
  QcepAllocatorPtr alloc(m_Allocator);

  if (alloc) {
    alloc->deallocate(m_Type, m_Allocated);
  }

  s_ImageDataObjectCounter.fetchAndAddOrdered(-1);
}

int QcepImageDataObjectCounter::value()
{
  return s_ImageDataObjectCounter.fetchAndAddOrdered(0);
}

void QcepImageDataObjectCounter::allocate(int sz, int width, int height)
{
  QcepAllocatorPtr alloc(m_Allocator);

  if (alloc) {
    alloc->allocate(m_Type, sz, width, height);
    m_Allocated += sz*width*height;
  }
}

int QcepImageDataObjectCounter::allocatedMemoryMB()
{
  QcepAllocatorPtr alloc(m_Allocator);

  if (alloc) {
    return alloc->allocatedMemoryMB();
  } else {
    return -1;
  }
}

QcepAllocatorWPtr QcepImageDataObjectCounter::allocator()
{
  return m_Allocator;
}
