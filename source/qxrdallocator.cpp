/******************************************************************
*
*  $Id: qxrdallocator.cpp,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#include "qxrdallocator.h"
#include "qxrdmutexlocker.h"

//#include "qxrdacquisition.h"

QxrdAllocator::QxrdAllocator
    (/*QxrdAcquisitionPtr acq,*/ QObject *parent)
  : QxrdAllocatorInterface(parent),
//    m_Acquisition(acq),
    m_FreeInt16Images("int16 allocator"),
    m_FreeInt32Images("int32 allocator"),
    m_FreeDoubleImages("double allocator"),
    m_FreeMasks("mask allocator"),
    m_FreeIntegratedData("integrated allocator"),
    m_Max(this, "max", 500),
    m_Allocated(this, "allocated", 0),
    m_Width(this, "width", 2048),
    m_Height(this, "height", 2048),
    SOURCE_IDENT("$Id: qxrdallocator.cpp,v 1.2 2010/09/13 20:00:39 jennings Exp $")
{
  QCEP_DEBUG(DEBUG_ALLOCATOR,
             printf("allocator %p constructed\n", this);
  );

  connect(&m_Timer, SIGNAL(timeout()), this, SLOT(allocatorHeartbeat()));

  m_Timer.start(100);

  allocatorHeartbeat();
}

QxrdAllocator::~QxrdAllocator()
{
  QCEP_DEBUG(DEBUG_ALLOCATOR,
             printf("allocator %p deleted\n", this);
  );
}

QxrdInt16ImageDataPtr QxrdAllocator::newInt16Image()
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_CountInt16.fetchAndAddOrdered(1);

  if (m_FreeInt16Images.size() > 0) {
    return m_FreeInt16Images.dequeue();
  } else {
    if ((m_AllocatedMemory/MegaBytes + int16SizeMB()) < get_Max()) {
      return QxrdInt16ImageDataPtr(new QxrdInt16ImageData(this, get_Width(), get_Height()), &QxrdAllocator::int16Deleter);
    } else {
      return QxrdInt16ImageDataPtr(NULL);
    }
  }
}

QxrdInt32ImageDataPtr QxrdAllocator::newInt32Image()
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_CountInt32.fetchAndAddOrdered(1);

  if (m_FreeInt32Images.size() > 0) {
    return m_FreeInt32Images.dequeue();
  } else {
    if ((m_AllocatedMemory/MegaBytes + int32SizeMB()) < get_Max()) {
      return QxrdInt32ImageDataPtr(new QxrdInt32ImageData(this, get_Width(), get_Height()), &QxrdAllocator::int32Deleter);
    } else {
      return QxrdInt32ImageDataPtr(NULL);
    }
  }
}

QxrdDoubleImageDataPtr QxrdAllocator::newDoubleImage()
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_CountDouble.fetchAndAddOrdered(1);

  if (m_FreeDoubleImages.size() > 0) {
    return m_FreeDoubleImages.dequeue();
  } else {
//    if ((m_AllocatedMemory/MegaBytes + doubleSizeMB()) < get_Max()) {
      return QxrdDoubleImageDataPtr(new QxrdDoubleImageData(this, get_Width(), get_Height()), &QxrdAllocator::doubleDeleter);
//    } else {
//      return QxrdDoubleImageDataPtr(NULL);
//    }
  }
}

QxrdMaskDataPtr QxrdAllocator::newMask()
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (m_FreeMasks.size() > 0) {
    return m_FreeMasks.dequeue();
  } else {
    return QxrdMaskDataPtr(new QxrdMaskData(this, get_Width(), get_Height()), &QxrdAllocator::maskDeleter);
  }
}

QxrdIntegratedDataPtr QxrdAllocator::newIntegratedData(QxrdDoubleImageDataPtr data)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (m_FreeIntegratedData.size() > 0) {
    QxrdIntegratedDataPtr res = m_FreeIntegratedData.dequeue();

    if (res) {
      res -> set_Image(data);
    }

    return res;
  } else {
    return QxrdIntegratedDataPtr(new QxrdIntegratedData(this, data, 10000), &QxrdAllocator::integratedDeleter);
  }
}

void QxrdAllocator::dimension(int width, int height)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  set_Width(width);
  set_Height(height);
}

void QxrdAllocator::preallocateInt16(int n16)
{
  m_PreallocateInt16.fetchAndStoreOrdered(n16);
}

void QxrdAllocator::preallocateInt32(int n32)
{
  m_PreallocateInt32.fetchAndStoreOrdered(n32);
}

void QxrdAllocator::preallocateDouble(int ndbl)
{
  m_PreallocateDouble.fetchAndStoreOrdered(ndbl);
}

void QxrdAllocator::allocate(int sz, int width, int height)
{
  m_AllocatedMemory.fetchAndAddOrdered(sz*width*height);
}

void QxrdAllocator::deallocate(int sz, int width, int height)
{
  m_AllocatedMemory.fetchAndAddOrdered(-sz*width*height);
}

void QxrdAllocator::maskDeleter(QxrdMaskData *mask)
{
  //  delete mask;
  mask->deleteLater();
}

void QxrdAllocator::int16Deleter(QxrdInt16ImageData *img)
{
  //  delete img;
  img->deleteLater();
}

void QxrdAllocator::int32Deleter(QxrdInt32ImageData *img)
{
  //  delete img;
  img->deleteLater();
}

void QxrdAllocator::doubleDeleter(QxrdDoubleImageData *img)
{
  //  delete img;
  img->deleteLater();
}

void QxrdAllocator::integratedDeleter(QxrdIntegratedData *img)
{
  //  delete img;
  img->deleteLater();
}

int QxrdAllocator::nFreeInt16()
{
  return  m_FreeInt16Images.size();
}

int QxrdAllocator::nFreeInt32()
{
  return  m_FreeInt32Images.size();
}

int QxrdAllocator::nFreeDouble()
{
  return  m_FreeDoubleImages.size();
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

void QxrdAllocator::allocatorHeartbeat()
{
//  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  int n16 = m_FreeInt16Images.size();
  int n32 = m_FreeInt32Images.size();
  int ndbl = m_FreeDoubleImages.size();
  int nmsk = m_FreeMasks.size();
  int nint = m_FreeIntegratedData.size();

  int n16needed = m_PreallocateInt16.fetchAndAddOrdered(0);
  int n32needed = m_PreallocateInt32.fetchAndAddOrdered(0);
  int ndblneeded = m_PreallocateDouble.fetchAndAddOrdered(0);

  int n16used = m_CountInt16.fetchAndStoreOrdered(0);
  int n32used = m_CountInt32.fetchAndStoreOrdered(0);
  int ndblused = m_CountDouble.fetchAndStoreOrdered(0);

  if (n16used > n16needed) n16needed = n16used;
  if (n32used > n32needed) n32needed = n32used;
  if (ndblused > ndblneeded) ndblneeded = ndblused;

  if (n16needed < 4) n16needed = 4;
  if (n32needed < 4) n32needed = 4;
  if (ndblneeded < 4) ndblneeded = 4;

  QCEP_DEBUG(DEBUG_ALLOCATOR,
             emit printMessage(tr("Allocator heartbeat, %1 out of %2, [%3,%4,%5,%6] [%7,%8,%9]\n")
                               .arg(m_AllocatedMemory/MegaBytes).arg(get_Max()).arg(n16).arg(n32).arg(ndbl).arg(nmsk)
                               .arg(n16needed).arg(n32needed).arg(ndblneeded));
  );

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

  set_Allocated(m_AllocatedMemory/MegaBytes);
}

double QxrdAllocator::allocatedMemoryMB()
{
  return m_AllocatedMemory/MegaBytes;
}

double QxrdAllocator::allocatedMemory()
{
  return m_AllocatedMemory;
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

/******************************************************************
*
*  $Log: qxrdallocator.cpp,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:37  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.11  2010/07/20 20:30:25  jennings
*  Added memory usage display to status bar
*  Improved calculation of processing timings
*
*  Revision 1.1.2.10  2010/07/09 21:33:22  jennings
*  Tried to improve memory management by predicting memory requirements better
*  The allocators are more accurate in limiting allocated memory
*
*  Revision 1.1.2.9  2010/06/23 21:49:35  jennings
*  Made allocator strategy more sophisticated, called it more often
*
*  Revision 1.1.2.8  2010/06/18 16:28:22  jennings
*  Added debugging output to QxrdAllocator constructor/destructor
*
*  Revision 1.1.2.7  2010/06/11 21:21:39  jennings
*  Added integrator data to allocator
*
*  Revision 1.1.2.6  2010/05/27 21:13:14  jennings
*  Added more debug flags
*  Tried to detect when processing is too slow for acquisition
*
*  Revision 1.1.2.5  2010/05/25 20:42:54  jennings
*  Added mutex to allocator object
*
*  Revision 1.1.2.4  2010/05/25 18:47:15  jennings
*  Added memory limit handling
*
*  Revision 1.1.2.3  2010/05/24 21:02:38  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.1.2.2  2010/05/22 03:43:03  jennings
*  APIs for allocator
*
*  Revision 1.1.2.1  2010/05/20 20:15:54  jennings
*  Initial files for QxrdAllocator and QxrdFileSaver threads
*
*
*
*******************************************************************/

