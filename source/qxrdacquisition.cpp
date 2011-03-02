#include "qxrdacquisition.h"
#include "qxrdmutexlocker.h"
#include "qxrddataprocessor.h"
#include "qxrdallocator.h"
#include "qxrdacquiredialog.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdwindow.h"

#include <QThreadPool>
#include <QtConcurrentRun>

QxrdAcquisition::QxrdAcquisition(QxrdDataProcessor *proc, QxrdAllocator *allocator)
  : QxrdAcquisitionOperations(proc, allocator),
    m_AcquiredInt32Data(),
    m_OverflowMask(),
    m_NSkippedAtStart(0),
    m_NSkippedBetweenGroups(0),
    m_NPhasesPerSummation(1),
    m_NSummationsPerGroup(1),
    m_NGroupsPerSequence(1),
    m_FrameCounter(0),
    m_InitialFileIndex(0),
    m_ControlPanel(NULL)
{
  m_SynchronizedAcquisition = new QxrdSynchronizedAcquisition(this);

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

  connect(&m_Watcher, SIGNAL(finished()), this, SLOT(onAcquireComplete()));
}

QxrdAcquisition::~QxrdAcquisition()
{
//  QCEP_DEBUG(DEBUG_ACQUIRE,
//             printf("QxrdAcquisition::~QxrdAcquisition\n");
//  );
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
  m_OverflowMask.resize(m_NPhasesPerSummation);
}

void QxrdAcquisition::acquire()
{
  THREAD_CHECK;

//  printf("QxrdAcquisitionPerkinElmer::acquire (thread()=%p, currentThread()=%p)\n",
//         thread(), QThread::currentThread());

  if (m_Acquiring.tryLock()) {
    set_Cancelling(false);

    //  emit printMessage(tr("QxrdAcquisitionPerkinElmer::acquire()"));
    emit statusMessage(QDateTime::currentDateTime(), "Starting acquisition");
    emit acquireStarted(0);

//    acquisition(0);

    typedef void (QxrdAcquisition::*MFType)(QString, double, int, int, int);
    MFType p = &QxrdAcquisition::doAcquire;
    QFuture<void> res = QtConcurrent::run(this, p, tr("junk"), 0.2, 10, 3, 3);

    m_Watcher.setFuture(res);
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
    set_Cancelling(false);

    //  emit printMessage(tr("QxrdAcquisitionPerkinElmer::acquireDark()"));
    emit statusMessage(QDateTime::currentDateTime(), "Starting dark acquisition");
    emit acquireStarted(1);

//    acquisition(1);

    typedef void (QxrdAcquisition::*MFType)(QString, double, int, int);
    MFType p = &QxrdAcquisition::doAcquireDark;
    QFuture<void> res = QtConcurrent::run(this, p, tr("junk"), 0.2, 10, 1);

    m_Watcher.setFuture(res);
  } else {
    emit statusMessage(QDateTime::currentDateTime(), "Acquisition is already in progress");
  }
}

void QxrdAcquisition::onAcquireComplete()
{
  m_Acquiring.unlock();

  emit acquireComplete();
}

void QxrdAcquisition::cancel()
{
  set_Cancelling(true);
//  INVOKE_CHECK(QMetaObject::invokeMethod(this, "haltAcquisition", Qt::QueuedConnection));
}

void QxrdAcquisition::cancelDark()
{
  set_Cancelling(true);
//  INVOKE_CHECK(QMetaObject::invokeMethod(this, "haltAcquisition", Qt::QueuedConnection));
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

  if (m_SynchronizedAcquisition) {
    m_SynchronizedAcquisition->prepareForAcquisition();
  }

  beginAcquisition();
}

void QxrdAcquisition::beginAcquisition()
{
  m_ElapsedTimer.start();
}

void QxrdAcquisition::indicateDroppedFrame(int n)
{
  QString msg = tr("Frame %1 dropped [%2/%3 MB Used]")
                  .arg(n)
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

//  m_CurrentExposure.fetchAndStoreOrdered(0);
//  m_CurrentPhase.fetchAndStoreOrdered(0);
//  m_CurrentSummation.fetchAndStoreOrdered(0);
//  m_CurrentGroup.fetchAndStoreOrdered(0);

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

  m_InitialFileIndex = get_FileIndex();

  QCEP_DEBUG(DEBUG_ACQUIRE,
             emit printMessage(QDateTime::currentDateTime(),
                               tr("SAS:%1 SBG:%2 PPS:%3 SPG:%4 GPS:%5\n")
                               .arg(m_NSkippedAtStart).arg(m_NSkippedBetweenGroups)
                               .arg(m_NPhasesPerSummation).arg(m_NSummationsPerGroup)
                               .arg(m_NGroupsPerSequence));
      );
}

static QTime tick;

int QxrdAcquisition::currentPhase(int frameNumber)
{
  if (frameNumber < m_NSkippedAtStart) {
    return -1;
  } else {
    int expAfterStart  = frameNumber - m_NSkippedAtStart;
    int expPerGroup    = m_NPhasesPerSummation*m_NSummationsPerGroup+m_NSkippedBetweenGroups;
//    int currentGroup   = expAfterStart / expPerGroup;
    int expWithinGroup = expAfterStart % expPerGroup;
//    int expPerSequence = m_NSkippedAtStart + m_NGroupsPerSequence*expPerGroup - m_NSkippedBetweenGroups;

    int expPerSummation = m_NPhasesPerSummation;
//    int currentSummation = expWithinGroup / expPerSummation;
    int currentPhase   = expWithinGroup % expPerSummation;
    return currentPhase;
  }
}

void QxrdAcquisition::acquiredFrameAvailable(QxrdInt16ImageDataPtr image, int counter)
    // A new frame of data has been acquired, it should be added to m_AcquiredInt32Data.
    // If a summed exposure has been completed, either the 16 or 32 bit data
    // frame should be passed to the data processor, and replaced with a newly
    // allocated image.
    // If m_AcquiredInt16Data is NULL, then the program has run out of free memory
    // and acquisition should drop frames until memory is available
{
  return;

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

  int expAfterStart  = counter - m_NSkippedAtStart;
  int expPerGroup    = m_NPhasesPerSummation*m_NSummationsPerGroup+m_NSkippedBetweenGroups;
  int currentGroup   = expAfterStart / expPerGroup;
  int expWithinGroup = expAfterStart % expPerGroup;
  int expPerSequence = m_NSkippedAtStart + m_NGroupsPerSequence*expPerGroup - m_NSkippedBetweenGroups;

  int expPerSummation = m_NPhasesPerSummation;
  int currentSummation = expWithinGroup / expPerSummation;
  int currentPhase   = expWithinGroup % expPerSummation;
  int isSummed       = true;

  if (m_Acquiring.tryLock()) {
    m_Acquiring.unlock();
  } else if (image == NULL) {
    indicateDroppedFrame(counter);
  } else  {
//    printf("acquiredFrameAvailable[%d] %d msec\n", counter, tick.restart());

    QString msg;
    msg = tr("Acq fctr %1: ").arg(m_FrameCounter);

    if (counter < m_NSkippedAtStart) {
      QCEP_DEBUG(DEBUG_ACQUIRE,
                 emit printMessage(QDateTime::currentDateTime(),
                                   tr("Frame %1 skipped").arg(counter));
      );
//      m_CurrentPhase.fetchAndStoreOrdered(-1);
//      m_CurrentSummation.fetchAndStoreOrdered(-1);
//      m_CurrentGroup.fetchAndStoreOrdered(-1);
    } else {

      if (expWithinGroup < m_NPhasesPerSummation*m_NSummationsPerGroup) {
        isSummed = true;
        msg += "Summed:  ";
      } else {
        isSummed = false;
        msg += "Skipped: ";
      }

//      m_CurrentPhase.fetchAndStoreOrdered(currentPhase);
//      m_CurrentSummation.fetchAndStoreOrdered(currentSummation);
//      m_CurrentGroup.fetchAndStoreOrdered(currentGroup);

      set_FileIndex(m_InitialFileIndex+currentGroup);

      QCEP_DEBUG(DEBUG_ACQUIRE,
                 emit printMessage(QDateTime::currentDateTime(),
                                   msg+tr("fIdx %1: mCE %2: mCP %3: mCS %4: mCG %5")
                                   .arg(get_FileIndex()).arg(counter)
                                   .arg(currentPhase).arg(currentSummation).arg(currentGroup));
          );

      if (isSummed) {
        if (m_AcquiredInt32Data[currentPhase] == NULL) {
          m_AcquiredInt32Data[currentPhase] = m_Allocator->newInt32Image();
          m_OverflowMask[currentPhase] = m_Allocator->newMask(0);
        } else if (currentSummation == 0) {
          QCEP_DEBUG(DEBUG_ACQUIRE,
                     emit printMessage(QDateTime::currentDateTime(),
                                       msg+tr("process[%1]").arg(currentPhase));
          );
          processAcquiredImage(m_InitialFileIndex+currentGroup-1, currentPhase, m_AcquiredInt32Data[currentPhase], m_OverflowMask[currentPhase]);
          m_AcquiredInt32Data[currentPhase] = m_Allocator->newInt32Image();
          m_OverflowMask[currentPhase] = m_Allocator->newMask(0);
        }

        QCEP_DEBUG(DEBUG_ACQUIRE,
                   emit printMessage(QDateTime::currentDateTime(),
                                     msg+tr("accumulate[%1]").arg(currentPhase));
        );

        accumulateAcquiredImage(image, m_AcquiredInt32Data[currentPhase], m_OverflowMask[currentPhase]);
      }

      QString fileBase, fileName;

      getFileBaseAndName(m_InitialFileIndex+currentGroup, currentPhase, fileBase, fileName);

      emit acquiredFrame(fileBase, get_FileIndex(),
                         currentPhase,     m_NPhasesPerSummation,
                         currentSummation, m_NSummationsPerGroup,
                         currentGroup,     m_NGroupsPerSequence);
    }

//    m_CurrentExposure.fetchAndAddOrdered(1);

    if (counter >= expPerSequence || get_Cancelling()) {
      for (int i=0; i<m_NPhasesPerSummation; i++) {
        if (m_AcquiredInt32Data[i]) {
          processAcquiredImage(m_InitialFileIndex+currentGroup, i, m_AcquiredInt32Data[i], m_OverflowMask[i]);
          m_AcquiredInt32Data[i] = QxrdInt32ImageDataPtr();
          m_OverflowMask[i] = QxrdMaskDataPtr();
        }
      }

      QCEP_DEBUG(DEBUG_ACQUIRE,
                 emit printMessage(QDateTime::currentDateTime(),
                                   "Acquisition Finished\n");
          );

      if (!get_AcquireDark()) {
        prop_FileIndex()->incValue(1);
      }

      haltAcquisition();
      return;
    }
  }
}

template <typename T>
void QxrdAcquisition::accumulateAcquiredImage(QSharedPointer< QxrdImageData<T> > image, QxrdInt32ImageDataPtr accum, QxrdMaskDataPtr overflow)
{
  if (image && accum && overflow) {
    long nPixels = get_NRows()*get_NCols();
    int ovflwlvl = get_OverflowLevel();
    T* src = image->data();
    quint32* dst = accum->data();
    short int* ovf = overflow->data();
    int nsummed = accum->get_SummedExposures();

    if (nsummed == 0) {
      QCEP_DEBUG(DEBUG_ACQUIRE,
                 emit printMessage(QDateTime::currentDateTime(),
                                   tr("Frame %1 saved").arg(nsummed));
          );
      for (long i=0; i<nPixels; i++) {
        T val = *src++;

        if (val>ovflwlvl) {
          *ovf++ = 1;
        } else {
          *ovf++ = 0;
        }

        *dst++ = val;
      }
    } else {
      QCEP_DEBUG(DEBUG_ACQUIRE,
                 emit printMessage(QDateTime::currentDateTime(),
                                   tr("Frame %1 summed").arg(nsummed));
          );
      for (long i=0; i<nPixels; i++) {
        T val = *src++;

        if (val>ovflwlvl) {
          *ovf++ |= 1;
        } else {
          ovf++;
        }

        *dst++ += val;
      }
    }

    accum -> prop_SummedExposures() -> incValue(1);
  }
}

void QxrdAcquisition::getFileBaseAndName(int fileIndex, int phase, QString &fileBase, QString &fileName)
{
  if (get_AcquireDark()) {
    fileBase = get_FilePattern()+tr("-%1.dark.tif").arg(fileIndex,5,10,QChar('0'));
    fileName = QDir(m_DataProcessor -> darkOutputDirectory()).filePath(fileBase);
  } else {
    if (m_NPhasesPerSummation > 1) {
      fileBase = get_FilePattern()+tr("-%1-%2.tif").arg(fileIndex,5,10,QChar('0')).arg(phase,3,10,QChar('0'));
    } else {
      fileBase = get_FilePattern()+tr("-%1.tif").arg(fileIndex,5,10,QChar('0'));
    }
    fileName = QDir(m_DataProcessor -> rawOutputDirectory()).filePath(fileBase);
  }
}

void QxrdAcquisition::processAcquiredImage(int fileIndex, int phase, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow)
{
  if (image) {
    QCEP_DEBUG(DEBUG_ACQUIRE,
               emit printMessage(QDateTime::currentDateTime(),
                                 tr("processAcquiredImage(%1,%2) %3 summed exposures")
                                 .arg(fileIndex).arg(phase).arg(image->get_SummedExposures()));
        );

    QString fileName;
    QString fileBase;

    getFileBaseAndName(fileIndex, phase, fileBase, fileName);

    QCEP_DEBUG(DEBUG_ACQUIRE,
               emit printMessage(QDateTime::currentDateTime(),
                                 tr("Fn: %1, Fi: %2, Phs: %3")
                                 .arg(fileName).arg(get_FileIndex()).arg(phase));
        );

    set_FileBase(fileBase);
    //        m_DataProcessor -> set_FileName(fileName);

    QFileInfo finfo(fileName);

    image -> set_FileBase(fileBase);
    image -> set_FileName(fileName);
    image -> set_Title(finfo.fileName());
    image -> set_ExposureTime(get_ExposureTime());
    image -> set_DateTime(QDateTime::currentDateTime());
    image -> set_HBinning(1);
    image -> set_VBinning(1);
    image -> set_CameraGain(get_CameraGain());
    image -> set_DataType(QxrdInt32ImageData::Raw32Data);
    image -> set_UserComment1(get_UserComment1());
    image -> set_UserComment2(get_UserComment2());
    image -> set_UserComment3(get_UserComment3());
    image -> set_UserComment4(get_UserComment4());
    image -> set_ImageSaved(false);

    copyDynamicProperties(image.data());

    if (get_AcquireDark()) {
      QCEP_DEBUG(DEBUG_ACQUIRE,
                 emit printMessage(QDateTime::currentDateTime(),
                                   tr("32 bit Dark Image acquired"));
          );

      image -> set_ImageNumber(-1);
      image -> set_PhaseNumber(-1);
    } else {
      QCEP_DEBUG(DEBUG_ACQUIRE,
                 emit printMessage(QDateTime::currentDateTime(),
                                   tr("32 bit Image %1 acquired").arg(get_FileIndex()));
          );
      image -> set_ImageNumber(fileIndex);
      image -> set_PhaseNumber(phase);
    }

    m_DataProcessor -> acquiredInt32Image(image, overflow);
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

QxrdAcquireDialog *QxrdAcquisition::controlPanel(QxrdWindow *win)
{
  if (win) {
    m_Window = win;

    m_ControlPanel = new QxrdAcquireDialog(m_Window, this, m_DataProcessor, m_Window);

    return m_ControlPanel;
  } else {
    return NULL;
  }
}

void QxrdAcquisition::setNIDAQPlugin(QxrdNIDAQPluginInterface *nidaqPlugin)
{
  if (m_SynchronizedAcquisition) {
    m_SynchronizedAcquisition -> setNIDAQPlugin(nidaqPlugin);
  }
}

QxrdNIDAQPluginInterface *QxrdAcquisition::nidaqPlugin() const
{
  if (m_SynchronizedAcquisition) {
    return m_SynchronizedAcquisition -> nidaqPlugin();
  } else {
    return NULL;
  }
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

void QxrdAcquisition::doAcquire    (QString fileName, double exposure, int nsummed, int nfiles, int nphases/*, int skipBefore, int skipBetween*/)
{
  stopIdling();

  int skipBefore = 0;
  int skipBetween = 0;

  QVector<QxrdInt32ImageDataPtr> res(nphases);
  QVector<QxrdMaskDataPtr>       ovf(nphases);

  for (int i=0; i<skipBefore; i++) {
    if (get_Cancelling()) goto cancel;
    acquireFrame(exposure);
  }


  for (int i=0; i<nfiles; i++) {
    if (get_Cancelling()) goto cancel;

    if (i != 0) {
      for (int k=0; k<skipBetween; k++) {
        acquireFrame(exposure);
      }
    }

    for (int p=0; p<nphases; p++) {
      res[p] = m_Allocator->newInt32Image();
      ovf[p] = m_Allocator->newMask(0);
    }

    if (get_Cancelling()) goto cancel;

    for (int s=0; s<nsummed; s++) {
      for (int p=0; p<nphases; p++) {
        QxrdInt16ImageDataPtr img = acquireFrame(exposure);

        accumulateAcquiredImage(img, res[p], ovf[p]);

        if (get_Cancelling()) goto saveCancel;
      }
    }

  saveCancel:
    for (int p=0; p<nphases; p++) {
      processAcquiredImage(fileName, i, p, res[p], ovf[p]);
    }
  }

cancel:
  startIdling();
}

void QxrdAcquisition::doAcquireDark(QString fileName, double exposure, int nsummed, int skipBefore)
{
  stopIdling();

  QxrdInt32ImageDataPtr res = m_Allocator->newInt32Image();
  QxrdMaskDataPtr overflow  = m_Allocator->newMask(0);

  for (int i=0; i<skipBefore; i++) {
    if (get_Cancelling()) goto cancel;
    acquireFrame(exposure);
  }

  for (int i=0; i<nsummed; i++) {
    if (get_Cancelling()) goto saveCancel;

    QxrdInt16ImageDataPtr img = acquireFrame(exposure);

    accumulateAcquiredImage(img, res, overflow);
  }

saveCancel:
  processDarkImage(fileName, 0, res, overflow);

cancel:
  startIdling();
}

void QxrdAcquisition::stopIdling()
{
  printf("stop idling\n");
}

void QxrdAcquisition::startIdling()
{
  printf("start idling\n");
}

QxrdInt16ImageDataPtr QxrdAcquisition::acquireFrame(double exposure)
{
  QxrdInt16ImageDataPtr res = m_Allocator->newInt16Image();

  res->fill((int) (1000.0*exposure));

  return res;
}

void QxrdAcquisition::processAcquiredImage(QString fileName, int fileIndex, int phase, QxrdInt32ImageDataPtr img, QxrdMaskDataPtr ovf)
{
  printf("processAcquiredImage(""%s"",%d,%d,img,ovf)\n", qPrintable(fileName), fileIndex, phase);
}

void QxrdAcquisition::processDarkImage(QString fileName, int fileIndex, QxrdInt32ImageDataPtr img, QxrdMaskDataPtr ovf)
{
  printf("processDarkImage(""%s"",%d,img,ovf)\n", qPrintable(fileName), fileIndex);
}
