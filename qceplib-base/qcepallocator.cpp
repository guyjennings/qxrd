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

QcepAllocator *g_Allocator = NULL;

QcepAllocator::QcepAllocator
(QString name)
  : QcepObject(name),
    m_Mutex(QMutex::Recursive),
//    m_AllocatedMemoryMB(0),
    m_TotalBufferSizeMB32 (this,"totalBufferSizeMB32", 800, "Maximum Image Memory in 32 bit system (MB)"),
    m_TotalBufferSizeMB64 (this,"totalBufferSizeMB64", 2000,"Maximum Image Memory in 64 bit system (MB)"),
    m_Reserve             (this,"reserve",100, "Extra Reserved Memory (MB)"),
    m_AvailableBytes      (this, "availableBytes", 0, "Total Available Data Memory (Bytes)"),
    m_AllocatedBytes      (this, "allocatedBytes", 0, "Allocated Data Memory (Bytes)")
{
#ifndef QT_NO_DEBUG
  printf("Constructing allocator\n");
#endif

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

//  connect(&m_Timer, SIGNAL(timeout()), this, SLOT(allocatorHeartbeat()));

//  m_Timer.start(100);

//  allocatorHeartbeat();

  if (sizeof(void*) == 4) {
    set_AvailableBytes(qint64(get_TotalBufferSizeMB32())*qint64(MegaBytes));
    connect(prop_TotalBufferSizeMB32(), &QcepIntProperty::valueChanged,
            this, &QcepAllocator::onMemorySizeChanged);
  } else {
    set_AvailableBytes(qint64(get_TotalBufferSizeMB64())*qint64(MegaBytes));
    connect(prop_TotalBufferSizeMB64(), &QcepIntProperty::valueChanged,
            this, &QcepAllocator::onMemorySizeChanged);
  }
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

void QcepAllocator::onMemorySizeChanged(qint64 newMB)
{
  changedAvailableBytes(newMB*qint64(MegaBytes));
}

void QcepAllocator::writeSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::writeSettings(settings);
}

void QcepAllocator::readSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::readSettings(settings);

  if (get_TotalBufferSizeMB32() > 100000000) {
    set_TotalBufferSizeMB32(qint64(get_TotalBufferSizeMB32())/qint64(MegaBytes));
  }

  if (get_TotalBufferSizeMB64() > 100000000) {
    set_TotalBufferSizeMB64(qint64(get_TotalBufferSizeMB64())/qint64(MegaBytes));
  }

  if (sizeof(void*) == 4) {
    set_AvailableBytes(qint64(get_TotalBufferSizeMB32())*qint64(MegaBytes));
  } else {
    set_AvailableBytes(qint64(get_TotalBufferSizeMB64())*qint64(MegaBytes));
  }
}

QMutex *QcepAllocator::mutex()
{
  return &m_Mutex;
}

int QcepAllocator::waitTillAvailable(AllocationStrategy strat, qint64 size)
{
  if (strat == QcepAllocator::WaitTillAvailable) {
    while((get_AllocatedBytes() + size) > get_AvailableBytes()) {
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
    return ((get_AllocatedBytes() + size) < get_AvailableBytes());
  } else if (strat == QcepAllocator::AllocateFromReserve) {
    return ((get_AllocatedBytes() + size) < (get_AvailableBytes() + get_Reserve()*qint64(MegaBytes)));
  } else if (strat == QcepAllocator::AlwaysAllocate) {
    return true;
  } else {
    return false;
  }
}

QcepUInt16ImageDataPtr QcepAllocator::newInt16Image(QString name, int width, int height, AllocationStrategy strat)
{
  if (g_Allocator) {
    QcepMutexLocker lock(__FILE__, __LINE__, g_Allocator->mutex());

    if (g_Allocator->waitTillAvailable(strat, int16Size(width, height))) {
      QcepUInt16ImageDataPtr res(new QcepUInt16ImageData(name, width, height, 0));

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

  return QcepUInt16ImageDataPtr(NULL);
}

QcepUInt32ImageDataPtr QcepAllocator::newInt32Image(QString name, int width, int height, AllocationStrategy strat)
{
  if (g_Allocator) {
    QcepMutexLocker lock(__FILE__, __LINE__, g_Allocator->mutex());

    if (g_Allocator->waitTillAvailable(strat, int32Size(width, height))) {
      QcepUInt32ImageDataPtr res(new QcepUInt32ImageData(name, width, height, 0));

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

  return QcepUInt32ImageDataPtr(NULL);
}

QcepDoubleImageDataPtr QcepAllocator::newDoubleImage(QString name, int width, int height, AllocationStrategy strat)
{
  if (g_Allocator) {
    QcepMutexLocker lock(__FILE__, __LINE__, g_Allocator->mutex());

    if (g_Allocator->waitTillAvailable(strat, doubleSize(width, height))) {
      QcepDoubleImageDataPtr res(new QcepDoubleImageData(name, width, height, 0));

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

    if (g_Allocator->waitTillAvailable(strat, maskSize(width, height))) {
      QcepMaskDataPtr res(new QcepMaskData(name, width, height, def));

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

    if (g_Allocator->waitTillAvailable(strat, integratedSize(size))) {
      QcepIntegratedDataPtr res(new QcepIntegratedData(name, size));

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

    if (g_Allocator->waitTillAvailable(strat, columnScanSize(cols.count(), nRows))) {
      QcepDataColumnScanPtr res(new QcepDataColumnScan(name, cols, nRows));

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

    if (g_Allocator->waitTillAvailable(strat, columnSize(sz))) {
      QcepDataColumnPtr res(new QcepDataColumn(name, sz));

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

    if (g_Allocator->waitTillAvailable(strat, arraySize(dims))) {
      QcepDataArrayPtr res(new QcepDataArray(name, dims));

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
  return QcepDataGroupPtr(new QcepDataGroup(name));
}

QcepDatasetPtr QcepAllocator::newDataset(QString name)
{
  return QcepDatasetPtr(new QcepDataset(name));
}

/* static */
void QcepAllocator::allocate(qint64 sz, qint64 width, qint64 height)
{
  allocate(sz*width*height);
}

/* static */
void QcepAllocator::allocate(qint64 amt)
{
  if (g_Allocator) {
    g_Allocator -> allocateBytes(amt);
  }
}

void QcepAllocator::allocateBytes(qint64 amt)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_AllocatedBytes.incValue(amt);
}

/* static */
void QcepAllocator::deallocate(qint64 sz, qint64 width, qint64 height)
{
  deallocate(sz*width*height);
}

/* static */
void QcepAllocator::deallocate(qint64 amt)
{
  if (g_Allocator) {
    g_Allocator -> deallocateBytes(amt);
  }
}

void QcepAllocator::deallocateBytes(qint64 amt)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_AllocatedBytes.incValue(-amt);
}

qint64 QcepAllocator::int16Size(int width, int height)
{
  return sizeof(quint16)*width*height;
}

qint64 QcepAllocator::int32Size(int width, int height)
{
  return sizeof(quint32)*width*height;
}

qint64 QcepAllocator::doubleSize(int width, int height)
{
  return sizeof(double)*width*height;
}

qint64 QcepAllocator::maskSize(int width, int height)
{
  return sizeof(quint16)*width*height;
}

qint64 QcepAllocator::integratedSize(int nrows)
{
  return (sizeof(double)*4*nrows);
}

qint64 QcepAllocator::columnSize(int sz)
{
  return (sizeof(double)*sz);
}

qint64 QcepAllocator::columnScanSize(int nCols, int nRows)
{
  return (sizeof(double)*nCols*nRows);
}

qint64 QcepAllocator::arraySize(QVector<int> dims)
{
  qint64 res=1;

  foreach (int d, dims) {
    res *= d;
  }

  return res*sizeof(double);
}

//void QcepAllocator::allocatorHeartbeat()
//{
//  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

//  set_Allocated(m_AllocatedBytes);
//}

qint64 QcepAllocator::allocatedMemoryMB()
{
  return allocatedMemory()/qint64(MegaBytes);
}

qint64 QcepAllocator::allocatedMemory()
{
  if (g_Allocator) {
    return g_Allocator->allocatedBytes();
  } else {
    return 0;
  }
}

qint64 QcepAllocator::availableMemoryMB()
{
  return availableMemory()/qint64(MegaBytes);
}

qint64 QcepAllocator::availableMemory()
{
  if (g_Allocator) {
    return g_Allocator->availableBytes();
  } else {
    return 0;
  }
}

void QcepAllocator::changedAvailableBytes(qint64 newsize)
{
  if (g_Allocator) {
    g_Allocator->setAvailableBytes(newsize);
  }
}

qint64 QcepAllocator::availableBytes()
{
  return get_AvailableBytes();
}

qint64 QcepAllocator::allocatedBytes()
{
  return get_AllocatedBytes();
}

void QcepAllocator::setAvailableBytes(qint64 newsize)
{
  set_AvailableBytes(newsize);
}

QcepDataObjectPtr QcepAllocator::newDataObject(QString id, QString name)
{
  QcepDataObjectPtr res;

  if (id == "QcepDataset") {
    res = newDataset(name);
  } else if (id == "QcepDataGroup") {
    res = newGroup(name);
  } else if (id == "QcepDataColumn") {
    res = newColumn(name, 0, QcepAllocator::NullIfNotAvailable);
  } else if (id == "QcepDataColumnScan") {
    res = newColumnScan(name, QStringList(), 0, QcepAllocator::NullIfNotAvailable);
  }

  return res;
}
