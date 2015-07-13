#include "qcepdebug.h"
#include "qcepallocator.h"
#include "qcepmutexlocker.h"
#include "qcepapplication.h"
#include <stdio.h>
#include "qcepimagedata.h"
#include "qcepmaskdata.h"
#include "qcepintegrateddata.h"
#include "qcepthread.h"

static QcepAllocator *g_Allocator = NULL;

QcepAllocator::QcepAllocator
(QcepSettingsSaverPtr saver)
  : QcepObject("allocator", NULL),
    m_AllocatedMemory(0),
    m_AllocatedMemoryMB(0),
    m_Max(saver, this, "max", 800, "Maximum Image Memory (MB)"),
    m_TotalBufferSizeMB32(saver, this,"totalBufferSizeMB32", 800, "Maximum Image Memory in 32 bit system (MB)"),
    m_TotalBufferSizeMB64(saver, this,"totalBufferSizeMB64", 2000,"Maximum Image Memory in 64 bit system (MB)"),
    m_Reserve(saver, this,"reserve",100, "Extra Reserved Memory (MB)"),
    m_Allocated(QcepSettingsSaverPtr(), this, "allocated", 0, "Allocated Memory (MB)"),
    m_QueuedDelete(QcepSettingsSaverPtr(), this, "queuedDelete", 0, "Queued Delete?"),
    m_NAllocatedInt16(QcepSettingsSaverPtr(), this, "nAllocatedInt16", 0, "Number of 16 bit images allocated"),
    m_NAllocatedInt32(QcepSettingsSaverPtr(), this, "nAllocatedInt32", 0, "Number of 32 bit images allocated"),
    m_NAllocatedDouble(QcepSettingsSaverPtr(), this, "nAllocatedDouble", 0, "Number of double images allocated"),
    m_NAllocatedMask(QcepSettingsSaverPtr(), this, "nAllocatedMask", 0, "Number of mask images allocated"),
    m_NAllocatedIntegrated(QcepSettingsSaverPtr(), this, "nAllocatedIntegrated", 0, "Number of integrated data sets allocated")
{
  g_Allocator = this;

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QcepAllocator::QcepAllocator(%p)\n", this);
  }

  if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
    g_Application->printMessage(tr("allocator %1 constructed").HEXARG(this));
  };

  connect(&m_Timer, SIGNAL(timeout()), this, SLOT(allocatorHeartbeat()));

  m_Timer.start(100);

  allocatorHeartbeat();
}

QcepAllocator::~QcepAllocator()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QcepAllocator::~QcepAllocator(%p)\n", this);
  }

  if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
    g_Application->printMessage(tr("allocator %1 deleted").HEXARG(this));
  };
}

void QcepAllocator::writeSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::writeSettings(settings, section);
}

void QcepAllocator::readSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::readSettings(settings, section);

  if (get_TotalBufferSizeMB32() > 100000000) {
    set_TotalBufferSizeMB32(get_TotalBufferSizeMB32()/MegaBytes);
  }

  if (get_TotalBufferSizeMB64() > 100000000) {
    set_TotalBufferSizeMB64(get_TotalBufferSizeMB64()/MegaBytes);
  }
}

QMutex *QcepAllocator::mutex()
{
  return &m_Mutex;
}

int QcepAllocator::waitTillAvailable(AllocationStrategy strat, int sizeMB)
{
  if (strat == QcepAllocator::WaitTillAvailable) {
    while((m_AllocatedMemoryMB.fetchAndAddOrdered(0) + sizeMB) > get_Max()) {
      m_Mutex.unlock();

      if (qcepDebug(DEBUG_ALLOCATOR)) {
        if (g_Application) {
          g_Application->printMessage("QcepAllocator::waitTillAvailable() sleeping 100msec");
        }
      }

      QcepThread::msleep(100);

      m_Mutex.lock();
    }

    return true;
  } else if (strat == QcepAllocator::NullIfNotAvailable) {
    return ((m_AllocatedMemoryMB.fetchAndAddOrdered(0) + sizeMB) < get_Max());
  } else if (strat == QcepAllocator::AllocateFromReserve) {
    return ((m_AllocatedMemoryMB.fetchAndAddOrdered(0) + sizeMB) < (get_Max() + get_Reserve()));
  } else if (strat == QcepAllocator::AlwaysAllocate) {
    return true;
  } else {
    return false;
  }
}

QcepInt16ImageDataPtr QcepAllocator::newInt16Image(AllocationStrategy strat, int width, int height)
{
  if (g_Allocator) {
    QcepMutexLocker lock(__FILE__, __LINE__, g_Allocator->mutex());

    if (g_Allocator->waitTillAvailable(strat, g_Allocator->int16SizeMB(width, height))) {
      QcepInt16ImageDataPtr res(new QcepInt16ImageData(QcepSettingsSaverPtr(), width, height));

//      res->moveToThread(g_Allocator->thread());

      if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
        g_Application->printMessage(tr("QcepAllocator::newInt16Image() succeeded [%1] [%2]").HEXARG(res.data()).arg(g_Allocator->allocatedMemoryMB()));
      }

      return res;
    }
  }

  if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
    g_Application->printMessage("QcepAllocator::newInt16Image() returned NULL");
  }

  return QcepInt16ImageDataPtr(NULL);
}

QcepInt32ImageDataPtr QcepAllocator::newInt32Image(AllocationStrategy strat, int width, int height)
{
  if (g_Allocator) {
    QcepMutexLocker lock(__FILE__, __LINE__, g_Allocator->mutex());

    if (g_Allocator->waitTillAvailable(strat, g_Allocator->int32SizeMB(width, height))) {
      QcepInt32ImageDataPtr res(new QcepInt32ImageData(QcepSettingsSaverPtr(), width, height));

//      res->moveToThread(g_Allocator->thread());

      if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
        g_Application->printMessage(tr("QcepAllocator::newInt32Image() succeeded [%1] [%2]").HEXARG(res.data()).arg(g_Allocator->allocatedMemoryMB()));
      }

      return res;
    }
  }

  if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
    g_Application->printMessage("QcepAllocator::newInt32Image() returned NULL");
  }

  return QcepInt32ImageDataPtr(NULL);
}

QcepDoubleImageDataPtr QcepAllocator::newDoubleImage(AllocationStrategy strat, int width, int height)
{
  if (g_Allocator) {
    QcepMutexLocker lock(__FILE__, __LINE__, g_Allocator->mutex());

    if (g_Allocator->waitTillAvailable(strat, g_Allocator->doubleSizeMB(width, height))) {
      QcepDoubleImageDataPtr res(new QcepDoubleImageData(QcepSettingsSaverPtr(), width, height));

//      res->moveToThread(g_Allocator->thread());

      if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
        g_Application->printMessage(tr("QcepAllocator::newDoubleImage() succeeded [%1] [%2]").HEXARG(res.data()).arg(g_Allocator->allocatedMemoryMB()));
      }

      return res;
    }
  }

  if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
    g_Application->printMessage("QcepAllocator::newDoubleImage() returned NULL");
  }

  return QcepDoubleImageDataPtr(NULL);
}

QcepDoubleImageDataPtr QcepAllocator::newDoubleImage(QString name, int width, int height)
{
  QcepDoubleImageDataPtr img;

  if (g_Application) {
   img = newDoubleImage(QcepAllocator::NullIfNotAvailable, width, height);

   if (img) {
     img->set_Name(name);
   }
  }

  return img;
}

QcepMaskDataPtr QcepAllocator::newMask(AllocationStrategy strat, int width, int height, int def)
{
  if (g_Allocator) {
    QcepMutexLocker lock(__FILE__, __LINE__, g_Allocator->mutex());

    if (g_Allocator->waitTillAvailable(strat, g_Allocator->maskSizeMB(width, height))) {
      QcepMaskDataPtr res(new QcepMaskData(QcepSettingsSaverPtr(), width, height, def));

//      res->moveToThread(g_Allocator->thread());

      if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
        g_Application->printMessage(tr("QcepAllocator::newMask() succeeded [%1] [%2]").HEXARG(res.data()).arg(g_Allocator->allocatedMemoryMB()));
      }

      return res;
    }
  }

  if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
    g_Application->printMessage("QcepAllocator::newMask() returned NULL");
  }

  return QcepMaskDataPtr(NULL);
}

QcepIntegratedDataPtr QcepAllocator::newIntegratedData(AllocationStrategy strat, QcepDoubleImageDataPtr data)
{
  if (g_Allocator) {
    QcepMutexLocker lock(__FILE__, __LINE__, g_Allocator->mutex());

    if (g_Allocator->waitTillAvailable(strat, g_Allocator->integratedSizeMB(10000))) {
      QcepIntegratedDataPtr res(new QcepIntegratedData(QcepSettingsSaverPtr(),
                                                       data,
                                                       AllocateIntegrated,
                                                       10000));

//      res->moveToThread(g_Allocator->thread());

      if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
        g_Application->printMessage(tr("QcepAllocator::newIntegratedData() succeeded [%1] [%2]").HEXARG(res.data()).arg(g_Allocator->allocatedMemoryMB()));
      }

      return res;
    }
  }

  if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
    g_Application->printMessage("QcepAllocator::newIntegratedData() returned NULL");
  }

  return QcepIntegratedDataPtr(NULL);
}

QcepIntegratedDataPtr QcepAllocator::newIntegratedData(QString name, int size)
{
  QcepIntegratedDataPtr integ;

  if (g_Application) {
    integ = newIntegratedData(QcepAllocator::NullIfNotAvailable, QcepDoubleImageDataPtr());

    if (integ) {
      integ->set_Name(name);

      integ->resize(size);
    }
  }

  return integ;
}

void QcepAllocator::newDoubleImageAndIntegratedData(QcepAllocator::AllocationStrategy strat, int width,
                                                    int height,
                                                    QcepDoubleImageDataPtr &img, QcepIntegratedDataPtr &integ)
{
  if (g_Allocator) {
    QcepMutexLocker lock(__FILE__, __LINE__, g_Allocator->mutex());

    if (g_Allocator->waitTillAvailable(strat, g_Allocator->doubleSizeMB(width, height) + g_Allocator->integratedSizeMB(10000))) {
      img = QcepDoubleImageDataPtr(new QcepDoubleImageData(QcepSettingsSaverPtr(), width, height));
      integ = QcepIntegratedDataPtr(new QcepIntegratedData(QcepSettingsSaverPtr(), img, AllocateIntegrated, 10000));

//      img->moveToThread(g_Allocator->thread());
//      integ->moveToThread(g_Allocator->thread());

      if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
        g_Application->printMessage(tr("QcepAllocator::newDoubleImageAndIntegratedData() succeeded [%1] [%2] [%3]")
                          .HEXARG(img.data()).HEXARG(integ.data()).arg(g_Allocator->allocatedMemoryMB()));
      }
    }
  }

  if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
    g_Application->printMessage("QcepAllocator::newDoubleImageAndIntegratedData() returned NULL");
  }
}

QcepInt16ImageDataPtr QcepAllocator::newInt16Image(int width, int height)
{
  return QcepAllocator::newInt16Image(WaitTillAvailable, width, height);
}

QcepInt32ImageDataPtr QcepAllocator::newInt32Image(int width, int height)
{
  return QcepAllocator::newInt32Image(WaitTillAvailable, width, height);
}

QcepDoubleImageDataPtr QcepAllocator::newDoubleImage(int width, int height)
{
  return QcepAllocator::newDoubleImage(WaitTillAvailable, width, height);
}

QcepMaskDataPtr       QcepAllocator::newMask(int width, int height)
{
  return QcepAllocator::newMask(WaitTillAvailable, width, height);
}

QcepIntegratedDataPtr QcepAllocator::newIntegratedData(QcepDoubleImageDataPtr image)
{
  return QcepAllocator::newIntegratedData(WaitTillAvailable, image);
}

void QcepAllocator::allocate(int typ, int sz, int width, int height)
{
  allocate(typ, (quint64) sz*width*height);
}

void QcepAllocator::allocate(int typ, quint64 amt)
{
  //  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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

void QcepAllocator::deallocate(int typ, int sz, int width, int height)
{
  deallocate(typ, (quint64) sz*width*height);
}

void QcepAllocator::deallocate(int typ, quint64 amt)
{
  //  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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

//static int g_QueuedDelete = 0;

//void QcepAllocator::maskDeleter(QcepMaskData *mask)
//{
//  //    delete mask;
//  if (g_QueuedDelete) {
//    if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
//      g_Application->printMessage(tr("QcepAllocator::maskDeleter deleteLater %1 [%2]").HEXARG(mask).arg(mask->allocatedMemoryMB()));
//    }

//    mask->deleteLater();
//  } else {
//    if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
//      g_Application->printMessage(tr("QcepAllocator::maskDeleter delete %1 [%2]").HEXARG(mask).arg(mask->allocatedMemoryMB()));
//    }

//    delete mask;
//  }
//}

//void QcepAllocator::int16Deleter(QcepInt16ImageData *img)
//{
//  //    delete img;
//  if (g_QueuedDelete) {
//    if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
//      g_Application->printMessage(tr("QcepAllocator::int16Deleter deleteLater %1 [%2]").HEXARG(img).arg(img->allocatedMemoryMB()));
//    }

//    img->deleteLater();
//  } else {
//    if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
//      g_Application->printMessage(tr("QcepAllocator::int16Deleter delete %1 [%2]").HEXARG(img).arg(img->allocatedMemoryMB()));
//    }

//    delete img;
//  }
//}

//void QcepAllocator::int32Deleter(QcepInt32ImageData *img)
//{
//  //    delete img;
//  if (g_QueuedDelete) {
//    if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
//      g_Application->printMessage(tr("QcepAllocator::int32Deleter deleteLater %1 [%2]").HEXARG(img).arg(img->allocatedMemoryMB()));
//    }

//    img->deleteLater();
//  } else {
//    if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
//      g_Application->printMessage(tr("QcepAllocator::int32Deleter delete %1 [%2]").HEXARG(img).arg(img->allocatedMemoryMB()));
//    }

//    delete img;
//  }
//}

//void QcepAllocator::doubleDeleter(QcepDoubleImageData *img)
//{
//  //    delete img;
//  if (g_QueuedDelete) {
//    if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
//      g_Application->printMessage(tr("QcepAllocator::doubleDeleter deleteLater %1 [%2]").HEXARG(img).arg(img->allocatedMemoryMB()));
//    }

//    img->deleteLater();
//  } else {
//    if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
//      g_Application->printMessage(tr("QcepAllocator::doubleDeleter delete %1 [%2]").HEXARG(img).arg(img->allocatedMemoryMB()));
//    }

//    delete img;
//  }
//}

//void QcepAllocator::integratedDeleter(QcepIntegratedData *img)
//{
//  //    delete img;
//  if (g_QueuedDelete) {
//    if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
//      g_Application->printMessage(tr("QcepAllocator::integratedDeleter deleteLater %1 [%2]").HEXARG(img).arg(img->allocatedMemoryMB()));
//    }

//    img->deleteLater();
//  } else {
//    if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
//      g_Application->printMessage(tr("QcepAllocator::integratedDeleter delete %1 [%2]").HEXARG(img).arg(img->allocatedMemoryMB()));
//    }

//    delete img;
//  }
//}

int QcepAllocator::int16SizeMB(int width, int height)
{
  return sizeof(quint16)*width*height/MegaBytes;
}

int QcepAllocator::int32SizeMB(int width, int height)
{
  return sizeof(quint32)*width*height/MegaBytes;
}

int QcepAllocator::doubleSizeMB(int width, int height)
{
  return sizeof(double)*width*height/MegaBytes;
}

int QcepAllocator::maskSizeMB(int width, int height)
{
  return sizeof(quint16)*width*height/MegaBytes;
}

int QcepAllocator::integratedSizeMB(int nrows)
{
  return (sizeof(double)*4*nrows)/MegaBytes;
}

void QcepAllocator::allocatorHeartbeat()
{
  set_Allocated(m_AllocatedMemoryMB.fetchAndAddOrdered(0));
}

double QcepAllocator::allocatedMemoryMB()
{
  return m_AllocatedMemoryMB.fetchAndAddOrdered(0);
}

double QcepAllocator::allocatedMemory()
{
  return m_AllocatedMemoryMB.fetchAndAddOrdered(0)*MegaBytes;
}

double QcepAllocator::maximumMemoryMB()
{
  return get_Max();
}

double QcepAllocator::maximumMemory()
{
  return get_Max()*MegaBytes;
}

void QcepAllocator::changedSizeMB(int newMB)
{
  set_Max(newMB);
}

void QcepAllocator::report()
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
