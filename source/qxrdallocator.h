#ifndef QXRDALLOCATOR_H
#define QXRDALLOCATOR_H

#include "qcepmacros.h"

#include <QMutex>
#include <QTimer>

#include "qcepproperty.h"
#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include "qxrdintegrateddata.h"
#include "qxrdintegrateddataqueue.h"
#include "qxrdimagequeue.h"

class QxrdAllocator : public QxrdAllocatorInterface
{
  Q_OBJECT;

public:
  QxrdAllocator(QObject *parent=0);
  virtual ~QxrdAllocator();

public:
  enum AllocationStrategy {
    WaitTillAvailable,
    NullIfNotAvailable,
    AllocateFromReserve,
    AlwaysAllocate
  };

  QxrdInt16ImageDataPtr newInt16Image(AllocationStrategy strat, int width, int height);
  QxrdInt32ImageDataPtr newInt32Image(AllocationStrategy strat, int width, int height);
  QxrdDoubleImageDataPtr newDoubleImage(AllocationStrategy strat, int width, int height);
  QxrdMaskDataPtr newMask(AllocationStrategy strat, int width, int height, int def=1);
  QxrdIntegratedDataPtr newIntegratedData(AllocationStrategy strat, QxrdDoubleImageDataPtr image);

  void newDoubleImageAndIntegratedData(AllocationStrategy strat,
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

private slots:
  void allocatorHeartbeat();

private:
  static void maskDeleter(QxrdMaskData *mask);
  static void int16Deleter(QxrdInt16ImageData *img);
  static void int32Deleter(QxrdInt32ImageData *img);
  static void doubleDeleter(QxrdDoubleImageData *img);
  static void integratedDeleter(QxrdIntegratedData *integ);
  int waitTillAvailable(AllocationStrategy strat, int sizeMB);

  void allocate(int sz, int width, int height);
  void allocate(quint64 amt);
  void deallocate(int sz, int width, int height);
  void deallocate(quint64 amt);

private:
  QMutex                m_Mutex;
  QTimer                m_Timer;
  quint64               m_AllocatedMemory;
  QAtomicInt            m_AllocatedMemoryMB;

  enum { MegaBytes = 0x100000 };

  Q_PROPERTY(int     max        READ get_Max   WRITE set_Max STORED false);
  QCEP_INTEGER_PROPERTY(Max);

  Q_PROPERTY(int     reserve        READ get_Reserve   WRITE set_Reserve STORED false);
  QCEP_INTEGER_PROPERTY(Reserve);

  Q_PROPERTY(int     allocated        READ get_Allocated   WRITE set_Allocated STORED false);
  QCEP_INTEGER_PROPERTY(Allocated);
};

#endif
