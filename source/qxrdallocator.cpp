#include "qxrdallocator.h"
#include "qxrdmutexlocker.h"
#include "qxrdallocatorthread.h"
#include "qxrdapplication.h"

QxrdAllocator::QxrdAllocator
    (QxrdSettingsSaver *saver, QObject *parent)
  : QxrdAllocatorInterface(parent),
    m_AllocatedMemory(0),
    m_AllocatedMemoryMB(0),
    m_Max(saver, this, "max", 800),
    m_Reserve(saver, this,"reserve",100),
    m_Allocated(saver, this, "allocated", 0),
    m_QueuedDelete(saver, this, "queuedDelete", 0),
    m_NAllocatedInt16(saver, this, "nAllocatedInt16", 0),
    m_NAllocatedInt32(saver, this, "nAllocatedInt32", 0),
    m_NAllocatedDouble(saver, this, "nAllocatedDouble", 0),
    m_NAllocatedMask(saver, this, "nAllocatedMask", 0),
    m_NAllocatedIntegrated(saver, this, "nAllocatedIntegrated", 0)
{
  if (qcepDebug(DEBUG_ALLOCATOR)) {
    g_Application->printMessage(tr("allocator %1 constructed").HEXARG(this));
  };

  connect(&m_Timer, SIGNAL(timeout()), this, SLOT(allocatorHeartbeat()));

  m_Timer.start(100);

  allocatorHeartbeat();
}

QxrdAllocator::~QxrdAllocator()
{
  if (qcepDebug(DEBUG_ALLOCATOR)) {
    g_Application->printMessage(tr("allocator %1 deleted").HEXARG(this));
  };
}

int QxrdAllocator::waitTillAvailable(AllocationStrategy strat, int sizeMB)
{
  if (strat == QxrdAllocator::WaitTillAvailable) {
    while((m_AllocatedMemoryMB + sizeMB) > get_Max()) {
      m_Mutex.unlock();

      if (qcepDebug(DEBUG_ALLOCATOR)) {
        g_Application->printMessage("QxrdAllocator::waitTillAvailable() sleeping 100msec");
      }

      QxrdAllocatorThread::msleep(100);

      m_Mutex.lock();
    }

    return true;
  } else if (strat == QxrdAllocator::NullIfNotAvailable) {
    return ((m_AllocatedMemoryMB + sizeMB) < get_Max());
  } else if (strat == QxrdAllocator::AllocateFromReserve) {
    return ((m_AllocatedMemoryMB + sizeMB) < (get_Max() + get_Reserve()));
  } else if (strat == QxrdAllocator::AlwaysAllocate) {
    return true;
  } else {
    return false;
  }
}

QxrdInt16ImageDataPtr QxrdAllocator::newInt16Image(AllocationStrategy strat, int width, int height)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (waitTillAvailable(strat, int16SizeMB(width, height))) {
    QxrdInt16ImageDataPtr res(new QxrdInt16ImageData(NULL, this, AllocateInt16, width, height), &QxrdAllocator::int16Deleter);

    res->moveToThread(thread());

    if (qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::newInt16Image() succeeded [%1] [%2]").HEXARG(res.data()).arg(m_AllocatedMemoryMB));
    }

    return res;
  } else {
    if (qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage("QxrdAllocator::newInt16Image() returned NULL");
    }

    return QxrdInt16ImageDataPtr(NULL);
  }
}

QxrdInt32ImageDataPtr QxrdAllocator::newInt32Image(AllocationStrategy strat, int width, int height)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (waitTillAvailable(strat, int32SizeMB(width, height))) {
    QxrdInt32ImageDataPtr res(new QxrdInt32ImageData(NULL, this, AllocateInt32, width, height), &QxrdAllocator::int32Deleter);

    res->moveToThread(thread());

    if (qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::newInt32Image() succeeded [%1] [%2]").HEXARG(res.data()).arg(m_AllocatedMemoryMB));
    }

    return res;
  } else {
    if (qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage("QxrdAllocator::newInt32Image() returned NULL");
    }

    return QxrdInt32ImageDataPtr(NULL);
  }
}

QxrdDoubleImageDataPtr QxrdAllocator::newDoubleImage(AllocationStrategy strat, int width, int height)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (waitTillAvailable(strat, doubleSizeMB(width, height))) {
    QxrdDoubleImageDataPtr res(new QxrdDoubleImageData(NULL, this, AllocateDouble, width, height), &QxrdAllocator::doubleDeleter);

    res->moveToThread(thread());

    if (qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::newDoubleImage() succeeded [%1] [%2]").HEXARG(res.data()).arg(m_AllocatedMemoryMB));
    }

    return res;
  } else {
    if (qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage("QxrdAllocator::newDoubleImage() returned NULL");
    }

    return QxrdDoubleImageDataPtr(NULL);
  }
}

QxrdMaskDataPtr QxrdAllocator::newMask(AllocationStrategy strat, int width, int height, int def)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (waitTillAvailable(strat, maskSizeMB(width, height))) {
    QxrdMaskDataPtr res(new QxrdMaskData(NULL, this, AllocateMask, width, height, def), &QxrdAllocator::maskDeleter);

    res->moveToThread(thread());

    if (qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::newMask() succeeded [%1] [%2]").HEXARG(res.data()).arg(m_AllocatedMemoryMB));
    }

    return res;
  } else {
    if (qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage("QxrdAllocator::newMask() returned NULL");
    }

    return QxrdMaskDataPtr(NULL);
  }
}

QxrdIntegratedDataPtr QxrdAllocator::newIntegratedData(AllocationStrategy strat, QxrdDoubleImageDataPtr data)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (waitTillAvailable(strat, integratedSizeMB(10000))) {
    QxrdIntegratedDataPtr res(new QxrdIntegratedData(NULL,
                                                     this,
                                                     data,
                                                     AllocateIntegrated,
                                                     10000), &QxrdAllocator::integratedDeleter);

    res->moveToThread(thread());

    if (qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::newIntegratedData() succeeded [%1] [%2]").HEXARG(res.data()).arg(m_AllocatedMemoryMB));
    }

    return res;
  } else {
    if (qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage("QxrdAllocator::newIntegratedData() returned NULL");
    }

    return QxrdIntegratedDataPtr(NULL);
  }
}

void QxrdAllocator::newDoubleImageAndIntegratedData(AllocationStrategy strat,
                                                    int width, int height,
                                                    QxrdDoubleImageDataPtr &img,
                                                    QxrdIntegratedDataPtr &integ)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (waitTillAvailable(strat, doubleSizeMB(width, height) + integratedSizeMB(10000))) {
    img = QxrdDoubleImageDataPtr(new QxrdDoubleImageData(NULL, this, AllocateDouble, width, height), &QxrdAllocator::doubleDeleter);
    integ = QxrdIntegratedDataPtr(new QxrdIntegratedData(NULL, this, img, AllocateIntegrated, 10000), &QxrdAllocator::integratedDeleter);

    img->moveToThread(thread());
    integ->moveToThread(thread());

    if (qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::newDoubleImageAndIntegratedData() succeeded [%1] [%2] [%3]")
             .HEXARG(img.data()).HEXARG(integ.data()).arg(m_AllocatedMemoryMB));
    }
  } else {
    if (qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage("QxrdAllocator::newDoubleImageAndIntegratedData() returned NULL");
    }
  }
}

void QxrdAllocator::allocate(int typ, int sz, int width, int height)
{
  allocate(typ, (quint64) sz*width*height);
}

void QxrdAllocator::allocate(int typ, quint64 amt)
{
//  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_AllocatedMemory += amt;

  m_AllocatedMemoryMB.fetchAndStoreOrdered(m_AllocatedMemory/MegaBytes);

  switch (typ) {
  case AllocateInt16:
    m_NAllocatedInt16.incValue(1);
    break;
  case AllocateInt32:
    m_NAllocatedInt32.incValue(1);
    break;
  case AllocateDouble:
    m_NAllocatedDouble.incValue(1);
    break;
  case AllocateMask:
    m_NAllocatedMask.incValue(1);
    break;
  case AllocateIntegrated:
    m_NAllocatedIntegrated.incValue(1);
    break;
  }
}

void QxrdAllocator::deallocate(int typ, int sz, int width, int height)
{
  deallocate(typ, (quint64) sz*width*height);
}

void QxrdAllocator::deallocate(int typ, quint64 amt)
{
//  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_AllocatedMemory -= amt;

  m_AllocatedMemoryMB.fetchAndStoreOrdered(m_AllocatedMemory/MegaBytes);

  switch (typ) {
  case AllocateInt16:
    m_NAllocatedInt16.incValue(-1);
    break;
  case AllocateInt32:
    m_NAllocatedInt32.incValue(-1);
    break;
  case AllocateDouble:
    m_NAllocatedDouble.incValue(-1);
    break;
  case AllocateMask:
    m_NAllocatedMask.incValue(-1);
    break;
  case AllocateIntegrated:
    m_NAllocatedIntegrated.incValue(-1);
    break;
  }
}

static int g_QueuedDelete = 0;

void QxrdAllocator::maskDeleter(QxrdMaskData *mask)
{
//    delete mask;
  if (g_QueuedDelete) {
    if (qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::maskDeleter deleteLater %1 [%2]").HEXARG(mask).arg(mask->allocatedMemoryMB()));
    }

    mask->deleteLater();
  } else {
    if (qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::maskDeleter delete %1 [%2]").HEXARG(mask).arg(mask->allocatedMemoryMB()));
    }

    delete mask;
  }
}

void QxrdAllocator::int16Deleter(QxrdInt16ImageData *img)
{
//    delete img;
  if (g_QueuedDelete) {
    if (qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::int16Deleter deleteLater %1 [%2]").HEXARG(img).arg(img->allocatedMemoryMB()));
    }

    img->deleteLater();
  } else {
    if (qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::int16Deleter delete %1 [%2]").HEXARG(img).arg(img->allocatedMemoryMB()));
    }

    delete img;
  }
}

void QxrdAllocator::int32Deleter(QxrdInt32ImageData *img)
{
//    delete img;
  if (g_QueuedDelete) {
    if (qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::int32Deleter deleteLater %1 [%2]").HEXARG(img).arg(img->allocatedMemoryMB()));
    }

    img->deleteLater();
  } else {
    if (qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::int32Deleter delete %1 [%2]").HEXARG(img).arg(img->allocatedMemoryMB()));
    }

    delete img;
  }
}

void QxrdAllocator::doubleDeleter(QxrdDoubleImageData *img)
{
//    delete img;
  if (g_QueuedDelete) {
    if (qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::doubleDeleter deleteLater %1 [%2]").HEXARG(img).arg(img->allocatedMemoryMB()));
    }

    img->deleteLater();
  } else {
    if (qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::doubleDeleter delete %1 [%2]").HEXARG(img).arg(img->allocatedMemoryMB()));
    }

    delete img;
  }
}

void QxrdAllocator::integratedDeleter(QxrdIntegratedData *img)
{
//    delete img;
  if (g_QueuedDelete) {
    if (qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::integratedDeleter deleteLater %1 [%2]").HEXARG(img).arg(img->allocatedMemoryMB()));
    }

    img->deleteLater();
  } else {
    if (qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::integratedDeleter delete %1 [%2]").HEXARG(img).arg(img->allocatedMemoryMB()));
    }

    delete img;
  }
}

int QxrdAllocator::int16SizeMB(int width, int height)
{
  return sizeof(quint16)*width*height/MegaBytes;
}

int QxrdAllocator::int32SizeMB(int width, int height)
{
  return sizeof(quint32)*width*height/MegaBytes;
}

int QxrdAllocator::doubleSizeMB(int width, int height)
{
  return sizeof(double)*width*height/MegaBytes;
}

int QxrdAllocator::maskSizeMB(int width, int height)
{
  return sizeof(quint16)*width*height/MegaBytes;
}

int QxrdAllocator::integratedSizeMB(int nrows)
{
  return (sizeof(double)*4*nrows)/MegaBytes;
}

void QxrdAllocator::allocatorHeartbeat()
{
  g_QueuedDelete = get_QueuedDelete();

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

void QxrdAllocator::report()
{
  g_Application->printMessage(tr("Allocator: %1 i16, %2 i32, %3 dbl, %4 msk, %5 integ")
                              .arg(get_NAllocatedInt16())
                              .arg(get_NAllocatedInt32())
                              .arg(get_NAllocatedDouble())
                              .arg(get_NAllocatedMask())
                              .arg(get_NAllocatedIntegrated())
                              );
}
