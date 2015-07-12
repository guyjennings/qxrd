#ifndef QCEPALLOCATOR_H
#define QCEPALLOCATOR_H

#include "qcepmacros.h"
#include "qcepobject.h"

#include <QMutex>
#include <QTimer>

#include "qcepproperty.h"

#include "qcepallocator-ptr.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"
#include "qcepintegrateddata-ptr.h"

class QcepAllocator : public QcepObject, public QEnableSharedFromThis<QcepAllocator>
{
  Q_OBJECT

public:
  QcepAllocator(QcepSettingsSaverPtr saver);
  virtual ~QcepAllocator();

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

public slots:
  void report();

public:
  QcepInt16ImageDataPtr newInt16Image(int width, int height);
  QcepInt32ImageDataPtr newInt32Image(int width, int height);
  QcepDoubleImageDataPtr newDoubleImage(int width, int height);
  QcepMaskDataPtr       newMask(int width, int height);
  QcepIntegratedDataPtr newIntegratedData(QcepDoubleImageDataPtr image);

  enum {
    AllocateInt16      = 0,
    AllocateInt32      = 1,
    AllocateDouble     = 2,
    AllocateMask       = 3,
    AllocateIntegrated = 4
  };

  enum AllocationStrategy {
    WaitTillAvailable,
    NullIfNotAvailable,
    AllocateFromReserve,
    AlwaysAllocate
  };

  static QcepInt16ImageDataPtr newInt16Image(AllocationStrategy strat, int width, int height);
  static QcepInt32ImageDataPtr newInt32Image(AllocationStrategy strat, int width, int height);
  static QcepDoubleImageDataPtr newDoubleImage(AllocationStrategy strat, int width, int height);
  static QcepMaskDataPtr newMask(AllocationStrategy strat, int width, int height, int def=1);
  static QcepIntegratedDataPtr newIntegratedData(AllocationStrategy strat, QcepDoubleImageDataPtr image);

  static QcepDoubleImageDataPtr newDoubleImage(QString name, int width, int height);
  static QcepIntegratedDataPtr  newIntegratedData(QString name, int size);

  static void newDoubleImageAndIntegratedData(AllocationStrategy strat,
                                       int width, int height,
                                       QcepDoubleImageDataPtr &img,
                                       QcepIntegratedDataPtr &integ);
  int int16SizeMB(int width, int height);
  int int32SizeMB(int width, int height);
  int doubleSizeMB(int width, int height);
  int maskSizeMB(int width, int height);
  int integratedSizeMB(int nrows);

  double allocatedMemoryMB();
  double allocatedMemory();
  double maximumMemoryMB();
  double maximumMemory();

  void changedSizeMB(int newMB);

  QMutex *mutex();

  void allocate(int typ, int sz, int width, int height);
  void allocate(int typ, quint64 amt);
  void deallocate(int typ, int sz, int width, int height);
  void deallocate(int typ, quint64 amt);

private slots:
  void allocatorHeartbeat();

private:
  int waitTillAvailable(AllocationStrategy strat, int sizeMB);

private:
  QMutex                m_Mutex;
  QTimer                m_Timer;
  quint64               m_AllocatedMemory;
  QAtomicInt            m_AllocatedMemoryMB;

public:
  enum { MegaBytes = 0x100000 };

  Q_PROPERTY(int     max        READ get_Max   WRITE set_Max)
  QCEP_INTEGER_PROPERTY(Max)

  Q_PROPERTY(int     totalBufferSizeMB32    READ get_TotalBufferSizeMB32 WRITE set_TotalBufferSizeMB32)
  QCEP_INTEGER_PROPERTY(TotalBufferSizeMB32)

  Q_PROPERTY(int     totalBufferSizeMB64    READ get_TotalBufferSizeMB64 WRITE set_TotalBufferSizeMB64)
  QCEP_INTEGER_PROPERTY(TotalBufferSizeMB64)

  Q_PROPERTY(int     reserve        READ get_Reserve   WRITE set_Reserve)
  QCEP_INTEGER_PROPERTY(Reserve)

  Q_PROPERTY(int     allocated        READ get_Allocated   WRITE set_Allocated STORED false)
  QCEP_INTEGER_PROPERTY(Allocated)

  Q_PROPERTY(int     queuedDelete      READ get_QueuedDelete WRITE set_QueuedDelete STORED false)
  QCEP_INTEGER_PROPERTY(QueuedDelete)

  Q_PROPERTY(int     nAllocatedInt16   READ get_NAllocatedInt16 WRITE set_NAllocatedInt16 STORED false)
  QCEP_INTEGER_PROPERTY(NAllocatedInt16)

  Q_PROPERTY(int     nAllocatedInt32   READ get_NAllocatedInt32 WRITE set_NAllocatedInt32 STORED false)
  QCEP_INTEGER_PROPERTY(NAllocatedInt32)

  Q_PROPERTY(int     nAllocatedDouble   READ get_NAllocatedDouble WRITE set_NAllocatedDouble STORED false)
  QCEP_INTEGER_PROPERTY(NAllocatedDouble)

  Q_PROPERTY(int     nAllocatedMask   READ get_NAllocatedMask WRITE set_NAllocatedMask STORED false)
  QCEP_INTEGER_PROPERTY(NAllocatedMask)

  Q_PROPERTY(int     nAllocatedIntegrated   READ get_NAllocatedIntegrated WRITE set_NAllocatedIntegrated STORED false)
  QCEP_INTEGER_PROPERTY(NAllocatedIntegrated)
};

#endif // QCEPALLOCATOR_H
