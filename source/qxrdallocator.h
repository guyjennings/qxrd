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
    NullIfNotAvailable
  };

  QxrdInt16ImageDataPtr newInt16Image(AllocationStrategy strat);
  QxrdInt32ImageDataPtr newInt32Image(AllocationStrategy strat);
  QxrdDoubleImageDataPtr newDoubleImage(AllocationStrategy strat);
  QxrdMaskDataPtr newMask(AllocationStrategy strat, int def=1);
  QxrdIntegratedDataPtr newIntegratedData(AllocationStrategy strat, QxrdDoubleImageDataPtr image);

  void dimension(int width, int height);
//  void preallocateInt16(int n16);
//  void preallocateInt32(int n32);
//  void preallocateDouble(int ndbl);

//  int nFreeInt16();
//  int nFreeInt32();
//  int nFreeDouble();

  int int16SizeMB();
  int int32SizeMB();
  int doubleSizeMB();
  int maskSizeMB();
  int integratedSizeMB(int nrows);

  double allocatedMemoryMB();
  double allocatedMemory();
  double maximumMemoryMB();
  double maximumMemory();

  void changedSizeMB(int newMB);

signals:
  void printMessage(QDateTime ts, QString msg);
  void statusMessage(QDateTime ts, QString msg);
  void criticalMessage(QDateTime ts, QString msg);

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
  QAtomicInt            m_AllocatedMemoryMB;
  quint64               m_AllocatedMemory;

//  QAtomicInt            m_CountInt16;
//  QAtomicInt            m_CountInt32;
//  QAtomicInt            m_CountDouble;
//  QAtomicInt            m_PreallocateInt16;
//  QAtomicInt            m_PreallocateInt32;
//  QAtomicInt            m_PreallocateDouble;

//  QxrdInt16ImageQueue   m_FreeInt16Images;
//  QxrdInt32ImageQueue   m_FreeInt32Images;
//  QxrdDoubleImageQueue  m_FreeDoubleImages;
//  QxrdMaskQueue         m_FreeMasks;
//  QxrdIntegratedDataQueue m_FreeIntegratedData;

  enum { MegaBytes = 0x100000 };

  Q_PROPERTY(int     max        READ get_Max   WRITE set_Max STORED false);
  QCEP_INTEGER_PROPERTY(Max);

  Q_PROPERTY(int     allocated        READ get_Allocated   WRITE set_Allocated STORED false);
  QCEP_INTEGER_PROPERTY(Allocated);

  Q_PROPERTY(int     width      READ get_Width WRITE set_Width STORED false);
  QCEP_INTEGER_PROPERTY(Width);

  Q_PROPERTY(int     height      READ get_Height WRITE set_Height STORED false);
  QCEP_INTEGER_PROPERTY(Height);
};

#endif
