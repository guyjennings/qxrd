#include "qcepdebug.h"
#include "qcepallocator.h"
#include "qcepmutexlocker.h"
#include "qcepapplication.h"
#include <stdio.h>
#include "qcepimagedata.h"
#include "qcepmaskdata.h"
#include "qcepintegrateddata.h"
#include "qcepthread.h"
#include "qcepdatacolumn.h"
#include "qcepdataarray.h"
#include "qcepdataset.h"

static QcepAllocator *g_Allocator = NULL;
static quint64        g_AllocatedMemory = 0;

QcepAllocator::QcepAllocator
(QcepSettingsSaverPtr saver)
  : QcepObject("allocator", NULL),
    m_Mutex(QMutex::Recursive),
    m_AllocatedMemoryMB(0),
    m_Max(saver, this, "max", 800, "Maximum Image Memory (MB)"),
    m_TotalBufferSizeMB32(saver, this,"totalBufferSizeMB32", 800, "Maximum Image Memory in 32 bit system (MB)"),
    m_TotalBufferSizeMB64(saver, this,"totalBufferSizeMB64", 2000,"Maximum Image Memory in 64 bit system (MB)"),
    m_Reserve(saver, this,"reserve",100, "Extra Reserved Memory (MB)"),
    m_Allocated(QcepSettingsSaverPtr(), this, "allocated", 0, "Allocated Memory (MB)")
{
  if (g_Allocator) {
    printf("Only one allocator can be created\n");
  } else {
    g_Allocator = this;
  }

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
#ifndef QT_NO_DEBUG
  printf("Deleting allocator\n");
#endif

  g_Allocator = NULL;

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

QcepInt16ImageDataPtr QcepAllocator::newInt16Image(QString name, int width, int height, AllocationStrategy strat)
{
  if (g_Allocator) {
    QcepMutexLocker lock(__FILE__, __LINE__, g_Allocator->mutex());

    if (g_Allocator->waitTillAvailable(strat, g_Allocator->int16SizeMB(width, height))) {
      QcepInt16ImageDataPtr res(new QcepInt16ImageData(QcepSettingsSaverPtr(), name, width, height, 0));

      if (res) {
        res->moveToThread(NULL);
      }

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

QcepInt32ImageDataPtr QcepAllocator::newInt32Image(QString name, int width, int height, AllocationStrategy strat)
{
  if (g_Allocator) {
    QcepMutexLocker lock(__FILE__, __LINE__, g_Allocator->mutex());

    if (g_Allocator->waitTillAvailable(strat, g_Allocator->int32SizeMB(width, height))) {
      QcepInt32ImageDataPtr res(new QcepInt32ImageData(QcepSettingsSaverPtr(), name, width, height, 0));

      if (res) {
        res->moveToThread(NULL);
      }

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

QcepDoubleImageDataPtr QcepAllocator::newDoubleImage(QString name, int width, int height, AllocationStrategy strat)
{
  if (g_Allocator) {
    QcepMutexLocker lock(__FILE__, __LINE__, g_Allocator->mutex());

    if (g_Allocator->waitTillAvailable(strat, g_Allocator->doubleSizeMB(width, height))) {
      QcepDoubleImageDataPtr res(new QcepDoubleImageData(QcepSettingsSaverPtr(), name, width, height, 0));

      if (res) {
        res -> moveToThread(NULL);
      }

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

QcepMaskDataPtr QcepAllocator::newMask(QString name, int width, int height, int def, AllocationStrategy strat)
{
  if (g_Allocator) {
    QcepMutexLocker lock(__FILE__, __LINE__, g_Allocator->mutex());

    if (g_Allocator->waitTillAvailable(strat, g_Allocator->maskSizeMB(width, height))) {
      QcepMaskDataPtr res(new QcepMaskData(QcepSettingsSaverPtr(), name, width, height, def));

      if (res) {
        res->moveToThread(NULL);
      }

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

QcepIntegratedDataPtr QcepAllocator::newIntegratedData(QString name, int size, AllocationStrategy strat)
{
  if (g_Allocator) {
    QcepMutexLocker lock(__FILE__, __LINE__, g_Allocator->mutex());

    if (g_Allocator->waitTillAvailable(strat, g_Allocator->integratedSizeMB(size))) {
      QcepIntegratedDataPtr res(new QcepIntegratedData(QcepSettingsSaverPtr(),
                                                       name,
                                                       size));

      res->moveToThread(NULL);

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

QcepDataColumnScanPtr QcepAllocator::newColumnScan(QString name, QStringList cols, int nRows, AllocationStrategy strat)
{
  if (g_Allocator) {
    QcepMutexLocker lock(__FILE__, __LINE__, g_Allocator->mutex());

    if (g_Allocator->waitTillAvailable(strat, g_Allocator->columnScanSizeMB(cols.count(), nRows))) {
      QcepDataColumnScanPtr res(new QcepDataColumnScan(QcepSettingsSaverWPtr(),name, cols, nRows));

      if (res) {
        res -> moveToThread(NULL);
      }

      if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
        g_Application->printMessage(tr("QcepAllocator::newColumnScan() succeeded [%1] [%2]").HEXARG(res.data()).arg(g_Allocator->allocatedMemoryMB()));
      }

      return res;
    }
  }

  if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
    g_Application->printMessage("QcepAllocator::newColumnScan() returned NULL");
  }

  return QcepDataColumnScanPtr();
}

QcepDataColumnPtr QcepAllocator::newColumn(QString name, int sz, AllocationStrategy strat)
{
  if (g_Allocator) {
    QcepMutexLocker lock(__FILE__, __LINE__, g_Allocator->mutex());

    if (g_Allocator->waitTillAvailable(strat, g_Allocator->columnSizeMB(sz))) {
      QcepDataColumnPtr res(new QcepDataColumn(QcepSettingsSaverWPtr(),name, sz));

      if (res) {
        res -> moveToThread(NULL);
      }

      if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
        g_Application->printMessage(tr("QcepAllocator::newColumn() succeeded [%1] [%2]").HEXARG(res.data()).arg(g_Allocator->allocatedMemoryMB()));
      }

      return res;
    }
  }

  if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
    g_Application->printMessage("QcepAllocator::newColumn() returned NULL");
  }

  return QcepDataColumnPtr();
}

QcepDataArrayPtr QcepAllocator::newArray(QString name, QVector<int> dims, AllocationStrategy strat)
{
  if (g_Allocator) {
    QcepMutexLocker lock(__FILE__, __LINE__, g_Allocator->mutex());

    if (g_Allocator->waitTillAvailable(strat, g_Allocator->arraySizeMB(dims))) {
      QcepDataArrayPtr res(new QcepDataArray(QcepSettingsSaverWPtr(),name, dims));

      if (res) {
        res -> moveToThread(NULL);
      }

      if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
        g_Application->printMessage(tr("QcepAllocator::newArray() succeeded [%1] [%2]").HEXARG(res.data()).arg(g_Allocator->allocatedMemoryMB()));
      }

      return res;
    }
  }

  if (g_Application && qcepDebug(DEBUG_ALLOCATOR)) {
    g_Application->printMessage("QcepAllocator::newArray() returned NULL");
  }

  return QcepDataArrayPtr();
}

QcepDataGroupPtr QcepAllocator::newGroup(QString name)
{
  return QcepDataGroupPtr(new QcepDataGroup(QcepSettingsSaverWPtr(), name));
}

QcepDatasetPtr QcepAllocator::newDataset(QString name)
{
  return QcepDatasetPtr(new QcepDataset(QcepSettingsSaverWPtr(), name));
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

  g_AllocatedMemory += amt;

  m_AllocatedMemoryMB.fetchAndStoreOrdered(g_AllocatedMemory/MegaBytes);
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

  g_AllocatedMemory -= amt;

  m_AllocatedMemoryMB.fetchAndStoreOrdered(g_AllocatedMemory/MegaBytes);
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

int QcepAllocator::columnSizeMB(int sz)
{
  return (sizeof(double)*sz)/MegaBytes;
}

int QcepAllocator::columnScanSizeMB(int nCols, int nRows)
{
  return (sizeof(double)*nCols*nRows)/MegaBytes;
}

int QcepAllocator::arraySizeMB(QVector<int> dims)
{
  int res=1;

  foreach (int d, dims) {
    res *= d;
  }

  return res*sizeof(double)/MegaBytes;
}

void QcepAllocator::allocatorHeartbeat()
{
  set_Allocated(m_AllocatedMemoryMB.fetchAndAddOrdered(0));
}

double QcepAllocator::allocatedMemoryMB()
{
  return m_AllocatedMemoryMB.fetchAndAddOrdered(0);
}

quint64 QcepAllocator::allocatedMemory()
{
  return g_AllocatedMemory;
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
