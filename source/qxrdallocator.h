/******************************************************************
*
*  $Id: qxrdallocator.h,v 1.3 2010/10/21 16:31:24 jennings Exp $
*
*******************************************************************/

#ifndef QXRDALLOCATOR_H
#define QXRDALLOCATOR_H

#include "qcepmacros.h"

//#include <QObject>
//#include <QReadWriteLock>
//#include <QAtomicInt>
//#include <QWaitCondition>
#include <QMutex>
#include <QTimer>

//#include "qxrdforwardtypes.h"
#include "qcepproperty.h"
//#include "qxrdsettings.h"
#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include "qxrddoubleimagedata.h"
#include "qxrdintegrateddata.h"
//#include "qxrdacquisition.h"
#include "qxrdintegrateddataqueue.h"
#include "qxrdimagequeue.h"

class QxrdAllocator : public QxrdAllocatorInterface
{
  Q_OBJECT;

public:
  QxrdAllocator(/*QxrdAcquisitionPtr acq,*/ QObject *parent=0);
  virtual ~QxrdAllocator();

public:
  QxrdInt16ImageDataPtr newInt16Image();
  QxrdInt32ImageDataPtr newInt32Image();
  QxrdDoubleImageDataPtr newDoubleImage();
  QxrdMaskDataPtr newMask();
  QxrdIntegratedDataPtr newIntegratedData(QxrdDoubleImageDataPtr image);

  void dimension(int width, int height);
  void preallocateInt16(int n16);
  void preallocateInt32(int n32);
  void preallocateDouble(int ndbl);

  void allocate(int sz, int width, int height);
  void deallocate(int sz, int width, int height);

  int nFreeInt16();
  int nFreeInt32();
  int nFreeDouble();

  int int16SizeMB();
  int int32SizeMB();
  int doubleSizeMB();

  double allocatedMemoryMB();
  double allocatedMemory();
  double maximumMemoryMB();
  double maximumMemory();

  void changedSizeMB(int newMB);

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

private slots:
  void allocatorHeartbeat();

private:
  static void maskDeleter(QxrdMaskData *mask);
  static void int16Deleter(QxrdInt16ImageData *img);
  static void int32Deleter(QxrdInt32ImageData *img);
  static void doubleDeleter(QxrdDoubleImageData *img);
  static void integratedDeleter(QxrdIntegratedData *integ);

private:
//  QxrdAcquisitionPtr    m_Acquisition;
  QMutex                m_Mutex;
  QTimer                m_Timer;
  QAtomicInt            m_AllocatedMemory;
  QAtomicInt            m_CountInt16;
  QAtomicInt            m_CountInt32;
  QAtomicInt            m_CountDouble;
  QAtomicInt            m_PreallocateInt16;
  QAtomicInt            m_PreallocateInt32;
  QAtomicInt            m_PreallocateDouble;

  QxrdInt16ImageQueue   m_FreeInt16Images;
  QxrdInt32ImageQueue   m_FreeInt32Images;
  QxrdDoubleImageQueue  m_FreeDoubleImages;
  QxrdMaskQueue         m_FreeMasks;
  QxrdIntegratedDataQueue m_FreeIntegratedData;

  enum { MegaBytes = 0x100000 };

  Q_PROPERTY(int     max        READ get_Max   WRITE set_Max STORED false);
  QCEP_INTEGER_PROPERTY(Max);

  Q_PROPERTY(int     allocated        READ get_Allocated   WRITE set_Allocated STORED false);
  QCEP_INTEGER_PROPERTY(Allocated);

  Q_PROPERTY(int     width      READ get_Width WRITE set_Width STORED false);
  QCEP_INTEGER_PROPERTY(Width);

  Q_PROPERTY(int     height      READ get_Height WRITE set_Height STORED false);
  QCEP_INTEGER_PROPERTY(Height);

  HEADER_IDENT("$Id: qxrdallocator.h,v 1.3 2010/10/21 16:31:24 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdallocator.h,v $
*  Revision 1.3  2010/10/21 16:31:24  jennings
*  Implemented saving of settings soon after they change, rather than at program exit
*
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:37  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.12  2010/07/20 20:30:25  jennings
*  Added memory usage display to status bar
*  Improved calculation of processing timings
*
*  Revision 1.1.2.11  2010/07/09 21:33:22  jennings
*  Tried to improve memory management by predicting memory requirements better
*  The allocators are more accurate in limiting allocated memory
*
*  Revision 1.1.2.10  2010/06/23 21:49:35  jennings
*  Made allocator strategy more sophisticated, called it more often
*
*  Revision 1.1.2.9  2010/06/18 16:28:22  jennings
*  Added debugging output to QxrdAllocator constructor/destructor
*
*  Revision 1.1.2.8  2010/06/17 16:02:35  jennings
*  Skeleton code to support result serialization during crucial points in processing
*  Separate QxrdDoubleImageData class which can hold a reference to a mask
*
*  Revision 1.1.2.7  2010/06/14 20:57:21  jennings
*  *** empty log message ***
*
*  Revision 1.1.2.6  2010/06/11 21:21:39  jennings
*  Added integrator data to allocator
*
*  Revision 1.1.2.5  2010/05/25 20:42:54  jennings
*  Added mutex to allocator object
*
*  Revision 1.1.2.4  2010/05/25 18:47:15  jennings
*  Added memory limit handling
*
*  Revision 1.1.2.3  2010/05/24 21:02:38  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.1.2.2  2010/05/22 03:43:03  jennings
*  APIs for allocator
*
*  Revision 1.1.2.1  2010/05/20 20:15:54  jennings
*  Initial files for QxrdAllocator and QxrdFileSaver threads
*
*
*
*******************************************************************/

