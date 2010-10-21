/******************************************************************
*
*  $Id: qxrdacquisition.h,v 1.3 2010/10/21 19:44:02 jennings Exp $
*
*******************************************************************/

#ifndef QXRDACQUISITION_H
#define QXRDACQUISITION_H

#include "qcepmacros.h"

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QAtomicInt>

#include "qxrdrasterdata.h"
#include "qxrdimagequeue.h"
#include "qxrdacquisitionoperations.h"

class QxrdAcquisition : public QxrdAcquisitionOperations
{
  Q_OBJECT;

public:
  QxrdAcquisition(QxrdDataProcessorPtr proc, QxrdAllocatorPtr allocator);
  ~QxrdAcquisition();

public slots:
  virtual void initialize();

  void acquire();
  void acquireDark();
  void cancel();
  void cancelDark();
  void trigger();
  void clearDropped();

  int acquisitionStatus(double time);

  virtual void onExposureTimeChanged(double newTime) = 0;
  virtual void onBinningModeChanged(int newMode) = 0;
  virtual void onCameraGainChanged(int newGain) = 0;
  void onBufferSizeChanged(int newMB);

signals:
  void acquiredFrame(QString fileName, int index, int isum, int nsum, int iframe, int nframe);
  void acquireStarted(int dark);
  void acquireComplete(int dark);

public:
  virtual void setupExposureMenu(QDoubleSpinBox *cb) = 0;
  virtual void setupCameraGainMenu(QComboBox *cb) = 0;
  virtual void setupCameraBinningModeMenu(QComboBox *cb) = 0;

//  void returnImageToPool(QxrdInt16ImageDataPtr img);
//  void returnImageToPool(QxrdInt32ImageDataPtr img);
//  void replaceImageFromPool(QxrdInt16ImageDataPtr &ptr);
//  void replaceImageFromPool(QxrdInt32ImageDataPtr &ptr);

  void indicateDroppedFrame();

protected:
  void allocateMemoryForAcquisition();
  void acquisition(int isDark);
  void copyParameters(int isDark);
  virtual void beginAcquisition();
  void acquiredFrameAvailable();

  void acquisitionError(int n);
  void acquisitionError(int ln, int n);

protected slots:
  virtual void haltAcquisition();

protected:
  QMutex                 m_Acquiring;
  QWaitCondition         m_StatusWaiting;

//  QxrdInt16ImageQueue    m_FreeInt16Images;
//  QxrdInt32ImageQueue    m_FreeInt32Images;
  QxrdInt16ImageQueue    m_PreTriggerInt16Images;
  QxrdInt32ImageQueue    m_PreTriggerInt32Images;
  QxrdInt16ImageDataPtr  m_AcquiredInt16Data;
  QxrdInt32ImageDataPtr  m_AcquiredInt32Data;
  QxrdMaskDataPtr        m_OverflowMask;

  QAtomicInt             m_AcquireDark;
  QAtomicInt             m_NFramesStillToSkip;
  QAtomicInt             m_NFramesStillToSum;
  QAtomicInt             m_NPretriggerAcquired;
  QAtomicInt             m_NPostTriggerAcquired;
  QAtomicInt             m_CurrentExposure;
  QAtomicInt             m_CurrentFile;

  HEADER_IDENT("$Id: qxrdacquisition.h,v 1.3 2010/10/21 19:44:02 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdacquisition.h,v $
*  Revision 1.3  2010/10/21 19:44:02  jennings
*  Adding code to display overflow pixels, removed cuda and simple processors
*
*  Revision 1.2  2010/09/13 20:00:38  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:36  jennings
*  Moving files into source subdirectory
*
*  Revision 1.36.4.17  2010/07/09 21:32:18  jennings
*  Tried to improve memory management by predicting memory requirements better
*
*  Revision 1.36.4.16  2010/06/23 21:43:10  jennings
*  Added clear dropped frames slot
*
*  Revision 1.36.4.15  2010/05/28 21:44:08  jennings
*  Fixed problems with cancelling acquisition
*
*  Revision 1.36.4.14  2010/05/24 21:02:37  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.36.4.13  2010/05/19 21:15:34  jennings
*  PE acquisition debugging
*
*  Revision 1.36.4.12  2010/05/02 16:56:50  jennings
*  Removed embedded C comments from commit log
*
*  Revision 1.36.4.11  2010/05/02 08:12:06  jennings
*  Replaced 'returnImageToPool' and 'replaceImageFromPool' by
*  equivalent smart pointer assignments
*
*  Revision 1.36.4.10  2010/04/26 23:46:12  jennings
*  *** empty log message ***
*
*  Revision 1.36.4.9  2010/04/26 20:53:25  jennings
*  More attempts to get QSharedPointers to work...
*
*  Revision 1.36.4.8  2010/04/26 00:37:09  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.36.4.7  2010/04/22 19:40:33  jennings
*  Further rearrangement of acquisition code
*
*  Revision 1.36.4.6  2010/04/20 21:18:33  jennings
*  More rearrangement
*
*  Revision 1.36.4.5  2010/04/19 21:48:26  jennings
*  More rearrangement
*
*  Revision 1.36.4.4  2010/04/19 19:20:16  jennings
*  More reorganization of acquisition code
*
*  Revision 1.36.4.3  2010/04/17 05:13:58  jennings
*  Partially working
*
*  Revision 1.36.4.2  2010/04/15 19:34:24  jennings
*  Reorganization of acquisition object classes
*
*  Revision 1.36.4.1  2010/04/12 21:21:46  jennings
*  Version 0.4.0, some rearrangement of the Acquisition classes
*
*  Revision 1.36  2009/07/14 05:36:14  jennings
*  Moved data storage into individual channel objects, accelerated fitting (somewhat)
*
*  Revision 1.35  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.34  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.33  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

