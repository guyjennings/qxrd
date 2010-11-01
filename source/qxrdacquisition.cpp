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
    m_AcquiredInt32Data(NULL)
{
  connect(prop_ExposureTime(), SIGNAL(changedValue(double)), this, SLOT(onExposureTimeChanged(double)));
  connect(prop_BinningMode(), SIGNAL(changedValue(int)), this, SLOT(onBinningModeChanged(int)));
  connect(prop_CameraGain(), SIGNAL(changedValue(int)), this, SLOT(onCameraGainChanged(int)));

  if (sizeof(void*) == 4) {
    connect(prop_TotalBufferSizeMB32(), SIGNAL(changedValue(int)), this, SLOT(onBufferSizeChanged(int)));
  } else {
    connect(prop_TotalBufferSizeMB64(), SIGNAL(changedValue(int)), this, SLOT(onBufferSizeChanged(int)));
  }
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
//  double avail = get_TotalBufferSizeMB()*MegaBytes;
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

//  double avail1 = avail - optInt16*szInt16 - optInt32*szInt32;
//  double optNTh = floor(avail1/(szAcq + szDbl));

//  emit printMessage(tr("optInt16 = %1, optInt32 = %2, optThread = %3").arg(optInt16).arg(optInt32).arg(optNTh));

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
    emit statusMessage(QDateTime::currentDateTime(), "Starting acquisition");
    emit acquireStarted(0);

    acquisition(0);
  } else {
    emit statusMessage(QDateTime::currentDateTime(), "Acquisition is already in progress");
  }
}

void QxrdAcquisition::acquireDark()
{
  THREAD_CHECK;

//  printf("QxrdAcquisitionPerkinElmer::acquireDark (thread()=%p, currentThread()=%p)\n",
//         thread(), QThread::currentThread());
  if (m_Acquiring.tryLock()) {

    //  emit printMessage(tr("QxrdAcquisitionPerkinElmer::acquireDark()"));
    emit statusMessage(QDateTime::currentDateTime(), "Starting dark acquisition");
    emit acquireStarted(1);

    acquisition(1);
  } else {
    emit statusMessage(QDateTime::currentDateTime(), "Acquisition is already in progress");
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

  emit statusMessage(QDateTime::currentDateTime(), msg);
  emit printMessage(QDateTime::currentDateTime(), msg);

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
                 emit printMessage(QDateTime::currentDateTime(),
                                   tr("Frame %1 skipped").arg(m_CurrentExposure));
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
        m_OverflowMask = m_Allocator -> newMask();

        long nPixels = get_NRows()*get_NCols();
        int ovflwlvl = get_OverflowLevel();
        quint16* src = m_AcquiredInt16Data->data();
        quint32* dst = m_AcquiredInt32Data->data();
        short int* ovf = m_OverflowMask->data();

        if (m_CurrentExposure == 1) {
          QCEP_DEBUG(DEBUG_ACQUIRE,
                     emit printMessage(QDateTime::currentDateTime(),
                                       tr("Frame %1 saved").arg(m_CurrentExposure));
          );
          for (long i=0; i<nPixels; i++) {
            quint16 val = *src++;

            if (val>ovflwlvl) {
              *ovf++ = 2;
            } else {
              *ovf++ = 0;
            }

            *dst++ = val;
          }
        } else {
          QCEP_DEBUG(DEBUG_ACQUIRE,
                     emit printMessage(QDateTime::currentDateTime(),
                                       tr("Frame %1 summed").arg(m_CurrentExposure));
          );
          for (long i=0; i<nPixels; i++) {
            quint16 val = *src++;

            if (val>ovflwlvl) {
              *ovf++ |= 2;
            } else {
              ovf++;
            }

            *dst++ += val;
          }
        }
      }

      QString fileName;
      QString fileBase;

      if (m_CurrentExposure == get_ExposuresToSum()) {
        set_ExposuresToSkip(get_SkippedExposures());
        m_CurrentExposure.fetchAndStoreOrdered(0);


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

        //  emit printMessage(QDateTime::currentDateTime(),
        //                    tr("Fn: %1, Fi: %2, Exp: %3, Nexp %4, Fil: %5, NFil: %6")
        //                    .arg(fileName).arg(fileIndex())
        //                    .arg(m_CurrentExposure).arg(m_ExposuresToSum)
        //                    .arg(m_CurrentFile).arg(m_FilesInSequence));

        set_FileBase(fileBase);
//        m_DataProcessor -> set_FileName(fileName);

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
                       emit printMessage(QDateTime::currentDateTime(), tr("16 Bit Dark Image acquired"));
            );
            m_DataProcessor -> acquiredInt16Image(m_AcquiredInt16Data, m_OverflowMask);
//            replaceImageFromPool(m_AcquiredInt16Data);
            m_AcquiredInt16Data = m_Allocator -> newInt16Image();
            haltAcquisition();
          } else {
            if (get_PreTriggerFiles() > 0) {
              if (get_Trigger()) {
                while (m_PreTriggerInt16Images.size() > 0) {
                  QxrdInt16ImageDataPtr img = m_PreTriggerInt16Images.dequeue();

                  QCEP_DEBUG(DEBUG_ACQUIRE,
                             emit printMessage(QDateTime::currentDateTime(),
                                               tr("16 bit Pretrigger Image %1 acquired").arg(m_CurrentFile));
                  );
                  img -> set_ImageNumber(m_CurrentFile.fetchAndAddOrdered(1));
                  fileName = QDir(m_DataProcessor -> get_OutputDirectory())
                             .filePath(get_FilePattern()+tr("-%1.tif")
                                       .arg(get_FileIndex(),5,10,QChar('0')));
                  img -> set_FileName(fileName);
                  img -> set_Title(QFileInfo(fileName).fileName());

                  m_DataProcessor -> acquiredInt16Image(img, m_OverflowMask);
//                  returnImageToPool(img);
                  set_FileIndex(get_FileIndex()+1);
                }

                fileName = QDir(m_DataProcessor -> get_OutputDirectory())
                           .filePath(get_FilePattern()+tr("-%1.tif")
                                     .arg(get_FileIndex(),5,10,QChar('0')));
                m_AcquiredInt16Data -> set_FileName(fileName);
                m_AcquiredInt16Data -> set_Title(QFileInfo(fileName).fileName());
                QCEP_DEBUG(DEBUG_ACQUIRE,
                           emit printMessage(QDateTime::currentDateTime(),
                                             tr("16 bit Image %1 acquired").arg(m_CurrentFile));
                );
                m_AcquiredInt16Data -> set_ImageNumber(m_CurrentFile.fetchAndAddOrdered(1));
                m_DataProcessor -> acquiredInt16Image(m_AcquiredInt16Data, m_OverflowMask);
//                replaceImageFromPool(m_AcquiredInt16Data);
                m_AcquiredInt16Data = m_Allocator -> newInt16Image();
                set_FileIndex(get_FileIndex()+1);
              } else {
                m_PreTriggerInt16Images.enqueue(m_AcquiredInt16Data);
                QCEP_DEBUG(DEBUG_ACQUIRE,
                           emit printMessage(QDateTime::currentDateTime(),
                                             tr("16 bit Pretrigger Image buffered"));
                           emit statusMessage(QDateTime::currentDateTime(),
                                              tr("%1 pre trigger 16 bit images queued")
                                              .arg(m_PreTriggerInt16Images.size()));
                );
//                replaceImageFromPool(m_AcquiredInt16Data);

                while (m_PreTriggerInt16Images.size() > get_PreTriggerFiles()) {
                  m_PreTriggerInt16Images.dequeue();
                }
                m_AcquiredInt16Data = m_Allocator -> newInt16Image();
              }
            } else {
              QCEP_DEBUG(DEBUG_ACQUIRE,
                         emit printMessage(QDateTime::currentDateTime(),
                                           tr("16 bit Image %1 acquired").arg(m_CurrentFile));
              );
              m_AcquiredInt16Data -> set_ImageNumber(m_CurrentFile.fetchAndAddOrdered(1));
              m_DataProcessor -> acquiredInt16Image(m_AcquiredInt16Data, m_OverflowMask);
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
                       emit printMessage(QDateTime::currentDateTime(),
                                         tr("32 bit Dark Image acquired"));
            );
            m_DataProcessor -> acquiredInt32Image(m_AcquiredInt32Data, m_OverflowMask);
//            replaceImageFromPool(m_AcquiredInt32Data);
            m_AcquiredInt32Data = m_Allocator -> newInt32Image();
            haltAcquisition();
          } else {
            if (get_PreTriggerFiles() > 0) {
              if (get_Trigger()) {
                while (m_PreTriggerInt32Images.size() > 0) {
                  QxrdInt32ImageDataPtr img = m_PreTriggerInt32Images.dequeue();

                  QCEP_DEBUG(DEBUG_ACQUIRE,
                             emit printMessage(QDateTime::currentDateTime(),
                                               tr("32 bit Pretrigger Image %1 acquired").arg(m_CurrentFile));
                  );
                  img -> set_ImageNumber(m_CurrentFile.fetchAndAddOrdered(1));
                  fileName = QDir(m_DataProcessor -> get_OutputDirectory())
                             .filePath(get_FilePattern()+tr("-%1.tif")
                                       .arg(get_FileIndex(),5,10,QChar('0')));
                  img -> set_FileName(fileName);
                  img -> set_Title(QFileInfo(fileName).fileName());

                  m_DataProcessor -> acquiredInt32Image(img, m_OverflowMask);
//                  returnImageToPool(img);
                  set_FileIndex(get_FileIndex()+1);
                }

                fileName = QDir(m_DataProcessor -> get_OutputDirectory())
                           .filePath(get_FilePattern()+tr("-%1.tif")
                                     .arg(get_FileIndex(),5,10,QChar('0')));
                m_AcquiredInt32Data -> set_FileName(fileName);
                m_AcquiredInt32Data -> set_Title(QFileInfo(fileName).fileName());
                QCEP_DEBUG(DEBUG_ACQUIRE,
                           emit printMessage(QDateTime::currentDateTime(),
                                             tr("32 bit Image %1 acquired").arg(m_CurrentFile));
                );
                m_AcquiredInt32Data -> set_ImageNumber(m_CurrentFile.fetchAndAddOrdered(1));
                m_DataProcessor -> acquiredInt32Image(m_AcquiredInt32Data, m_OverflowMask);
//                replaceImageFromPool(m_AcquiredInt32Data);
                m_AcquiredInt32Data = m_Allocator -> newInt32Image();
                set_FileIndex(get_FileIndex()+1);
              } else {
                m_PreTriggerInt32Images.enqueue(m_AcquiredInt32Data);
                QCEP_DEBUG(DEBUG_ACQUIRE,
                           emit printMessage(QDateTime::currentDateTime(),
                                             tr("32 bit Pretrigger Image buffered"));
                           emit statusMessage(QDateTime::currentDateTime(),
                                              tr("%1 pre trigger 32 bit images queued")
                                              .arg(m_PreTriggerInt32Images.size()));
                );
//                replaceImageFromPool(m_AcquiredInt32Data);

                while (m_PreTriggerInt32Images.size() > get_PreTriggerFiles()) {
                  m_PreTriggerInt32Images.dequeue();
                }
                m_AcquiredInt32Data = m_Allocator -> newInt32Image();
              }
            } else {
              QCEP_DEBUG(DEBUG_ACQUIRE,
                         emit printMessage(QDateTime::currentDateTime(),
                                           tr("32 bit Image %1 acquired").arg(m_CurrentFile));
              );
              m_AcquiredInt32Data -> set_ImageNumber(m_CurrentFile.fetchAndAddOrdered(1));
              m_DataProcessor -> acquiredInt32Image(m_AcquiredInt32Data, m_OverflowMask);
//              replaceImageFromPool(m_AcquiredInt32Data);
              m_AcquiredInt32Data = m_Allocator -> newInt32Image();
              set_FileIndex(get_FileIndex()+1);
            }
          }
        }

        emit statusMessage(QDateTime::currentDateTime(),
                           tr("Acquiring ""%1"" (%2 i16, %3 i32, %4 dbl)").arg(fileName)
                           .arg(m_Allocator->nFreeInt16())
                           .arg(m_Allocator->nFreeInt32())
                           .arg(m_Allocator->nFreeDouble())
                            );

        if (m_CurrentFile >= (get_FilesInAcquiredSequence() + get_PreTriggerFiles())) {
          emit printMessage(QDateTime::currentDateTime(), "Acquisition ended");
          emit printMessage(QDateTime::currentDateTime(), "Halted acquisition");

          haltAcquisition();
        }
      }

      emit acquiredFrame(fileName, get_FileIndex(),
                         m_CurrentExposure,get_ExposuresToSum(),
                         m_CurrentFile, get_FilesInAcquiredSequence());
    }
  }
}

void QxrdAcquisition::haltAcquisition()
{
  set_Cancelling(true);

  emit statusMessage(QDateTime::currentDateTime(), "Acquire Complete");
  emit acquireComplete(get_AcquireDark());

  m_Acquiring.tryLock();
  m_Acquiring.unlock();

  m_StatusWaiting.wakeAll();
}

void QxrdAcquisition::acquisitionError(int n)
{
  haltAcquisition();

  emit criticalMessage(QDateTime::currentDateTime(), tr("Acquisition Error %1").arg(n));
}

void QxrdAcquisition::acquisitionError(int ln, int n)
{
  haltAcquisition();

  emit criticalMessage(QDateTime::currentDateTime(), tr("Acquisition Error %1 at line %2").arg(n).arg(ln));
}
