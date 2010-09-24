/******************************************************************
*
*  $Id: qxrdacquisition.cpp,v 1.3 2010/09/24 22:29:37 jennings Exp $
*
*******************************************************************/

#include "qxrdacquisition.h"
#include "qxrdmutexlocker.h"
#include "qxrddataprocessor.h"
#include "qxrdallocator.h"
#include <QThreadPool>

QxrdAcquisition::QxrdAcquisition(QxrdDataProcessorPtr proc, QxrdAllocatorPtr allocator)
  : QxrdAcquisitionOperations(proc, allocator),
    m_PreTriggerInt16Images("preTriggerInt16Images"),
    m_PreTriggerInt32Images("preTriggerInt32Images"),
    m_AcquiredInt16Data(NULL),
    m_AcquiredInt32Data(NULL),
    SOURCE_IDENT("$Id: qxrdacquisition.cpp,v 1.3 2010/09/24 22:29:37 jennings Exp $")
{
  connect(prop_ExposureTime(), SIGNAL(changedValue(double)), this, SLOT(onExposureTimeChanged(double)));
  connect(prop_BinningMode(), SIGNAL(changedValue(int)), this, SLOT(onBinningModeChanged(int)));
  connect(prop_CameraGain(), SIGNAL(changedValue(int)), this, SLOT(onCameraGainChanged(int)));
  connect(prop_TotalBufferSizeMB(), SIGNAL(changedValue(int)), this, SLOT(onBufferSizeChanged(int)));
}

QxrdAcquisition::~QxrdAcquisition()
{
  QCEP_DEBUG(DEBUG_ACQUIRE,
             printf("QxrdAcquisition::~QxrdAcquisition\n");
  );
}

void QxrdAcquisition::initialize()
{
//  allocateMemoryForAcquisition();

  m_AcquiredInt16Data = m_Allocator -> newInt16Image();
  m_AcquiredInt32Data = m_Allocator -> newInt32Image();
}

void QxrdAcquisition::onBufferSizeChanged(int newMB)
{
  m_Allocator -> changedSizeMB(newMB);
}

void QxrdAcquisition::allocateMemoryForAcquisition()
{
  THREAD_CHECK;

  int nThreads = QThreadPool::globalInstance()->maxThreadCount();
  int nRows = get_NRows(), nCols = get_NCols();
  int nSum = get_ExposuresToSum(), nPre = get_PreTriggerFiles();
  int optInt16 = 2, optInt32 = 1, optDbl = 2;
  double avail = get_TotalBufferSizeMB()*MegaBytes;
  double szInt16 = nRows*nCols*sizeof(quint16);
  double szInt32 = nRows*nCols*sizeof(quint32);
  double szDbl   = nRows*nCols*sizeof(double);
  int szAcq;

  if (nSum == 1) {
    optInt16 += nPre;
    szAcq = szInt16;
  } else {
    optInt32 += nPre;
    szAcq = szInt32;
  }

  double avail1 = avail - optInt16*szInt16 - optInt32*szInt32;
  double optNTh = floor(avail1/(szAcq + szDbl));

  emit printMessage(tr("optInt16 = %1, optInt32 = %2, optThread = %3").arg(optInt16).arg(optInt32).arg(optNTh));

  m_PreTriggerInt32Images.deallocate();
  m_PreTriggerInt16Images.deallocate();

  m_Allocator -> dimension(get_NCols(), get_NRows());
  m_Allocator -> preallocateInt16(optInt16);
  m_Allocator -> preallocateInt32(optInt32);

//  if (get_ExposuresToSum() == 1) {
//    int nMaxFrames = get_TotalBufferSizeMB()*MegaBytes/(get_NCols()*get_NRows()*sizeof(quint16));
//    int nFrames = qMin(get_FilesInAcquiredSequence()+get_PreTriggerFiles()+3, nMaxFrames);

//    emit printMessage(tr("Preallocating %1 %2 x %3 16 bit images").arg(nFrames).arg(get_NCols()).arg(get_NRows()));


//    emit printMessage(tr("Preallocated %1 %2 x %3 16 bit images").arg(nFrames).arg(get_NCols()).arg(get_NRows()));
//  } else {
//    int nMaxFrames = get_TotalBufferSizeMB()*MegaBytes/(get_NCols()*get_NRows()*sizeof(qint32));
//    int nFrames = qMin(get_FilesInAcquiredSequence()+get_PreTriggerFiles()+3, nMaxFrames);

//    emit printMessage(tr("Preallocating %1 %2 x %3 32 bit images").arg(nFrames).arg(get_NCols()).arg(get_NRows()));

//    m_Allocator -> dimension(get_NCols(), get_NRows());
//    m_Allocator -> preallocateInt16(3);
//    m_Allocator -> preallocateInt32(nFrames);

//    emit printMessage(tr("Preallocated %1 %2 x %3 32 bit images").arg(nFrames).arg(get_NCols()).arg(get_NRows()));
//  }
}

void QxrdAcquisition::acquire()
{
  THREAD_CHECK;

//  printf("QxrdAcquisitionPerkinElmer::acquire (thread()=%p, currentThread()=%p)\n",
//         thread(), QThread::currentThread());

  if (m_Acquiring.tryLock()) {
    //  emit printMessage(tr("QxrdAcquisitionPerkinElmer::acquire()"));
    emit statusMessage("Starting acquisition");
    emit acquireStarted(0);

    acquisition(0);
  } else {
    emit statusMessage("Acquisition is already in progress");
  }
}

void QxrdAcquisition::acquireDark()
{
  THREAD_CHECK;

//  printf("QxrdAcquisitionPerkinElmer::acquireDark (thread()=%p, currentThread()=%p)\n",
//         thread(), QThread::currentThread());
  if (m_Acquiring.tryLock()) {

    //  emit printMessage(tr("QxrdAcquisitionPerkinElmer::acquireDark()"));
    emit statusMessage("Starting dark acquisition");
    emit acquireStarted(1);

    acquisition(1);
  } else {
    emit statusMessage("Acquisition is already in progress");
  }
}

void QxrdAcquisition::cancel()
{
  INVOKE_CHECK(QMetaObject::invokeMethod(this, "haltAcquisition", Qt::QueuedConnection));
}

void QxrdAcquisition::cancelDark()
{
  INVOKE_CHECK(QMetaObject::invokeMethod(this, "haltAcquisition", Qt::QueuedConnection));
}

void QxrdAcquisition::trigger()
{
  set_Trigger(true);
}

void QxrdAcquisition::clearDropped()
{
  prop_DroppedFrames() -> setValue(0);
}

int  QxrdAcquisition::acquisitionStatus(double time)
{
  if (m_Acquiring.tryLock()) {
    m_Acquiring.unlock();

    //    printf("m_Acquiring.tryLock() succeeded\n");

    return 1;
  }

  QMutex mutex;
  QxrdMutexLocker lock(__FILE__, __LINE__, &mutex);

  if (m_StatusWaiting.wait(&mutex, (int)(time*1000))) {
    //    printf("m_StatusWaiting.wait succeeded\n");
    return 1;
  } else {
    //    printf("m_StatusWaiting.wait failed\n");
    return 0;
  }
}

void QxrdAcquisition::acquisition(int isDark)
{
  m_DataProcessor -> beginAcquisition(isDark);

  copyParameters(isDark);

  allocateMemoryForAcquisition();

  beginAcquisition();
}

void QxrdAcquisition::beginAcquisition()
{
}

void QxrdAcquisition::indicateDroppedFrame()
{
  QString msg = tr("Frame %1 dropped [%2/%3 MB Used]")
                  .arg(m_CurrentExposure)
                  .arg(m_Allocator->allocatedMemoryMB())
                  .arg(m_Allocator->maximumMemoryMB());

  emit statusMessage(msg);
  emit printMessage(msg);

  prop_DroppedFrames() -> incValue(1);
}

void QxrdAcquisition::copyParameters(int isDark)
{
  int skipAtStart = get_SkippedExposuresAtStart();

  if (get_AcquireDark() != isDark) {
    skipAtStart += 1;
  }

  set_AcquireDark(isDark);
  set_Trigger(0);

  set_Cancelling(0);
  m_CurrentExposure.fetchAndStoreOrdered(0);
  m_CurrentFile.fetchAndStoreOrdered(0);

  if (get_AcquireDark()) {
    set_ExposuresToSum(get_DarkSummedExposures());
    set_ExposuresToSkip(1);
    set_FilesInAcquiredSequence(1);
  } else {
    set_ExposuresToSum(get_SummedExposures());
    set_ExposuresToSkip(skipAtStart);
    set_FilesInAcquiredSequence(get_PostTriggerFiles());
  }

  if (get_FilesInAcquiredSequence ()<= 0) {
    set_FilesInAcquiredSequence(1);
  }
}

void QxrdAcquisition::acquiredFrameAvailable()
    // A new frame of data has been acquired, it is in m_AcquiredInt16Data.
    // If summation is required, it should be added to m_AcquiredInt32Data.
    // If a summed exposure has been completed, either the 16 or 32 bit data
    // frame should be passed to the data processor, and replaced with a new
    // image drawn from the free pool.
    // If m_AcquiredInt16Data is NULL, then the program has run out of free memory
    // and acquisition should drop frames until memory is available
{
  THREAD_CHECK;

  static int frameCounter = 0;
  static int updateInterval = 0;

  if (m_Acquiring.tryLock()) {
    m_Acquiring.unlock();

    if (m_AcquiredInt16Data) {
      updateInterval = 1.0/get_ExposureTime();

      if (updateInterval < 1) {
        updateInterval = 1;
      }

      frameCounter++;

      if ((frameCounter % updateInterval) == 0) {
        m_DataProcessor -> idleInt16Image(m_AcquiredInt16Data);
      }
    }

    m_AcquiredInt16Data = m_Allocator -> newInt16Image();
  } else if (m_AcquiredInt16Data == NULL) {
    indicateDroppedFrame();

    m_AcquiredInt16Data = m_Allocator -> newInt16Image();
  } else if (m_AcquiredInt32Data == NULL) {
    indicateDroppedFrame();

    m_AcquiredInt32Data = m_Allocator -> newInt32Image();
  } else  {
    m_CurrentExposure.fetchAndAddOrdered(1);

    if (m_CurrentExposure <= get_ExposuresToSkip()) {
      QCEP_DEBUG(DEBUG_ACQUIRE,
                 emit printMessage(tr("Frame %1 skipped").arg(m_CurrentExposure));
      );
      m_DataProcessor -> idleInt16Image(m_AcquiredInt16Data);
//      replaceImageFromPool(m_AcquiredInt16Data);
      m_AcquiredInt16Data = m_Allocator -> newInt16Image();
      if (m_CurrentExposure == get_ExposuresToSkip()) {
        set_ExposuresToSkip(0);
        m_CurrentExposure.fetchAndStoreOrdered(0);
      }
    } else {
      if (m_CurrentExposure <= get_ExposuresToSum()) {
        long nPixels = get_NRows()*get_NCols();
        quint16* src = m_AcquiredInt16Data->data();
        quint32* dst = m_AcquiredInt32Data->data();

        if (m_CurrentExposure == 1) {
          QCEP_DEBUG(DEBUG_ACQUIRE,
                     emit printMessage(tr("Frame %1 saved").arg(m_CurrentExposure));
          );
          for (long i=0; i<nPixels; i++) {
            *dst++ = *src++;
          }
        } else {
          QCEP_DEBUG(DEBUG_ACQUIRE,
                     emit printMessage(tr("Frame %1 summed").arg(m_CurrentExposure));
          );
          for (long i=0; i<nPixels; i++) {
            *dst++ += *src++;
          }
        }
      }

      if (m_CurrentExposure == get_ExposuresToSum()) {
        set_ExposuresToSkip(get_SkippedExposures());
        m_CurrentExposure.fetchAndStoreOrdered(0);

        QString fileName;
        QString fileBase;

        if (get_AcquireDark()) {
          fileBase = get_FilePattern()+tr("-%1.dark.tif").arg(get_FileIndex(),5,10,QChar('0'));
          fileName = QDir(m_DataProcessor -> get_OutputDirectory())
                     .filePath(get_FilePattern()+tr("-%1.dark.tif")
                               .arg(get_FileIndex(),5,10,QChar('0')));
        } else {
          fileBase = get_FilePattern()+tr("-%1.tif").arg(get_FileIndex(),5,10,QChar('0'));
          fileName = QDir(m_DataProcessor -> get_OutputDirectory())
                     .filePath(get_FilePattern()+tr("-%1.tif")
                               .arg(get_FileIndex(),5,10,QChar('0')));
        }

        //  emit printMessage(tr("Fn: %1, Fi: %2, Exp: %3, Nexp %4, Fil: %5, NFil: %6")
        //                    .arg(fileName).arg(fileIndex())
        //                    .arg(m_CurrentExposure).arg(m_ExposuresToSum)
        //                    .arg(m_CurrentFile).arg(m_FilesInSequence));

        set_FileBase(fileBase);
//        m_DataProcessor -> set_FileName(fileName);

        emit acquiredFrame(fileName, get_FileIndex(),
                           m_CurrentExposure,get_ExposuresToSum(),
                           m_CurrentFile, get_FilesInAcquiredSequence());

        QFileInfo finfo(fileName);

        if (get_ExposuresToSum() == 1) {
          m_AcquiredInt16Data -> set_FileName(fileName);
          m_AcquiredInt16Data -> set_Title(finfo.fileName());
          m_AcquiredInt16Data -> set_ExposureTime(get_ExposureTime());
          m_AcquiredInt16Data -> set_SummedExposures(get_ExposuresToSum());
          m_AcquiredInt16Data -> set_DateTime(QDateTime::currentDateTime());
          m_AcquiredInt16Data -> set_HBinning(1);
          m_AcquiredInt16Data -> set_VBinning(1);
          m_AcquiredInt16Data -> set_CameraGain(get_CameraGain());
          m_AcquiredInt16Data -> set_DataType(QxrdInt16ImageData::Raw16Data);
          m_AcquiredInt16Data -> set_Triggered(get_Trigger());
          m_AcquiredInt16Data -> set_UserComment1(get_UserComment1());
          m_AcquiredInt16Data -> set_UserComment2(get_UserComment2());
          m_AcquiredInt16Data -> set_UserComment3(get_UserComment3());
          m_AcquiredInt16Data -> set_UserComment4(get_UserComment4());
          m_AcquiredInt16Data -> set_ImageSaved(false);

          copyDynamicProperties(m_AcquiredInt16Data.data());

          if (get_AcquireDark()) {
            m_AcquiredInt16Data -> set_ImageNumber(-1);
            QCEP_DEBUG(DEBUG_ACQUIRE,
                       emit printMessage(tr("16 Bit Dark Image acquired"));
            );
            m_DataProcessor -> acquiredInt16Image(m_AcquiredInt16Data);
//            replaceImageFromPool(m_AcquiredInt16Data);
            m_AcquiredInt16Data = m_Allocator -> newInt16Image();
            haltAcquisition();
          } else {
            if (get_PreTriggerFiles() > 0) {
              if (get_Trigger()) {
                while (m_PreTriggerInt16Images.size() > 0) {
                  QxrdInt16ImageDataPtr img = m_PreTriggerInt16Images.dequeue();

                  QCEP_DEBUG(DEBUG_ACQUIRE,
                             emit printMessage(tr("16 bit Pretrigger Image %1 acquired").arg(m_CurrentFile));
                  );
                  img -> set_ImageNumber(m_CurrentFile.fetchAndAddOrdered(1));
                  fileName = QDir(m_DataProcessor -> get_OutputDirectory())
                             .filePath(get_FilePattern()+tr("-%1.tif")
                                       .arg(get_FileIndex(),5,10,QChar('0')));
                  img -> set_FileName(fileName);
                  img -> set_Title(QFileInfo(fileName).fileName());

                  m_DataProcessor -> acquiredInt16Image(img);
//                  returnImageToPool(img);
                  set_FileIndex(get_FileIndex()+1);
                }

                fileName = QDir(m_DataProcessor -> get_OutputDirectory())
                           .filePath(get_FilePattern()+tr("-%1.tif")
                                     .arg(get_FileIndex(),5,10,QChar('0')));
                m_AcquiredInt16Data -> set_FileName(fileName);
                m_AcquiredInt16Data -> set_Title(QFileInfo(fileName).fileName());
                QCEP_DEBUG(DEBUG_ACQUIRE,
                           emit printMessage(tr("16 bit Image %1 acquired").arg(m_CurrentFile));
                );
                m_AcquiredInt16Data -> set_ImageNumber(m_CurrentFile.fetchAndAddOrdered(1));
                m_DataProcessor -> acquiredInt16Image(m_AcquiredInt16Data);
//                replaceImageFromPool(m_AcquiredInt16Data);
                m_AcquiredInt16Data = m_Allocator -> newInt16Image();
                set_FileIndex(get_FileIndex()+1);
              } else {
                m_PreTriggerInt16Images.enqueue(m_AcquiredInt16Data);
                QCEP_DEBUG(DEBUG_ACQUIRE,
                           emit printMessage(tr("16 bit Pretrigger Image buffered"));
                           emit statusMessage(tr("%1 pre trigger 16 bit images queued").arg(m_PreTriggerInt16Images.size()));
                );
//                replaceImageFromPool(m_AcquiredInt16Data);

                while (m_PreTriggerInt16Images.size() > get_PreTriggerFiles()) {
                  m_PreTriggerInt16Images.dequeue();
                }
                m_AcquiredInt16Data = m_Allocator -> newInt16Image();
              }
            } else {
              QCEP_DEBUG(DEBUG_ACQUIRE,
                         emit printMessage(tr("16 bit Image %1 acquired").arg(m_CurrentFile));
              );
              m_AcquiredInt16Data -> set_ImageNumber(m_CurrentFile.fetchAndAddOrdered(1));
              m_DataProcessor -> acquiredInt16Image(m_AcquiredInt16Data);
//              replaceImageFromPool(m_AcquiredInt16Data);
              m_AcquiredInt16Data = m_Allocator -> newInt16Image();
              set_FileIndex(get_FileIndex()+1);
            }
          }
        } else {
          m_AcquiredInt32Data -> set_FileName(fileName);
          m_AcquiredInt32Data -> set_Title(finfo.fileName());
          m_AcquiredInt32Data -> set_ExposureTime(get_ExposureTime());
          m_AcquiredInt32Data -> set_SummedExposures(get_ExposuresToSum());
          m_AcquiredInt32Data -> set_DateTime(QDateTime::currentDateTime());
          m_AcquiredInt32Data -> set_HBinning(1);
          m_AcquiredInt32Data -> set_VBinning(1);
          m_AcquiredInt32Data -> set_CameraGain(get_CameraGain());
          m_AcquiredInt32Data -> set_DataType(QxrdInt32ImageData::Raw32Data);
          m_AcquiredInt32Data -> set_Triggered(get_Trigger());
          m_AcquiredInt32Data -> set_UserComment1(get_UserComment1());
          m_AcquiredInt32Data -> set_UserComment2(get_UserComment2());
          m_AcquiredInt32Data -> set_UserComment3(get_UserComment3());
          m_AcquiredInt32Data -> set_UserComment4(get_UserComment4());
          m_AcquiredInt32Data -> set_ImageSaved(false);

          copyDynamicProperties(m_AcquiredInt32Data.data());

          if (get_AcquireDark()) {
            m_AcquiredInt32Data -> set_ImageNumber(-1);
            QCEP_DEBUG(DEBUG_ACQUIRE,
                       emit printMessage(tr("32 bit Dark Image acquired"));
            );
            m_DataProcessor -> acquiredInt32Image(m_AcquiredInt32Data);
//            replaceImageFromPool(m_AcquiredInt32Data);
            m_AcquiredInt32Data = m_Allocator -> newInt32Image();
            haltAcquisition();
          } else {
            if (get_PreTriggerFiles() > 0) {
              if (get_Trigger()) {
                while (m_PreTriggerInt32Images.size() > 0) {
                  QxrdInt32ImageDataPtr img = m_PreTriggerInt32Images.dequeue();

                  QCEP_DEBUG(DEBUG_ACQUIRE,
                             emit printMessage(tr("32 bit Pretrigger Image %1 acquired").arg(m_CurrentFile));
                  );
                  img -> set_ImageNumber(m_CurrentFile.fetchAndAddOrdered(1));
                  fileName = QDir(m_DataProcessor -> get_OutputDirectory())
                             .filePath(get_FilePattern()+tr("-%1.tif")
                                       .arg(get_FileIndex(),5,10,QChar('0')));
                  img -> set_FileName(fileName);
                  img -> set_Title(QFileInfo(fileName).fileName());

                  m_DataProcessor -> acquiredInt32Image(img);
//                  returnImageToPool(img);
                  set_FileIndex(get_FileIndex()+1);
                }

                fileName = QDir(m_DataProcessor -> get_OutputDirectory())
                           .filePath(get_FilePattern()+tr("-%1.tif")
                                     .arg(get_FileIndex(),5,10,QChar('0')));
                m_AcquiredInt32Data -> set_FileName(fileName);
                m_AcquiredInt32Data -> set_Title(QFileInfo(fileName).fileName());
                QCEP_DEBUG(DEBUG_ACQUIRE,
                           emit printMessage(tr("32 bit Image %1 acquired").arg(m_CurrentFile));
                );
                m_AcquiredInt32Data -> set_ImageNumber(m_CurrentFile.fetchAndAddOrdered(1));
                m_DataProcessor -> acquiredInt32Image(m_AcquiredInt32Data);
//                replaceImageFromPool(m_AcquiredInt32Data);
                m_AcquiredInt32Data = m_Allocator -> newInt32Image();
                set_FileIndex(get_FileIndex()+1);
              } else {
                m_PreTriggerInt32Images.enqueue(m_AcquiredInt32Data);
                QCEP_DEBUG(DEBUG_ACQUIRE,
                           emit printMessage(tr("32 bit Pretrigger Image buffered"));
                           emit statusMessage(tr("%1 pre trigger 32 bit images queued").arg(m_PreTriggerInt32Images.size()));
                );
//                replaceImageFromPool(m_AcquiredInt32Data);

                while (m_PreTriggerInt32Images.size() > get_PreTriggerFiles()) {
                  m_PreTriggerInt32Images.dequeue();
                }
                m_AcquiredInt32Data = m_Allocator -> newInt32Image();
              }
            } else {
              QCEP_DEBUG(DEBUG_ACQUIRE,
                         emit printMessage(tr("32 bit Image %1 acquired").arg(m_CurrentFile));
              );
              m_AcquiredInt32Data -> set_ImageNumber(m_CurrentFile.fetchAndAddOrdered(1));
              m_DataProcessor -> acquiredInt32Image(m_AcquiredInt32Data);
//              replaceImageFromPool(m_AcquiredInt32Data);
              m_AcquiredInt32Data = m_Allocator -> newInt32Image();
              set_FileIndex(get_FileIndex()+1);
            }
          }
        }

        emit statusMessage(tr("Acquiring ""%1"" (%2 i16, %3 i32, %4 dbl)").arg(fileName)
                           .arg(m_Allocator->nFreeInt16())
                           .arg(m_Allocator->nFreeInt32())
                           .arg(m_Allocator->nFreeDouble())
                            );

        if (m_CurrentFile >= (get_FilesInAcquiredSequence() + get_PreTriggerFiles())) {
          emit printMessage("Acquisition ended");
          emit printMessage("Halted acquisition");

          haltAcquisition();
        }
      }
    }
  }
}

void QxrdAcquisition::haltAcquisition()
{
  set_Cancelling(true);

  emit statusMessage("Acquire Complete");
  emit acquireComplete(get_AcquireDark());

  m_Acquiring.tryLock();
  m_Acquiring.unlock();

  m_StatusWaiting.wakeAll();
}

void QxrdAcquisition::acquisitionError(int n)
{
  haltAcquisition();

  emit criticalMessage(tr("Acquisition Error %1").arg(n));
}

void QxrdAcquisition::acquisitionError(int ln, int n)
{
  haltAcquisition();

  emit criticalMessage(tr("Acquisition Error %1 at line %2").arg(n).arg(ln));
}


/******************************************************************
*
*  $Log: qxrdacquisition.cpp,v $
*  Revision 1.3  2010/09/24 22:29:37  jennings
*  Work on NSIS installer
*  Fixed startup problem on debug builds when calling QxrdAcquisitionThread->initialize()
*
*  Revision 1.2  2010/09/13 20:00:38  jennings
*  Merged
*
*  Revision 1.1.2.3  2010/09/09 16:54:24  jennings
*  Only call idle image calculations at most every 3 seconds
*
*  Revision 1.1.2.2  2010/08/17 19:20:50  jennings
*  Added INVOKE_CHECK macro to check returned result QMetaObject::invokeMethod calls
*
*  Revision 1.1.2.1  2010/07/22 18:39:36  jennings
*  Moving files into source subdirectory
*
*  Revision 1.53.4.25  2010/07/09 21:32:18  jennings
*  Tried to improve memory management by predicting memory requirements better
*
*  Revision 1.53.4.24  2010/06/23 21:42:36  jennings
*  Added clear dropped frames counter slot,
*
*  Revision 1.53.4.23  2010/06/22 23:37:33  jennings
*  Made most acquisition and processing output conditional
*
*  Revision 1.53.4.22  2010/06/17 19:19:59  jennings
*  Added 'save dark' and 'skip at start' options
*
*  Revision 1.53.4.21  2010/06/17 18:37:13  jennings
*  Added dropped frames counter to display window
*
*  Revision 1.53.4.20  2010/06/10 20:55:22  jennings
*  Improved behaviour when acquisition is interrupted - if acquisition is already running when a new acquire operation is started, give an error message
*
*  Revision 1.53.4.19  2010/05/28 21:44:08  jennings
*  Fixed problems with cancelling acquisition
*
*  Revision 1.53.4.18  2010/05/27 21:47:50  jennings
*  Renamed totalBufferSize parameter to totalBufferSizeMB to avoid conflicts with qxrd 0.3.x which interprets
*  totalBufferSize in bytes, rather than MB
*
*  Revision 1.53.4.17  2010/05/25 18:47:15  jennings
*  Added memory limit handling
*
*  Revision 1.53.4.16  2010/05/24 21:02:37  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.53.4.15  2010/05/21 20:03:06  jennings
*  Memory size parameter units changed to MB
*
*  Revision 1.53.4.14  2010/05/19 21:15:34  jennings
*  PE acquisition debugging
*
*  Revision 1.53.4.13  2010/05/17 15:59:52  jennings
*  Changed debugging output to use QCEP_DEBUG macro
*
*  Revision 1.53.4.12  2010/05/02 16:56:50  jennings
*  Removed embedded C comments from commit log
*
*  Revision 1.53.4.11  2010/05/02 08:12:06  jennings
*  Replaced 'returnImageToPool' and 'replaceImageFromPool' by
*  equivalent smart pointer assignments
*
*  Revision 1.53.4.10  2010/04/26 00:37:09  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.53.4.9  2010/04/23 21:26:32  jennings
*  Rearranged message output
*
*  Revision 1.53.4.8  2010/04/22 20:56:25  jennings
*  Simulated acquisition is beginning to work
*
*  Revision 1.53.4.7  2010/04/22 19:40:33  jennings
*  Further rearrangement of acquisition code
*
*  Revision 1.53.4.6  2010/04/20 21:18:33  jennings
*  More rearrangement
*
*  Revision 1.53.4.5  2010/04/19 21:48:26  jennings
*  More rearrangement
*
*  Revision 1.53.4.4  2010/04/19 19:20:16  jennings
*  More reorganization of acquisition code
*
*  Revision 1.53.4.3  2010/04/17 05:13:58  jennings
*  Partially working
*
*  Revision 1.53.4.2  2010/04/15 19:34:24  jennings
*  Reorganization of acquisition object classes
*
*  Revision 1.53.4.1  2010/04/12 21:21:46  jennings
*  Version 0.4.0, some rearrangement of the Acquisition classes
*
*  Revision 1.53  2009/09/04 15:15:42  jennings
*  Added log file routines
*  Removed newlines from any printMessage calls.
*
*  Revision 1.52  2009/07/20 00:32:04  jennings
*  Removed image queues for acquired and dark images - use 'connect' args instead
*
*  Revision 1.51  2009/07/14 05:36:14  jennings
*  Moved data storage into individual channel objects, accelerated fitting (somewhat)
*
*  Revision 1.50  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.49  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.48  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

