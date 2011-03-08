#include "qxrdallocator.h"
#include "qxrdmutexlocker.h"
#include "qxrdallocatorthread.h"

QxrdAllocator::QxrdAllocator
    (QObject *parent)
  : QxrdAllocatorInterface(parent),
    m_Max(this, "max", 800),
    m_Allocated(this, "allocated", 0),
    m_Width(this, "width", 2048),
    m_Height(this, "height", 2048)
{
  if (qcepDebug(DEBUG_ALLOCATOR)) {
    printf("allocator %p constructed\n", this);
  };

  connect(&m_Timer, SIGNAL(timeout()), this, SLOT(allocatorHeartbeat()));

  m_Timer.start(100);

  allocatorHeartbeat();
}

QxrdAllocator::~QxrdAllocator()
{
  if (qcepDebug(DEBUG_ALLOCATOR)) {
    printf("allocator %p deleted\n", this);
  };
}

QxrdInt16ImageDataPtr QxrdAllocator::newInt16Image()
{
//  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if ((m_AllocatedMemoryMB + int16SizeMB()) < get_Max()) {
//    m_CountInt16.fetchAndAddOrdered(1);

    QxrdInt16ImageDataPtr res(new QxrdInt16ImageData(this, get_Width(), get_Height()), &QxrdAllocator::int16Deleter);

    res->moveToThread(thread());

    if (qcepDebug(DEBUG_ALLOCATOR)) {
      printf("QxrdAllocator::newInt16Image() succeeded [%p] [%d]\n", res.data(), (int) m_AllocatedMemoryMB);
    }

    return res;
  } else {
    if (qcepDebug(DEBUG_ALLOCATOR)) {
      printf("QxrdAllocator::newInt16Image() returned NULL\n");
    }

    return QxrdInt16ImageDataPtr(NULL);
  }
}

QxrdInt32ImageDataPtr QxrdAllocator::newInt32Image()
{
//  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if ((m_AllocatedMemoryMB + int32SizeMB()) < get_Max()) {
//    m_CountInt32.fetchAndAddOrdered(1);

    QxrdInt32ImageDataPtr res(new QxrdInt32ImageData(this, get_Width(), get_Height()), &QxrdAllocator::int32Deleter);

    res->moveToThread(thread());

    if (qcepDebug(DEBUG_ALLOCATOR)) {
      printf("QxrdAllocator::newInt32Image() succeeded [%p] [%d]\n", res.data(), (int) m_AllocatedMemoryMB);
    }

    return res;
  } else {
    if (qcepDebug(DEBUG_ALLOCATOR)) {
      printf("QxrdAllocator::newInt32Image() returned NULL\n");
    }

    return QxrdInt32ImageDataPtr(NULL);
  }
}

QxrdDoubleImageDataPtr QxrdAllocator::newDoubleImage()
{
//  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

//  m_CountDouble.fetchAndAddOrdered(1);

  while ((m_AllocatedMemoryMB + doubleSizeMB()) > get_Max()) {
    if (qcepDebug(DEBUG_ALLOCATOR)) {
      printf("QxrdAllocator::newDoubleImage() sleeping 100msec\n");
    }

    QxrdAllocatorThread::msleep(100);
  }

  QxrdDoubleImageDataPtr res(new QxrdDoubleImageData(this, get_Width(), get_Height()), &QxrdAllocator::doubleDeleter);

  res->moveToThread(thread());

  if (qcepDebug(DEBUG_ALLOCATOR)) {
    printf("QxrdAllocator::newDoubleImage() succeeded [%p] [%d]\n", res.data(), (int) m_AllocatedMemoryMB);
  }

  return res;
}

QxrdMaskDataPtr QxrdAllocator::newMask(int def)
{
//  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  while ((m_AllocatedMemoryMB + maskSizeMB()) > get_Max()) {
    if (qcepDebug(DEBUG_ALLOCATOR)) {
      printf("QxrdAllocator::newMask() sleeping 100msec\n");
    }

    QxrdAllocatorThread::msleep(100);
  }

  QxrdMaskDataPtr res(new QxrdMaskData(this, get_Width(), get_Height(), def), &QxrdAllocator::maskDeleter);

  res->moveToThread(thread());

  if (qcepDebug(DEBUG_ALLOCATOR)) {
    printf("QxrdAllocator::newMask() succeeded [%p] [%d]\n", res.data(), (int) m_AllocatedMemoryMB);
  }

  return res;
}

QxrdIntegratedDataPtr QxrdAllocator::newIntegratedData(QxrdDoubleImageDataPtr data)
{
//  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  while ((m_AllocatedMemoryMB + integratedSizeMB(10000)) > get_Max()) {
    if (qcepDebug(DEBUG_ALLOCATOR)) {
      printf("QxrdAllocator::newIntegratedData() sleeping 100msec\n");
    }

    QxrdAllocatorThread::msleep(100);
  }

  QxrdIntegratedDataPtr res(new QxrdIntegratedData(this, data, 10000), &QxrdAllocator::integratedDeleter);

  res->moveToThread(thread());

  if (qcepDebug(DEBUG_ALLOCATOR)) {
    printf("QxrdAllocator::newIntegratedData() succeeded [%p] [%d]\n", res.data(), (int) m_AllocatedMemoryMB);
  }

  return res;
}

void QxrdAllocator::dimension(int width, int height)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  set_Width(width);
  set_Height(height);
}

void QxrdAllocator::allocate(int sz, int width, int height)
{
  allocate((quint64) sz*width*height);
}

void QxrdAllocator::allocate(quint64 amt)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_AllocatedMemory += amt;

  m_AllocatedMemoryMB.fetchAndStoreOrdered(m_AllocatedMemory/MegaBytes);
}

void QxrdAllocator::deallocate(int sz, int width, int height)
{
  deallocate((quint64) sz*width*height);
}

void QxrdAllocator::deallocate(quint64 amt)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_AllocatedMemory -= amt;

  m_AllocatedMemoryMB.fetchAndStoreOrdered(m_AllocatedMemory/MegaBytes);
}

void QxrdAllocator::maskDeleter(QxrdMaskData *mask)
{
//    delete mask;
  if (qcepDebug(DEBUG_ALLOCATOR)) {
    printf("QxrdAllocator::maskDeleter deleteLater %p [%d]\n", mask, mask->allocatedMemoryMB());
  }

  mask->deleteLater();
}

void QxrdAllocator::int16Deleter(QxrdInt16ImageData *img)
{
//    delete img;
  if (qcepDebug(DEBUG_ALLOCATOR)) {
    printf("QxrdAllocator::int16Deleter deleteLater %p [%d]\n", img, img->allocatedMemoryMB());
  }

  img->deleteLater();
}

void QxrdAllocator::int32Deleter(QxrdInt32ImageData *img)
{
//    delete img;
  if (qcepDebug(DEBUG_ALLOCATOR)) {
    printf("QxrdAllocator::int32Deleter deleteLater %p [%d]\n", img, img->allocatedMemoryMB());
  }

  img->deleteLater();
}

void QxrdAllocator::doubleDeleter(QxrdDoubleImageData *img)
{
//    delete img;
  if (qcepDebug(DEBUG_ALLOCATOR)) {
    printf("QxrdAllocator::doubleDeleter deleteLater %p [%d]\n", img, img->allocatedMemoryMB());
  }

  img->deleteLater();
}

void QxrdAllocator::integratedDeleter(QxrdIntegratedData *img)
{
//    delete img;
  if (qcepDebug(DEBUG_ALLOCATOR)) {
    printf("QxrdAllocator::integratedDeleter deleteLater %p [%d]\n", img, img->allocatedMemoryMB());
  }

  img->deleteLater();
}

int QxrdAllocator::int16SizeMB()
{
  return sizeof(quint16)*get_Width()*get_Height()/MegaBytes;
}

int QxrdAllocator::int32SizeMB()
{
  return sizeof(quint32)*get_Width()*get_Height()/MegaBytes;
}

int QxrdAllocator::doubleSizeMB()
{
  return sizeof(double)*get_Width()*get_Height()/MegaBytes;
}

int QxrdAllocator::maskSizeMB()
{
  return sizeof(quint16)*get_Width()*get_Height()/MegaBytes;
}

int QxrdAllocator::integratedSizeMB(int nrows)
{
  return (sizeof(double)*4*nrows)/MegaBytes;
}

void QxrdAllocator::allocatorHeartbeat()
{
//  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

//  int n16 = m_FreeInt16Images.size();
//  int n32 = m_FreeInt32Images.size();
//  int ndbl = m_FreeDoubleImages.size();
//  int nmsk = m_FreeMasks.size();
//  int nint = m_FreeIntegratedData.size();

//  int n16needed = m_PreallocateInt16.fetchAndAddOrdered(0);
//  int n32needed = m_PreallocateInt32.fetchAndAddOrdered(0);
//  int ndblneeded = m_PreallocateDouble.fetchAndAddOrdered(0);

//  int n16used = m_CountInt16.fetchAndStoreOrdered(0);
//  int n32used = m_CountInt32.fetchAndStoreOrdered(0);
//  int ndblused = m_CountDouble.fetchAndStoreOrdered(0);

//  if (n16used > n16needed) n16needed = n16used;
//  if (n32used > n32needed) n32needed = n32used;
//  if (ndblused > ndblneeded) ndblneeded = ndblused;

//  if (n16needed < 4) n16needed = 4;
//  if (n32needed < 4) n32needed = 4;
//  if (ndblneeded < 4) ndblneeded = 4;

//  if (qcepDebug(DEBUG_ALLOCATOR)) {
//    emit printMessage(QDateTime::currentDateTime(),
//                      tr("Allocator heartbeat, %1 out of %2, [%3,%4,%5,%6] [%7,%8,%9]\n")
//                      .arg(m_AllocatedMemoryMB).arg(get_Max()).arg(n16).arg(n32).arg(ndbl).arg(nmsk)
//                      .arg(n16needed).arg(n32needed).arg(ndblneeded));
//  }

//  for (int i=ndbl; i<ndblneeded; i++) {
//    if (m_AllocatedMemory/MegaBytes < get_Max()) {
//      try {
//        m_FreeDoubleImages.enqueue(QxrdDoubleImageDataPtr(new QxrdDoubleImageData(this, get_Width(), get_Height()), &QxrdAllocator::doubleDeleter));
//      }
//      catch (...) {
//        return;
//      }
//    }
//  }

//  for (int i=n32; i<n32needed; i++) {
//    if (m_AllocatedMemory/MegaBytes < get_Max()) {
//      try {
//        m_FreeInt32Images.enqueue(QxrdInt32ImageDataPtr(new QxrdInt32ImageData(this, get_Width(), get_Height()), &QxrdAllocator::int32Deleter));
//      }
//      catch (...) {
//        return;
//      }
//    }
//  }

//  for (int i=n16; i<n16needed; i++) {
//    if (m_AllocatedMemory/MegaBytes < get_Max()) {
//      try {
//        m_FreeInt16Images.enqueue(QxrdInt16ImageDataPtr(new QxrdInt16ImageData(this, get_Width(), get_Height()), &QxrdAllocator::int16Deleter));
//      }
//      catch (...) {
//        return;
//      }
//    }
//  }

//  for (int i=nmsk; i<4; i++) {
//    if (m_AllocatedMemory/MegaBytes < get_Max()) {
//      try {
//        m_FreeMasks.enqueue(QxrdMaskDataPtr(new QxrdMaskData(this, get_Width(), get_Height()), &QxrdAllocator::maskDeleter));
//      }
//      catch (...) {
//        return;
//      }
//    }
//  }

//  for (int i=nint; i<4; i++) {
//    if (m_AllocatedMemory/MegaBytes < get_Max()) {
//      try {
//        m_FreeIntegratedData.enqueue(QxrdIntegratedDataPtr(new QxrdIntegratedData(this, QxrdDoubleImageDataPtr(), 10000), &QxrdAllocator::integratedDeleter));
//      }
//      catch (...) {
//        return;
//      }
//    }
//  }

  set_Allocated(m_AllocatedMemoryMB);
}

double QxrdAllocator::allocatedMemoryMB()
{
  return m_AllocatedMemoryMB;
}

double QxrdAllocator::allocatedMemory()
{
  return m_AllocatedMemoryMB*MegaBytes;
}

double QxrdAllocator::maximumMemoryMB()
{
  return get_Max();
}

double QxrdAllocator::maximumMemory()
{
  return get_Max()*MegaBytes;
}

void QxrdAllocator::changedSizeMB(int newMB)
{
  set_Max(newMB);
}
