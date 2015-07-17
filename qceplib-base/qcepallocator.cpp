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
    m_Mutex(QMutex::Recursive),
    m_AllocatedMemory(0),
    m_AllocatedMemoryMB(0),
    m_Max(saver, this, "max", 800, "Maximum Image Memory (MB)"),
    m_TotalBufferSizeMB32(saver, this,"totalBufferSizeMB32", 800, "Maximum Image Memory in 32 bit system (MB)"),
    m_TotalBufferSizeMB64(saver, this,"totalBufferSizeMB64", 2000,"Maximum Image Memory in 64 bit system (MB)"),
    m_Reserve(saver, this,"reserve",100, "Extra Reserved Memory (MB)"),
    m_Allocated(QcepSettingsSaverPtr(), this, "allocated", 0, "Allocated Memory (MB)")
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
                                                       10000));

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
      integ = QcepIntegratedDataPtr(new QcepIntegratedData(QcepSettingsSaverPtr(), img, 10000));

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

/* static */
void QcepAllocator::allocate(int sz, int width, int height)
{
  if (g_Allocator) {
    g_Allocator -> allocate((quint64) sz*width*height);
  }
}

/* static */
void QcepAllocator::allocate(quint64 amt)
{
  if (g_Allocator) {
    g_Allocator -> allocateBytes(amt);
  }
}

void QcepAllocator::allocateBytes(quint64 amt)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_AllocatedMemory += amt;

  m_AllocatedMemoryMB.fetchAndStoreOrdered(m_AllocatedMemory/MegaBytes);
}

/* static */
void QcepAllocator::deallocate(int sz, int width, int height)
{
  if (g_Allocator) {
    g_Allocator -> deallocate((quint64) sz*width*height);
  }
}

/* static */
void QcepAllocator::deallocate(quint64 amt)
{
  if (g_Allocator) {
    g_Allocator -> deallocateBytes(amt);
  }
}

void QcepAllocator::deallocateBytes(quint64 amt)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_AllocatedMemory -= amt;

  m_AllocatedMemoryMB.fetchAndStoreOrdered(m_AllocatedMemory/MegaBytes);
}

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
