#ifndef QCEPALLOCATOR_H
#define QCEPALLOCATOR_H

#include "qceplib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"

#include <QMutex>
#include <QTimer>

#include "qcepproperty.h"

#include "qcepallocator-ptr.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"
#include "qcepintegrateddata-ptr.h"
#include "qcepdatacolumnscan.h"
#include "qcepdatacolumn-ptr.h"
#include "qcepdataset-ptr.h"

extern QCEP_EXPORT QcepAllocator *g_Allocator;

class QCEP_EXPORT QcepAllocator : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  QcepAllocator(QString name);
  virtual ~QcepAllocator();
  void initialize(QcepObjectWPtr parent);

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

public:

  enum AllocationStrategy {
    WaitTillAvailable,
    NullIfNotAvailable,
    AllocateFromReserve,
    AlwaysAllocate
  };

  static QcepDataObjectPtr      newDataObject(QcepObjectWPtr parent,
                                              QString        id,
                                              QString        name);

  static QcepUInt16ImageDataPtr newInt16Image(QcepObjectWPtr     parent,
                                              QString            name,
                                              int                width,
                                              int                height,
                                              AllocationStrategy strat);

  static QcepUInt32ImageDataPtr newInt32Image(QcepObjectWPtr     parent,
                                              QString            name,
                                              int                width,
                                              int                height,
                                              AllocationStrategy strat);

  static QcepDoubleImageDataPtr newDoubleImage(QcepObjectWPtr     parent,
                                               QString            name,
                                               int                width,
                                               int                height,
                                               AllocationStrategy strat);

  static QcepMaskDataPtr        newMask(QcepObjectWPtr     parent,
                                        QString            name,
                                        int                width,
                                        int                height,
                                        int                def,
                                        AllocationStrategy strat);

  static QcepIntegratedDataPtr  newIntegratedData(QcepObjectWPtr     parent,
                                                  QString            name,
                                                  int                size,
                                                  AllocationStrategy strat);

  static QcepDataColumnScanPtr  newColumnScan(QcepObjectWPtr     parent,
                                              QString            name,
                                              QStringList        cols,
                                              int                nRows,
                                              AllocationStrategy strat);

  static QcepDataColumnPtr      newColumn(QcepObjectWPtr     parent,
                                          QString            name,
                                          int                sz,
                                          AllocationStrategy strat);

  static QcepDataArrayPtr       newArray(QcepObjectWPtr     parent,
                                         QString            name,
                                         QVector<int>       dims,
                                         AllocationStrategy strat);

  static QcepDataGroupPtr       newGroup(QcepObjectWPtr parent,
                                         QString        name);

  static QcepDatasetPtr         newDataset(QcepObjectWPtr parent,
                                           QString        name);

  static qint64 allocatedMemoryMB();
  static qint64 availableMemoryMB();
  static qint64 availableMemory();

  static void changedAvailableBytes(qint64 newsize);

  static void allocate(qint64 sz, qint64 width, qint64 height);
  static void deallocate(qint64 sz, qint64 width, qint64 height);

  static void allocate(qint64 amt);
  static void deallocate(qint64 amt);

  static qint64 allocatedMemory();

  static qint64 int16Size(int width, int height);
  static qint64 int32Size(int width, int height);
  static qint64 doubleSize(int width, int height);
  static qint64 maskSize(int width, int height);
  static qint64 integratedSize(int nrows);
  static qint64 columnSize(int sz);
  static qint64 columnScanSize(int nCols, int nRows);
  static qint64 arraySize(QVector<int> dims);

private slots:
  void onMemorySizeChanged(qint64 newMB);

public:
  QMutex *mutex();

protected:
  void allocateBytes(qint64 amt);
  void deallocateBytes(qint64 amt);
  qint64 allocatedBytes();
  qint64 availableBytes();
  void setAvailableBytes(qint64 newsize);

//private slots:
//  void allocatorHeartbeat();

public:
  int waitTillAvailable(AllocationStrategy strat, qint64 size);

private:
  QMutex                m_Mutex;
//  QTimer                m_Timer;
//  QAtomicInt            m_AllocatedMemoryMB;
//  qint64                m_AllocatedMemory;
//  qint64                m_AvailableMemory;

public:
  enum { MegaBytes = 0x100000 };

  Q_PROPERTY(int     totalBufferSizeMB32    READ get_TotalBufferSizeMB32 WRITE set_TotalBufferSizeMB32)
  QCEP_INTEGER_PROPERTY(TotalBufferSizeMB32)

  Q_PROPERTY(int     totalBufferSizeMB64    READ get_TotalBufferSizeMB64 WRITE set_TotalBufferSizeMB64)
  QCEP_INTEGER_PROPERTY(TotalBufferSizeMB64)

  Q_PROPERTY(qint64     reserve             READ get_Reserve     WRITE set_Reserve)
  QCEP_INTEGER64_PROPERTY(Reserve)

  Q_PROPERTY(qint64     availableBytes      READ get_AvailableBytes WRITE set_AvailableBytes STORED false)
  QCEP_INTEGER64_PROPERTY(AvailableBytes)

  Q_PROPERTY(qint64     allocatedBytes      READ get_AllocatedBytes WRITE set_AllocatedBytes STORED false)
  QCEP_INTEGER64_PROPERTY(AllocatedBytes)
};

#endif // QCEPALLOCATOR_H
