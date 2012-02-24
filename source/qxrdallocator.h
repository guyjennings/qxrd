#ifndef QXRDALLOCATOR_H
#define QXRDALLOCATOR_H

#include "qcepmacros.h"
#include "qxrdallocator.h"

#include <QMutex>
#include <QTimer>
#include "qcepproperty.h"
#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include "qxrdintegrateddata.h"
#include "qxrdintegrateddataqueue.h"
#include "qxrdimagequeue.h"

class QxrdAllocator : public QObject
{
  Q_OBJECT

public:
  QxrdAllocator(QxrdSettingsSaverPtr saver);
  virtual ~QxrdAllocator();

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

public:
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

  static QxrdInt16ImageDataPtr newInt16Image(QxrdAllocatorWPtr allocw, AllocationStrategy strat, int width, int height);
  static QxrdInt32ImageDataPtr newInt32Image(QxrdAllocatorWPtr allocw, AllocationStrategy strat, int width, int height);
  static QxrdDoubleImageDataPtr newDoubleImage(QxrdAllocatorWPtr allocw, AllocationStrategy strat, int width, int height);
  static QxrdMaskDataPtr newMask(QxrdAllocatorWPtr allocw, AllocationStrategy strat, int width, int height, int def=1);
  static QxrdIntegratedDataPtr newIntegratedData(QxrdAllocatorWPtr allocw, AllocationStrategy strat, QxrdDoubleImageDataPtr image);

  static void newDoubleImageAndIntegratedData(QxrdAllocatorWPtr allocw,
                                              AllocationStrategy strat,
                                       int width, int height,
                                       QxrdDoubleImageDataPtr &img,
                                       QxrdIntegratedDataPtr &integ);

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

public slots:
  void report();

private slots:
  void allocatorHeartbeat();

private:
  static void maskDeleter(QxrdMaskData *mask);
  static void int16Deleter(QxrdInt16ImageData *img);
  static void int32Deleter(QxrdInt32ImageData *img);
  static void doubleDeleter(QxrdDoubleImageData *img);
  static void integratedDeleter(QxrdIntegratedData *integ);
  int waitTillAvailable(AllocationStrategy strat, int sizeMB);


private:
  QMutex                m_Mutex;
  QTimer                m_Timer;
  quint64               m_AllocatedMemory;
  QAtomicInt            m_AllocatedMemoryMB;

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

#endif

#include <QWeakPointer>
#include <QSharedPointer>

class QxrdAllocator;
typedef QSharedPointer<QxrdAllocator> QxrdAllocatorPtr;
typedef QWeakPointer<QxrdAllocator> QxrdAllocatorWPtr;
