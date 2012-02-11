#include "qxrdallocator.h"
#include "qxrdmutexlocker.h"
#include "qxrdallocatorthread.h"
#include "qxrdapplication.h"

QxrdAllocator::QxrdAllocator
(QxrdSettingsSaverPtr saver, QObject *parent)
  : QObject(parent),
    m_AllocatedMemory(0),
    m_AllocatedMemoryMB(0),
    m_Max(saver, this, "max", 800),
    m_TotalBufferSizeMB32(saver, this,"totalBufferSizeMB32", 800),
    m_TotalBufferSizeMB64(saver, this,"totalBufferSizeMB64", 2000),
    m_Reserve(saver, this,"reserve",100),
    m_Allocated(QxrdSettingsSaverPtr(), this, "allocated", 0),
    m_QueuedDelete(QxrdSettingsSaverPtr(), this, "queuedDelete", 0),
    m_NAllocatedInt16(QxrdSettingsSaverPtr(), this, "nAllocatedInt16", 0),
    m_NAllocatedInt32(QxrdSettingsSaverPtr(), this, "nAllocatedInt32", 0),
    m_NAllocatedDouble(QxrdSettingsSaverPtr(), this, "nAllocatedDouble", 0),
    m_NAllocatedMask(QxrdSettingsSaverPtr(), this, "nAllocatedMask", 0),
    m_NAllocatedIntegrated(QxrdSettingsSaverPtr(), this, "nAllocatedIntegrated", 0)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAllocator::QxrdAllocator(%p)\n", this);
  }

  if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
    g_Application->printMessage(tr("allocator %1 constructed").HEXARG(this));
  };

  connect(&m_Timer, SIGNAL(timeout()), this, SLOT(allocatorHeartbeat()));

  m_Timer.start(100);

  allocatorHeartbeat();
}

QxrdAllocator::~QxrdAllocator()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAllocator::~QxrdAllocator(%p)\n", this);
  }

  if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
    g_Application->printMessage(tr("allocator %1 deleted").HEXARG(this));
  };
}

void QxrdAllocator::writeSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdAllocator::readSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);

  if (get_TotalBufferSizeMB32() > 100000000) {
    set_TotalBufferSizeMB32(get_TotalBufferSizeMB32()/MegaBytes);
  }

  if (get_TotalBufferSizeMB64() > 100000000) {
    set_TotalBufferSizeMB64(get_TotalBufferSizeMB64()/MegaBytes);
  }
}

QMutex *QxrdAllocator::mutex()
{
  return &m_Mutex;
}

int QxrdAllocator::waitTillAvailable(AllocationStrategy strat, int sizeMB)
{
  if (strat == QxrdAllocator::WaitTillAvailable) {
    while((m_AllocatedMemoryMB + sizeMB) > get_Max()) {
      m_Mutex.unlock();

      if (qcepDebug(DEBUG_ALLOCATOR)) {
        if (g_Application) {
          g_Application->printMessage("QxrdAllocator::waitTillAvailable() sleeping 100msec");
        }
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

QxrdInt16ImageDataPtr QxrdAllocator::newInt16Image(QxrdAllocatorWPtr allocw, AllocationStrategy strat, int width, int height)
{
  QxrdAllocatorPtr alloc(allocw);

  if (alloc) {
    QxrdMutexLocker lock(__FILE__, __LINE__, alloc->mutex());

    if (alloc->waitTillAvailable(strat, alloc->int16SizeMB(width, height))) {
      QxrdInt16ImageDataPtr res(new QxrdInt16ImageData(QxrdSettingsSaverPtr(), alloc, AllocateInt16, width, height), &QxrdAllocator::int16Deleter);

      res->moveToThread(alloc->thread());

      if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
        g_Application->printMessage(tr("QxrdAllocator::newInt16Image() succeeded [%1] [%2]").HEXARG(res.data()).arg(alloc->allocatedMemoryMB()));
      }

      return res;
    }
  }

  if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
    g_Application->printMessage("QxrdAllocator::newInt16Image() returned NULL");
  }

  return QxrdInt16ImageDataPtr(NULL);
}

QxrdInt32ImageDataPtr QxrdAllocator::newInt32Image(QxrdAllocatorWPtr allocw, AllocationStrategy strat, int width, int height)
{
  QxrdAllocatorPtr alloc(allocw);

  if (alloc) {
    QxrdMutexLocker lock(__FILE__, __LINE__, alloc->mutex());

    if (alloc->waitTillAvailable(strat, alloc->int32SizeMB(width, height))) {
      QxrdInt32ImageDataPtr res(new QxrdInt32ImageData(QxrdSettingsSaverPtr(), alloc, AllocateInt32, width, height), &QxrdAllocator::int32Deleter);

      res->moveToThread(alloc->thread());

      if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
        g_Application->printMessage(tr("QxrdAllocator::newInt32Image() succeeded [%1] [%2]").HEXARG(res.data()).arg(alloc->allocatedMemoryMB()));
      }

      return res;
    }
  }

  if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
    g_Application->printMessage("QxrdAllocator::newInt32Image() returned NULL");
  }

  return QxrdInt32ImageDataPtr(NULL);
}

QxrdDoubleImageDataPtr QxrdAllocator::newDoubleImage(QxrdAllocatorWPtr allocw, AllocationStrategy strat, int width, int height)
{
  QxrdAllocatorPtr alloc(allocw);

  if (alloc) {
    QxrdMutexLocker lock(__FILE__, __LINE__, alloc->mutex());

    if (alloc->waitTillAvailable(strat, alloc->doubleSizeMB(width, height))) {
      QxrdDoubleImageDataPtr res(new QxrdDoubleImageData(QxrdSettingsSaverPtr(), alloc, AllocateDouble, width, height), &QxrdAllocator::doubleDeleter);

      res->moveToThread(alloc->thread());

      if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
        g_Application->printMessage(tr("QxrdAllocator::newDoubleImage() succeeded [%1] [%2]").HEXARG(res.data()).arg(alloc->allocatedMemoryMB()));
      }

      return res;
    }
  }

  if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
    g_Application->printMessage("QxrdAllocator::newDoubleImage() returned NULL");
  }

  return QxrdDoubleImageDataPtr(NULL);
}

QxrdMaskDataPtr QxrdAllocator::newMask(QxrdAllocatorWPtr allocw, AllocationStrategy strat, int width, int height, int def)
{
  QxrdAllocatorPtr alloc(allocw);

  if (alloc) {
    QxrdMutexLocker lock(__FILE__, __LINE__, alloc->mutex());

    if (alloc->waitTillAvailable(strat, alloc->maskSizeMB(width, height))) {
      QxrdMaskDataPtr res(new QxrdMaskData(QxrdSettingsSaverPtr(), alloc, AllocateMask, width, height, def), &QxrdAllocator::maskDeleter);

      res->moveToThread(alloc->thread());

      if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
        g_Application->printMessage(tr("QxrdAllocator::newMask() succeeded [%1] [%2]").HEXARG(res.data()).arg(alloc->allocatedMemoryMB()));
      }

      return res;
    }
  }

  if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
    g_Application->printMessage("QxrdAllocator::newMask() returned NULL");
  }

  return QxrdMaskDataPtr(NULL);
}

QxrdIntegratedDataPtr QxrdAllocator::newIntegratedData(QxrdAllocatorWPtr allocw, AllocationStrategy strat, QxrdDoubleImageDataPtr data)
{
  QxrdAllocatorPtr alloc(allocw);

  if (alloc) {
    QxrdMutexLocker lock(__FILE__, __LINE__, alloc->mutex());

    if (alloc->waitTillAvailable(strat, alloc->integratedSizeMB(10000))) {
      QxrdIntegratedDataPtr res(new QxrdIntegratedData(QxrdSettingsSaverPtr(),
                                                       alloc,
                                                       data,
                                                       AllocateIntegrated,
                                                       10000), &QxrdAllocator::integratedDeleter);

      res->moveToThread(alloc->thread());

      if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
        g_Application->printMessage(tr("QxrdAllocator::newIntegratedData() succeeded [%1] [%2]").HEXARG(res.data()).arg(alloc->allocatedMemoryMB()));
      }

      return res;
    }
  }

  if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
    g_Application->printMessage("QxrdAllocator::newIntegratedData() returned NULL");
  }

  return QxrdIntegratedDataPtr(NULL);
}

void QxrdAllocator::newDoubleImageAndIntegratedData(QxrdAllocatorWPtr allocw,
                                                    QxrdAllocator::AllocationStrategy strat, int width,
                                                    int height,
                                                    QxrdDoubleImageDataPtr &img, QxrdIntegratedDataPtr &integ)
{
  QxrdAllocatorPtr alloc(allocw);

  if (alloc) {
    QxrdMutexLocker lock(__FILE__, __LINE__, alloc->mutex());

    if (alloc->waitTillAvailable(strat, alloc->doubleSizeMB(width, height) + alloc->integratedSizeMB(10000))) {
      img = QxrdDoubleImageDataPtr(new QxrdDoubleImageData(QxrdSettingsSaverPtr(), alloc, AllocateDouble, width, height), &QxrdAllocator::doubleDeleter);
      integ = QxrdIntegratedDataPtr(new QxrdIntegratedData(QxrdSettingsSaverPtr(), alloc, img, AllocateIntegrated, 10000), &QxrdAllocator::integratedDeleter);

      img->moveToThread(alloc->thread());
      integ->moveToThread(alloc->thread());

      if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
        g_Application->printMessage(tr("QxrdAllocator::newDoubleImageAndIntegratedData() succeeded [%1] [%2] [%3]")
                          .HEXARG(img.data()).HEXARG(integ.data()).arg(alloc->allocatedMemoryMB()));
      }
    }
  }

  if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
    g_Application->printMessage("QxrdAllocator::newDoubleImageAndIntegratedData() returned NULL");
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
    if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::maskDeleter deleteLater %1 [%2]").HEXARG(mask).arg(mask->allocatedMemoryMB()));
    }

    mask->deleteLater();
  } else {
    if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::maskDeleter delete %1 [%2]").HEXARG(mask).arg(mask->allocatedMemoryMB()));
    }

    delete mask;
  }
}

void QxrdAllocator::int16Deleter(QxrdInt16ImageData *img)
{
  //    delete img;
  if (g_QueuedDelete) {
    if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::int16Deleter deleteLater %1 [%2]").HEXARG(img).arg(img->allocatedMemoryMB()));
    }

    img->deleteLater();
  } else {
    if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::int16Deleter delete %1 [%2]").HEXARG(img).arg(img->allocatedMemoryMB()));
    }

    delete img;
  }
}

void QxrdAllocator::int32Deleter(QxrdInt32ImageData *img)
{
  //    delete img;
  if (g_QueuedDelete) {
    if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::int32Deleter deleteLater %1 [%2]").HEXARG(img).arg(img->allocatedMemoryMB()));
    }

    img->deleteLater();
  } else {
    if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::int32Deleter delete %1 [%2]").HEXARG(img).arg(img->allocatedMemoryMB()));
    }

    delete img;
  }
}

void QxrdAllocator::doubleDeleter(QxrdDoubleImageData *img)
{
  //    delete img;
  if (g_QueuedDelete) {
    if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::doubleDeleter deleteLater %1 [%2]").HEXARG(img).arg(img->allocatedMemoryMB()));
    }

    img->deleteLater();
  } else {
    if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::doubleDeleter delete %1 [%2]").HEXARG(img).arg(img->allocatedMemoryMB()));
    }

    delete img;
  }
}

void QxrdAllocator::integratedDeleter(QxrdIntegratedData *img)
{
  //    delete img;
  if (g_QueuedDelete) {
    if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
      g_Application->printMessage(tr("QxrdAllocator::integratedDeleter deleteLater %1 [%2]").HEXARG(img).arg(img->allocatedMemoryMB()));
    }

    img->deleteLater();
  } else {
    if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
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
  if (g_Application) {
    g_Application->printMessage(tr("Allocator: %1 i16, %2 i32, %3 dbl, %4 msk, %5 integ")
                      .arg(get_NAllocatedInt16())
                      .arg(get_NAllocatedInt32())
                      .arg(get_NAllocatedDouble())
                      .arg(get_NAllocatedMask())
                      .arg(get_NAllocatedIntegrated())
                      );
  }
}
