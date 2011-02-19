#include "qxrdacquisition.h"
#include "qxrdmutexlocker.h"
#include "qxrddataprocessor.h"
#include "qxrdallocator.h"
#include "qxrdacquiredialog.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdwindow.h"

#include <QThreadPool>

QxrdAcquisition::QxrdAcquisition(QxrdDataProcessorPtr proc, QxrdAllocatorPtr allocator)
  : QxrdAcquisitionOperations(proc, allocator),
    m_AcquiredInt32Data(),
    m_NSkippedAtStart(0),
    m_NSkippedBetweenGroups(0),
    m_NPhasesPerSummation(1),
    m_NSummationsPerGroup(1),
    m_NGroupsPerSequence(1),
    m_FrameCounter(0),
    m_ControlPanel(NULL),
    m_NIDAQPlugin(NULL),
    m_SynchronizedAcquisition(new QxrdSynchronizedAcquisition(this))
{
  connect(prop_ExposureTime(), SIGNAL(changedValue(double)), this, SLOT(onExposureTimeChanged(double)));
  connect(prop_BinningMode(), SIGNAL(changedValue(int)), this, SLOT(onBinningModeChanged(int)));
  connect(prop_CameraGain(), SIGNAL(changedValue(int)), this, SLOT(onCameraGainChanged(int)));

  if (sizeof(void*) == 4) {
    connect(prop_TotalBufferSizeMB32(), SIGNAL(changedValue(int)), this, SLOT(onBufferSizeChanged(int)));
    onBufferSizeChanged(get_TotalBufferSizeMB32());
  } else {
    connect(prop_TotalBufferSizeMB64(), SIGNAL(changedValue(int)), this, SLOT(onBufferSizeChanged(int)));
    onBufferSizeChanged(get_TotalBufferSizeMB64());
  }

  m_ElapsedHistogram.resize(21);
  m_ElapsedHistogram.fill(0);
  m_ElapsedCounter = 0;
  m_ElapsedTimer.start();
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

//  m_AcquiredInt16Data    = m_Allocator -> newInt16Image();
//  m_AcquiredInt32Data[0] = m_Allocator -> newInt32Image();
}

void QxrdAcquisition::onBufferSizeChanged(int newMB)
{
  m_Allocator -> changedSizeMB(newMB);
}

void QxrdAcquisition::allocateMemoryForAcquisition()
{
  THREAD_CHECK;

  m_AcquiredInt32Data.clear();

  m_Allocator -> dimension(get_NCols(), get_NRows());
  m_Allocator -> preallocateInt16(2);
  m_Allocator -> preallocateInt32(3+m_NPhasesPerSummation);

  m_AcquiredInt32Data.resize(m_NPhasesPerSummation);
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
  m_ElapsedTimer.start();
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

  set_Cancelling(0);

  m_CurrentExposure.fetchAndStoreOrdered(0);
  m_CurrentPhase.fetchAndStoreOrdered(0);
  m_CurrentSummation.fetchAndStoreOrdered(0);
  m_CurrentGroup.fetchAndStoreOrdered(0);

  m_NSkippedAtStart.fetchAndStoreOrdered(get_SkippedExposuresAtStart());
  m_NSkippedBetweenGroups.fetchAndStoreOrdered(get_SkippedExposures());

  if (get_AcquireDark()) {
    m_NPhasesPerSummation.fetchAndStoreOrdered(1);
    m_NSummationsPerGroup.fetchAndStoreOrdered(get_DarkSummedExposures());
    m_NGroupsPerSequence.fetchAndStoreOrdered(1);
  } else {
    m_NPhasesPerSummation.fetchAndStoreOrdered(get_FilesInGroup());
    m_NSummationsPerGroup.fetchAndStoreOrdered(get_SummedExposures());
    m_NGroupsPerSequence.fetchAndStoreOrdered(get_GroupsInSequence());
  }

  printf("SAS:%d SBG:%d PPS:%d SPG:%d GPS:%d\n",
         (int) m_NSkippedAtStart, (int) m_NSkippedBetweenGroups,
         (int) m_NPhasesPerSummation, (int) m_NSummationsPerGroup,
         (int) m_NGroupsPerSequence);
}

void QxrdAcquisition::acquiredFrameAvailable(QxrdInt16ImageDataPtr image)
    // A new frame of data has been acquired, it should be added to m_AcquiredInt32Data.
    // If a summed exposure has been completed, either the 16 or 32 bit data
    // frame should be passed to the data processor, and replaced with a newly
    // allocated image.
    // If m_AcquiredInt16Data is NULL, then the program has run out of free memory
    // and acquisition should drop frames until memory is available
{
  THREAD_CHECK;

//  acquireTiming();

  if (m_NPhasesPerSummation < 1) m_NPhasesPerSummation.fetchAndStoreOrdered(1);
  if (m_NSummationsPerGroup < 1) m_NSummationsPerGroup.fetchAndStoreOrdered(1);
  if (m_NGroupsPerSequence  < 1) m_NGroupsPerSequence.fetchAndStoreOrdered(1);

  m_FrameCounter.fetchAndAddOrdered(1);

  if (image) {
    m_UpdateInterval.fetchAndStoreOrdered(1.0/get_ExposureTime());

    if (m_UpdateInterval < 1) {
      m_UpdateInterval.fetchAndStoreOrdered(1);
    }

    if ((m_FrameCounter % m_UpdateInterval) == 0) {
      m_DataProcessor -> idleInt16Image(image);
    }
  }

  int expAfterStart  = m_CurrentExposure - m_NSkippedAtStart;
  int expPerGroup    = m_NPhasesPerSummation*m_NSummationsPerGroup+m_NSkippedBetweenGroups;
  int grpInSequence  = expAfterStart / expPerGroup;
  int expWithinGroup = expAfterStart % expPerGroup;
  int expPerSequence = m_NSkippedAtStart + m_NGroupsPerSequence*expPerGroup - m_NSkippedBetweenGroups;

  int expPerSummation = m_NPhasesPerSummation;
  int sumInGroup     = expWithinGroup / expPerSummation;
  int expWithinSummation = expWithinGroup % expPerSummation;
  int isSummed       = true;

  if (m_Acquiring.tryLock()) {
    m_Acquiring.unlock();
  } else if (image == NULL) {
    indicateDroppedFrame();
  } else  {
//    m_CurrentExposure.fetchAndAddOrdered(1);

    if (m_CurrentExposure < m_NSkippedAtStart) {
      QCEP_DEBUG(DEBUG_ACQUIRE,
                 emit printMessage(QDateTime::currentDateTime(),
                                   tr("Frame %1 skipped").arg(m_CurrentExposure));
      );

      printf("mCE %d: skipped at start\n", m_CurrentExposure);
    } else {

      if (expWithinGroup < m_NPhasesPerSummation*m_NSummationsPerGroup) {
        isSummed = true;
        printf("Summed:  ");
      } else {
        isSummed = false;
        printf("Skipped: ");
      }

      m_CurrentPhase.fetchAndStoreOrdered(expWithinSummation);
      m_CurrentSummation.fetchAndStoreOrdered(sumInGroup);
      m_CurrentGroup.fetchAndStoreOrdered(grpInSequence);

      printf("mCE %d: mCP %d: mCS %d: mCG %d" /*": eas %d: epg %d: gis %d: ewg %d: eps %d: sig %d: ews %d"*/ "\n",
             (int) m_CurrentExposure, (int) m_CurrentPhase, (int) m_CurrentSummation, (int) m_CurrentGroup/*,
             expAfterStart, expPerGroup, grpInSequence, expWithinGroup, expPerSummation, sumInGroup, expWithinSummation*/);

      printf("Buff:");
      for (int i=0; i<m_NPhasesPerSummation; i++) {
        if (m_AcquiredInt32Data[i] == NULL) {
          printf(" %d:NULL", i);
        } else {
          printf(" %d:%-4d", i, m_AcquiredInt32Data[i]->get_SummedExposures());
        }
      }
      printf("\n");

      if (isSummed) {
        if (m_AcquiredInt32Data[m_CurrentPhase] == NULL) {
          m_AcquiredInt32Data[m_CurrentPhase] = m_Allocator->newInt32Image();
        } else if (m_CurrentSummation == 0) {
          processAcquiredImage(m_AcquiredInt32Data[m_CurrentPhase], m_OverflowMask);
          m_AcquiredInt32Data[m_CurrentPhase] = m_Allocator->newInt32Image();
        }

        accumulateAcquiredImage(image);
      }

//      m_CurrentGroup.fetchAndStoreOrdered(grpInSequence);

//      if (m_CurrentExposure <= get_ExposuresToSum()) {
//        if (m_CurrentExposure == 1) {
//          m_OverflowMask = m_Allocator -> newMask(0);
//        }

//        long nPixels = get_NRows()*get_NCols();
//        int ovflwlvl = get_OverflowLevel();
//        quint16* src = image->data();
//        quint32* dst = m_AcquiredInt32Data[0]->data();
//        short int* ovf = m_OverflowMask->data();

//        if (m_CurrentExposure == 1) {
//          QCEP_DEBUG(DEBUG_ACQUIRE,
//                     emit printMessage(QDateTime::currentDateTime(),
//                                       tr("Frame %1 saved").arg(m_CurrentExposure));
//          );
//          for (long i=0; i<nPixels; i++) {
//            quint16 val = *src++;

//            if (val>ovflwlvl) {
//              *ovf++ = 1;
//            } else {
//              *ovf++ = 0;
//            }

//            *dst++ = val;
//          }
//        } else {
//          QCEP_DEBUG(DEBUG_ACQUIRE,
//                     emit printMessage(QDateTime::currentDateTime(),
//                                       tr("Frame %1 summed").arg(m_CurrentExposure));
//          );
//          for (long i=0; i<nPixels; i++) {
//            quint16 val = *src++;

//            if (val>ovflwlvl) {
//              *ovf++ |= 1;
//            } else {
//              ovf++;
//            }

//            *dst++ += val;
//          }
//        }

//        m_AcquiredInt32Data[0] -> prop_SummedExposures() -> incValue(1);
//      }

//      QString fileName;
//      QString fileBase;

//      if (m_CurrentExposure == get_ExposuresToSum()) {
//        set_ExposuresToSkip(get_SkippedExposures());
//        m_CurrentExposure.fetchAndStoreOrdered(0);


//        if (get_AcquireDark()) {
//          fileBase = get_FilePattern()+tr("-%1.dark.tif").arg(get_FileIndex(),5,10,QChar('0'));
//          fileName = QDir(m_DataProcessor -> darkOutputDirectory()).filePath(fileBase);
//        } else {
//          fileBase = get_FilePattern()+tr("-%1.tif").arg(get_FileIndex(),5,10,QChar('0'));
//          fileName = QDir(m_DataProcessor -> rawOutputDirectory()).filePath(fileBase);
//        }

//        QCEP_DEBUG(DEBUG_ACQUIRE,
//          emit printMessage(QDateTime::currentDateTime(),
//                            tr("Fn: %1, Fi: %2, Exp: %3, Nexp %4, Fil: %5, NFil: %6, Grp: %7, NGrp: %8")
//                            .arg(fileName).arg(get_FileIndex())
//                            .arg(m_CurrentExposure).arg(get_ExposuresToSum())
//                            .arg(m_CurrentFile).arg(get_FilesInAcquiredGroup())
//                            .arg(m_CurrentGroup).arg(get_GroupsInAcquiredSequence()));
//        );

//        set_FileBase(fileBase);
////        m_DataProcessor -> set_FileName(fileName);

//        QFileInfo finfo(fileName);

//        m_AcquiredInt32Data[0] -> set_FileBase(fileBase);
//        m_AcquiredInt32Data[0] -> set_FileName(fileName);
//        m_AcquiredInt32Data[0] -> set_Title(finfo.fileName());
//        m_AcquiredInt32Data[0] -> set_ExposureTime(get_ExposureTime());
//        //          m_AcquiredInt32Data[0] -> set_SummedExposures(get_ExposuresToSum());
//        m_AcquiredInt32Data[0] -> set_DateTime(QDateTime::currentDateTime());
//        m_AcquiredInt32Data[0] -> set_HBinning(1);
//        m_AcquiredInt32Data[0] -> set_VBinning(1);
//        m_AcquiredInt32Data[0] -> set_CameraGain(get_CameraGain());
//        m_AcquiredInt32Data[0] -> set_DataType(QxrdInt32ImageData::Raw32Data);
//        m_AcquiredInt32Data[0] -> set_UserComment1(get_UserComment1());
//        m_AcquiredInt32Data[0] -> set_UserComment2(get_UserComment2());
//        m_AcquiredInt32Data[0] -> set_UserComment3(get_UserComment3());
//        m_AcquiredInt32Data[0] -> set_UserComment4(get_UserComment4());
//        m_AcquiredInt32Data[0] -> set_ImageSaved(false);

//        copyDynamicProperties(m_AcquiredInt32Data[0].data());

//        if (get_AcquireDark()) {
//          m_AcquiredInt32Data[0] -> set_ImageNumber(-1);
//          QCEP_DEBUG(DEBUG_ACQUIRE,
//                     emit printMessage(QDateTime::currentDateTime(),
//                                       tr("32 bit Dark Image acquired"));
//              );
//          m_DataProcessor -> acquiredInt32Image(m_AcquiredInt32Data[0], m_OverflowMask);
//          m_AcquiredInt32Data[0] = m_Allocator -> newInt32Image();
//          haltAcquisition();
//        } else {
//          QCEP_DEBUG(DEBUG_ACQUIRE,
//                     emit printMessage(QDateTime::currentDateTime(),
//                                       tr("32 bit Image %1 acquired").arg(m_CurrentFile));
//              );
//          m_AcquiredInt32Data[0] -> set_ImageNumber(m_CurrentFile.fetchAndAddOrdered(1));
//          m_DataProcessor -> acquiredInt32Image(m_AcquiredInt32Data[0], m_OverflowMask);
//          m_AcquiredInt32Data[0] = m_Allocator -> newInt32Image();
//          set_FileIndex(get_FileIndex()+1);
//        }

//        emit statusMessage(QDateTime::currentDateTime(),
//                           tr("Acquiring ""%1"" (%2 i16, %3 i32, %4 dbl)").arg(fileName)
//                           .arg(m_Allocator->nFreeInt16())
//                           .arg(m_Allocator->nFreeInt32())
//                           .arg(m_Allocator->nFreeDouble())
//                           );

//        if (m_CurrentFile >= (get_FilesInAcquiredGroup())) {
//          emit printMessage(QDateTime::currentDateTime(), "Acquisition ended");
//          emit printMessage(QDateTime::currentDateTime(), "Halted acquisition");

//          haltAcquisition();
//        }
//      }

      emit acquiredFrame("<<>>", get_FileIndex(),
                         m_CurrentPhase,     m_NPhasesPerSummation,
                         m_CurrentSummation, m_NSummationsPerGroup,
                         m_CurrentGroup,     m_NGroupsPerSequence);
    }

    m_CurrentExposure.fetchAndAddOrdered(1);

    if (m_CurrentExposure >= expPerSequence || get_Cancelling()) {
      for (int i=0; i<m_NPhasesPerSummation; i++) {
        if (m_AcquiredInt32Data[i]) {
          processAcquiredImage(m_AcquiredInt32Data[i], m_OverflowMask);
          m_AcquiredInt32Data[i] = QxrdInt32ImageDataPtr();
        }
      }
      printf("Finished\n");
      haltAcquisition();
      return;
    }
  }
}

template <typename T>
void QxrdAcquisition::accumulateAcquiredImage(QSharedPointer< QxrdImageData<T> > image)
{
  if (image && m_AcquiredInt32Data[m_CurrentPhase]) {
    m_AcquiredInt32Data[m_CurrentPhase]->accumulateImage(image);
  }
}

void QxrdAcquisition::processAcquiredImage(QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow)
{
  if (image) {
    printf("processAcquiredImage summed:%d\n", image->get_SummedExposures());
  }
//  m_DataProcessor->acquiredInt32Image(image, overflow);
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

QxrdAcquireDialog *QxrdAcquisition::controlPanel(QxrdWindowPtr win)
{
  if (win) {
    m_Window = win;

    m_ControlPanel = new QxrdAcquireDialog(m_Window, this, m_DataProcessor, m_Window);

    return m_ControlPanel;
  } else {
    return NULL;
  }
}

void QxrdAcquisition::acquisitionReady()
{
  if (m_ControlPanel) {
    m_ControlPanel -> acquisitionReady();
  }
}

void QxrdAcquisition::acquisitionStarted()
{
  if (m_ControlPanel) {
    m_ControlPanel -> acquisitionStarted();
  }
}

void QxrdAcquisition::acquisitionFinished()
{
  if (m_ControlPanel) {
    m_ControlPanel -> acquisitionFinished();
  }
}

void QxrdAcquisition::darkAcquisitionStarted()
{
  if (m_ControlPanel) {
    m_ControlPanel -> darkAcquisitionStarted();
  }
}

void QxrdAcquisition::setNIDAQPlugin(QxrdNIDAQPluginInterface *nidaqPlugin)
{
  m_NIDAQPlugin = nidaqPlugin;

  if (m_SynchronizedAcquisition) {
    m_SynchronizedAcquisition -> setNIDAQPlugin(nidaqPlugin);
  }
}

QxrdNIDAQPluginInterface *QxrdAcquisition::nidaqPlugin() const
{
  return m_NIDAQPlugin;
}

QxrdSynchronizedAcquisition* QxrdAcquisition::synchronizedAcquisition() const
{
  return m_SynchronizedAcquisition;
}

void QxrdAcquisition::acquireTiming()
{
  int msec = m_ElapsedTimer.restart();

  int expmsec = 1000.0*get_ExposureTime();

  int del = expmsec-msec;

  if (del >= -10 && del <= 10) {
    m_ElapsedHistogram[del+10] += 1;
  }

  m_ElapsedCounter += 1;

  if (((double)m_ElapsedCounter)*get_ExposureTime() > 60.0) {
    QString msg = "Elapsed Histogram";

    for (int i=-10; i<10; i++) {
      msg += QString(" %1").arg(m_ElapsedHistogram[i+10]);
    }

    emit printMessage(QDateTime::currentDateTime(), msg);

    m_ElapsedCounter = 0;
    m_ElapsedHistogram.fill(0);
  }
}
