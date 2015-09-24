#include "qxrddebug.h"
#include "qxrdacquisition.h"
#include "qcepmutexlocker.h"
#include "qxrddataprocessor.h"
#include "qcepallocator.h"
#include "qxrdacquisitiondialog.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdacquisitionextrainputs.h"
#include "qxrdwindow.h"
#include "qxrdapplication.h"
#include "qxrdacquisition-ptr.h"
//#include <QThreadPool>
#include <QtConcurrentRun>
#include <QDir>
#include <QMetaProperty>
#include "qxrdprocessargs.h"
#include "qxrddetectorproxy.h"
#include "qxrddetector.h"
#include "qxrddetectorthread.h"
#include "qxrddetectorprocessor.h"
#include "qxrdacquisitionparameterpack.h"
#include "qxrddarkacquisitionparameterpack.h"

QxrdAcquisition::QxrdAcquisition(QcepSettingsSaverWPtr saver,
                                 QxrdExperimentWPtr doc,
                                 QxrdDataProcessorWPtr proc,
                                 QcepAllocatorWPtr allocator)
  : QxrdAcquisitionInterface(saver, doc, proc, allocator),
    m_QxrdVersion(QcepSettingsSaverWPtr(), this,"qxrdVersion",STR(QXRD_VERSION), "QXRD Version Number"),
    m_QtVersion(QcepSettingsSaverWPtr(), this,"qtVersion",qVersion(), "QT Version Number"),
    m_DetectorCount(m_Saver, this, "detectorCount", 0, "Number of Detectors"),
    m_LastAcquired(QcepSettingsSaverWPtr(), this, "lastAcquired", 0, "Internal Acquisition Flag"),
    m_FileIndex(saver, this,"fileIndex",0, "File Index"),
    m_FileIndexWidth(saver, this, "fileIndexWidth", 5, "Digits in File Index Field"),
    m_FilePhaseWidth(saver, this, "filePhaseWidth", 3, "Digits in Phase Number Field"),
    m_FileOverflowWidth(saver, this, "fileOverflowWidth", 5, "Digits in Overflow Index Field"),
    m_DetectorNumberWidth(saver, this, "detectorNumberWidth", 2, "Digits in detector number field"),
    m_FileBase(saver, this,"fileBase","", "File Base"),
//    m_NRows(saver, this, "nRows", 2048, "Number of Rows"),
//    m_NCols(saver, this, "nCols", 2048, "Number of Cols"),
    m_OverflowLevel(saver, this, "overflowLevel", 65500, "Overflow level (per exposure)"),
    m_Raw16SaveTime(saver, this,"raw16SaveTime", 0.1, "Time to save 16 bit images"),
    m_Raw32SaveTime(saver, this,"raw32SaveTime", 0.2, "Time to save 32 bit images"),
    m_RawSaveTime(saver, this,"rawSaveTime", 0.2, "Time to save raw images"),
    m_DarkSaveTime(saver, this,"darkSaveTime", 0.2, "Time to save dark images"),
    m_UserComment1(saver, this,"userComment1","", "User Comment 1"),
    m_UserComment2(saver, this,"userComment2","", "User Comment 2"),
    m_UserComment3(saver, this,"userComment3","", "User Comment 3"),
    m_UserComment4(saver, this,"userComment4","", "User Comment 4"),
    m_Normalization(saver, this, "normalization", QcepDoubleList(), "Normalization Values"),
    m_DroppedFrames(QcepSettingsSaverWPtr(), this,"droppedFrames",0, "Number of Dropped Frames"),
    m_LiveViewAtIdle(saver, this, "liveViewAtIdle", false, "Live View during Idle"),
    m_AcquisitionCancelsLiveView(saver, this, "acquisitionCancelsLiveView", true, "Acquisition operations cancel live view"),
    m_RetryDropped(saver, this, "retryDropped", false, "Automatically retry dropped frames during acquisition"),
    m_Mutex(QMutex::Recursive),
    m_SynchronizedAcquisition(NULL),
    m_AcquisitionExtraInputs(NULL),
    m_Experiment(doc),
    m_Window(),
    m_Allocator(allocator),
    m_DataProcessor(proc),
    m_ControlPanel(NULL),
    m_Idling(1)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisition::QxrdAcquisition(%p)\n", this);
  }
}

QxrdAcquisitionPtr QxrdAcquisition::myself()
{
  QxrdAcquisitionPtr me = qSharedPointerCast<QxrdAcquisition>(sharedFromThis());

//  if (me == NULL) {
//    printf("QxrdAcquisition::myself returns NULL\n");
//  }

  return me;
}

void QxrdAcquisition::initialize()
{
  QxrdAcquisitionInterface::initialize();

  connect(prop_Raw16SaveTime(), &QcepDoubleProperty::valueChanged, this, &QxrdAcquisition::updateSaveTimes);
  connect(prop_Raw32SaveTime(), &QcepDoubleProperty::valueChanged, this, &QxrdAcquisition::updateSaveTimes);
  connect(prop_SummedExposures(), &QcepIntProperty::valueChanged,  this, &QxrdAcquisition::updateSaveTimes);
  connect(prop_DarkSummedExposures(), &QcepIntProperty::valueChanged, this, &QxrdAcquisition::updateSaveTimes);

//  m_FileIndex.setDebug(1);

  QcepAllocatorPtr alloc(m_Allocator);

  if (qcepDebug(DEBUG_APP)) {
    printMessage("QxrdAcquisition::QxrdAcquisition");
  }

  m_SynchronizedAcquisition = QxrdSynchronizedAcquisitionPtr(
        new QxrdSynchronizedAcquisition(m_Saver, myself()));

  m_AcquisitionExtraInputs = QxrdAcquisitionExtraInputsPtr(
        new QxrdAcquisitionExtraInputs(m_Saver, m_Experiment, myself()));
  m_AcquisitionExtraInputs -> initialize(m_AcquisitionExtraInputs);

  connect(prop_ExposureTime(), &QcepDoubleProperty::valueChanged,
          this, &QxrdAcquisition::onExposureTimeChanged);

  if (alloc) {
    if (sizeof(void*) == 4) {
      connect(alloc->prop_TotalBufferSizeMB32(), &QcepIntProperty::valueChanged,
              this, &QxrdAcquisition::onBufferSizeChanged);
      onBufferSizeChanged(alloc->get_TotalBufferSizeMB32());
    } else {
      connect(alloc->prop_TotalBufferSizeMB64(), &QcepIntProperty::valueChanged,
              this, &QxrdAcquisition::onBufferSizeChanged);
      onBufferSizeChanged(alloc->get_TotalBufferSizeMB64());
    }
  }

//  connect(&m_Watcher, &QFutureWatcherBase::finished, this, &QxrdAcquisition::onAcquireComplete);
  connect(&m_IdleTimer, &QTimer::timeout, this, &QxrdAcquisition::onIdleTimeout);

  m_IdleTimer.start(1000);
}

QxrdAcquisition::~QxrdAcquisition()
{
#ifndef QT_NO_DEBUG
  printf("Deleting acquisition\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisition::~QxrdAcquisition(%p)\n", this);
  }

  if (qcepDebug(DEBUG_APP)) {
    printMessage("QxrdAcquisition::~QxrdAcquisition");
  }
}

QxrdExperimentWPtr QxrdAcquisition::experiment()
{
  return m_Experiment;
}

QxrdDataProcessorWPtr QxrdAcquisition::dataProcessor()
{
  return m_DataProcessor;
}

void QxrdAcquisition::shutdown()
{
  if (qcepDebug(DEBUG_APP)) {
    printMessage("QxrdAcquisition::shutdown()");
  }

  for (int i=0; i<get_DetectorCount(); i++) {
    detector(i) -> shutdownAcquisition();
  }

  thread()->exit();
}

void QxrdAcquisition::updateSaveTimes()
{
  if (get_SummedExposures() <= 1) {
    set_RawSaveTime(get_Raw16SaveTime());
  } else {
    set_RawSaveTime(get_Raw32SaveTime());
  }

  if (get_DarkSummedExposures() <= 1) {
    set_DarkSaveTime(get_Raw16SaveTime());
  } else {
    set_DarkSaveTime(get_Raw32SaveTime());
  }
}

void QxrdAcquisition::dynamicProperties()
{
  QByteArray name;

  if (g_Application) {
    foreach(name, dynamicPropertyNames()) {
      g_Application->printMessage(tr("acquisition.%1\n").arg(name.data()));
    }
  }
}

void QxrdAcquisition::copyDynamicProperties(QObject *dest)
{
  if (dest) {
    QByteArray name;

    foreach(name, dynamicPropertyNames()) {
      dest -> setProperty(name.data(), property(name.data()));
    }
  }
}

void QxrdAcquisition::writeSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::writeSettings(settings, section);

  if (m_SynchronizedAcquisition) {
    m_SynchronizedAcquisition->writeSettings(settings, section+"/sync");
  }

  if (m_AcquisitionExtraInputs) {
    m_AcquisitionExtraInputs->writeSettings(settings, section+"/extrainputs");
  }

  if (get_DetectorCount() > 0) {
    settings->beginWriteArray(section+"/detectors");

    for (int i=0; i<get_DetectorCount(); i++) {
      settings->setArrayIndex(i);

      m_Detectors[i]->writeSettings(settings, "");
    }

    settings->endArray();
  }
}

void QxrdAcquisition::readSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::readSettings(settings, section);

  if (m_SynchronizedAcquisition) {
    m_SynchronizedAcquisition->readSettings(settings, section+"/sync");
  }

  if (m_AcquisitionExtraInputs) {
    m_AcquisitionExtraInputs->readSettings(settings, section+"/extrainputs");
  }

  if (get_DetectorCount() > 0) {
    int n = settings->beginReadArray(section+"/detectors");

    m_DetectorThreads.resize(n);
    m_Detectors.resize(n);

    for (int i=0; i<n; i++) {
      settings->setArrayIndex(i);

      int detType = settings->value("detectorType", 0).toInt();

      QxrdDetectorThreadPtr detThread =
          QxrdDetectorThreadPtr(new QxrdDetectorThread(m_Saver,
                                                       experiment(),
                                                       myself(),
                                                       detType,
                                                       i,
                                                       this));

      if (detThread) {
        detThread->start();

        QxrdDetectorPtr det = detThread->detector();

        if (det) {
          det->initialize();
          det->readSettings(settings, "");

          m_DetectorThreads[i] = detThread;
          m_Detectors[i]       = det;

          det->startOrStop(det->isEnabled());
        }
      }
    }

    settings->endArray();
  }
}

void QxrdAcquisition::appendDetector(int detType)
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "appendDetector", Qt::BlockingQueuedConnection,
                              Q_ARG(int, detType));
  } else {
    int nDet = get_DetectorCount();

    QxrdDetectorThreadPtr detThread =
        QxrdDetectorThreadPtr(new QxrdDetectorThread(m_Saver,
                                                     experiment(),
                                                     myself(),
                                                     detType,
                                                     nDet,
                                                     this));

    if (detThread) {
      detThread->start();

      QxrdDetectorPtr det = detThread->detector();

      if (det) {
        m_DetectorThreads.append(detThread);
        m_Detectors.append(det);

        set_DetectorCount(m_Detectors.count());
      }
    }
  }
}

void QxrdAcquisition::appendDetectorProxy(QxrdDetectorProxyPtr proxy)
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "appendDetectorProxy", Qt::BlockingQueuedConnection,
                              Q_ARG(QxrdDetectorProxyPtr, proxy));
  } else {
    if (proxy) {
      QxrdDetectorThreadPtr detThread = proxy->detectorThread();
      QxrdDetectorPtr       detector  = proxy->detector();

      int nDet = get_DetectorCount();

      if (detThread==NULL || detector==NULL) {
        int detType = proxy->detectorType();

       detThread =
            QxrdDetectorThreadPtr(new QxrdDetectorThread(m_Saver,
                                                         experiment(),
                                                         myself(),
                                                         detType,
                                                         nDet,
                                                         this));
       detThread->start();

       detector = detThread->detector();
      }

      m_DetectorThreads.append(detThread);
      m_Detectors.append(detector);

      set_DetectorCount(m_Detectors.count());

      detector->pullPropertiesfromProxy(proxy);
    }
  }
}

void QxrdAcquisition::clearDetectors()
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "clearDetectors", Qt::BlockingQueuedConnection);
  } else {
    m_Detectors.resize(0);

    set_DetectorCount(0);
  }
}

QxrdDetectorThreadPtr QxrdAcquisition::detectorThread(int n)
{
  return m_DetectorThreads.value(n);
}

QxrdDetectorPtr QxrdAcquisition::detector(int n)
{
  return m_Detectors.value(n);
}

void QxrdAcquisition::setWindow(QxrdWindowWPtr win)
{
  m_Window = win;
}

void QxrdAcquisition::printMessage(QString msg, QDateTime ts) const
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->printMessage(msg, ts);
  }
}

void QxrdAcquisition::criticalMessage(QString msg, QDateTime ts) const
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->criticalMessage(msg, ts);
  }
}

void QxrdAcquisition::statusMessage(QString msg, QDateTime ts) const
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->statusMessage(msg, ts);
  }
}

void QxrdAcquisition::onBufferSizeChanged(int newMB)
{
  QcepAllocatorPtr alloc(m_Allocator);

  if (alloc) {
    alloc -> changedSizeMB(newMB);
  }
}

QcepAllocatorWPtr QxrdAcquisition::allocator() const
{
  return m_Allocator;
}

bool QxrdAcquisition::sanityCheckCommon()
{
  if (get_DetectorCount() == 0) {
    criticalMessage("No Detectors are configured");
    return false;
  }

  int ndet = 0;

  for (int i=0; i<get_DetectorCount(); i++) {
    QxrdDetectorPtr det = detector(i);

    if (det && det->isEnabled()) {
      ndet++;
    }
  }

  if (ndet == 0) {
    criticalMessage("No detectors are enabled");
    return false;
  }

  if (get_FilePattern() == "") {
    criticalMessage("No save file name has been provided");
    return false;
  }

  return true;
}

bool QxrdAcquisition::sanityCheckAcquire()
{
  bool res = false;

  if (sanityCheckCommon() == false) {
  } else if (get_PreTriggerFiles() < 0) {
    criticalMessage("pretrigger files cannot be < 0");
  } else if (get_PostTriggerFiles() < 0) {
    criticalMessage("posttrigger files cannot be < 0");
  } else if (get_PreTriggerFiles() + get_PostTriggerFiles() <= 0) {
    criticalMessage("pre+post trigger files must be >= 1");
  } else if (get_SummedExposures() <= 0) {
    criticalMessage("Summed exposures must be >= 1");
  } else if (get_SkippedExposures() < 0) {
    criticalMessage("Skipped exposures must be >= 0");
  } else if (get_ExposureTime() <= 0) {
    criticalMessage("Exposure time must be > 0");
  } else {
    res = true;
  }

  return res;
}

bool QxrdAcquisition::sanityCheckAcquireDark()
{
  bool res = false;

  if (sanityCheckCommon() == false) {
  } else if (get_DarkSummedExposures() <= 0) {
    criticalMessage("Dark summed exposures must be >= 0");
  } else if (get_ExposureTime() <= 0) {
    criticalMessage("Exposure time must be > 0");
  } else {
    res = true;
  }

  return res;
}

void QxrdAcquisition::onExposureTimeChanged()
{
  foreach (QxrdDetectorPtr det, m_Detectors) {
    det->onExposureTimeChanged();
  }
}

void QxrdAcquisition::configureDetector(int i)
{
  printMessage(tr("Configure Detector %1").arg(i));

  QxrdDetectorPtr det = detector(i);

  QxrdDetectorProxyPtr proxy(new QxrdDetectorProxy(detectorThread(i), detector(i), myself()));

  if (proxy && proxy->configureDetector()) {
    det->pullPropertiesfromProxy(proxy);
  }
}

void QxrdAcquisition::openDetectorControlWindow(int i)
{
  QxrdDetectorPtr det = detector(i);

  if (det) {
    det->openControlWindow();
  }
}

void QxrdAcquisition::clearDropped()
{
  prop_DroppedFrames() -> setValue(0);
}

void QxrdAcquisition::indicateDroppedFrame(int n)
{
  QcepAllocatorPtr alloc(m_Allocator);

  if (alloc) {
    QString msg = tr("Frame %1 dropped [%2/%3 MB Used]")
        .arg(n)
        .arg(alloc->allocatedMemoryMB())
        .arg(alloc->maximumMemoryMB());

    statusMessage(msg);
    printMessage(msg);

    prop_DroppedFrames() -> incValue(1);
  }
}

void QxrdAcquisition::accumulateAcquiredImage(QcepImageDataBasePtr image, QcepInt32ImageDataPtr accum, QcepMaskDataPtr overflow)
{
  QcepInt16ImageDataPtr img16 = qSharedPointerCast<QcepInt16ImageData>(image);

  if (img16) {
    accumulateAcquiredImage(img16, accum, overflow);
  } else {
    QcepInt32ImageDataPtr img32 = qSharedPointerCast<QcepInt32ImageData>(image);

    if (img32) {
      accumulateAcquiredImage(img32, accum, overflow);
    } else {
      printMessage("type conversion failed in QxrdAcquisition::accumulateAcquiredImage");
    }
  }
}

void QxrdAcquisition::accumulateAcquiredImage(QcepInt16ImageDataPtr image, QcepInt32ImageDataPtr accum, QcepMaskDataPtr overflow)
{
  if (image && accum && overflow) {
    long nPixels = image->get_Height()*image->get_Width();
    int ovflwlvl = get_OverflowLevel();
    quint16* src = image->data();
    quint32* dst = accum->data();
    short int* ovf = overflow->data();
    int nsummed = accum->get_SummedExposures();

    if (nsummed == 0) {
      if (qcepDebug(DEBUG_ACQUIRE)) {
        printMessage(tr("Frame %1 saved").arg(nsummed));
      }

      for (long i=0; i<nPixels; i++) {
        quint16 val = *src++;

        if (val>ovflwlvl) {
          *ovf++ = 1;
        } else {
          *ovf++ = 0;
        }

        *dst++ = val;
      }

      accum->set_Normalization(image->get_Normalization());
      accum->set_ExtraInputs(image->get_ExtraInputs());
      accum->set_SummedExposures(1);
      accum->set_ImageSequenceNumber(image->get_ImageSequenceNumber());
    } else {
      if (qcepDebug(DEBUG_ACQUIRE)) {
        printMessage(tr("Frame %1 summed").arg(nsummed));
      }

      for (long i=0; i<nPixels; i++) {
        quint16 val = *src++;

        if (val>ovflwlvl) {
          *ovf++ += 1;
        } else {
          ovf++;
        }

        *dst++ += val;
      }

      accum->prop_Normalization()   -> incValue(image->get_Normalization());
      accum->prop_ExtraInputs()     -> incValue(image->get_ExtraInputs());
      accum->prop_SummedExposures() -> incValue(1);
      accum->prop_ImageSequenceNumber() -> incValue(image->get_ImageSequenceNumber());
    }
  }
}

void QxrdAcquisition::accumulateAcquiredImage(QcepInt32ImageDataPtr image, QcepInt32ImageDataPtr accum, QcepMaskDataPtr overflow)
{
  if (image && accum && overflow) {
    long nPixels = image->get_Height()*image->get_Width();
    int ovflwlvl = get_OverflowLevel();
    quint32* src = image->data();
    quint32* dst = accum->data();
    short int* ovf = overflow->data();
    int nsummed = accum->get_SummedExposures();

    if (nsummed == 0) {
      if (qcepDebug(DEBUG_ACQUIRE)) {
        printMessage(tr("Frame %1 saved").arg(nsummed));
      }

      for (long i=0; i<nPixels; i++) {
        quint32 val = *src++;

        if (val>ovflwlvl) {
          *ovf++ = 1;
        } else {
          *ovf++ = 0;
        }

        *dst++ = val;
      }

      accum->set_Normalization(image->get_Normalization());
      accum->set_ExtraInputs(image->get_ExtraInputs());
      accum->set_SummedExposures(1);
      accum->set_ImageSequenceNumber(image->get_ImageSequenceNumber());
    } else {
      if (qcepDebug(DEBUG_ACQUIRE)) {
        printMessage(tr("Frame %1 summed").arg(nsummed));
      }

      for (long i=0; i<nPixels; i++) {
        quint32 val = *src++;

        if (val>ovflwlvl) {
          *ovf++ += 1;
        } else {
          ovf++;
        }

        *dst++ += val;
      }

      accum->prop_Normalization()   -> incValue(image->get_Normalization());
      accum->prop_ExtraInputs()     -> incValue(image->get_ExtraInputs());
      accum->prop_SummedExposures() -> incValue(1);
      accum->prop_ImageSequenceNumber() -> incValue(image->get_ImageSequenceNumber());
    }
  }
}

QString QxrdAcquisition::currentFileBase(int detNum)
{
  QString fileBase, fileName;

  QxrdDetectorPtr det = detector(detNum);
  QString extent;

  if (det) {
    extent = det->get_Extension();
  } else {
    extent = "tif";
  }

  getFileBaseAndName(
        get_FilePattern(),
        extent,
        detNum,
        get_CurrentFile(),
        get_CurrentPhase(),
        get_PhasesInGroup(),
        fileBase, fileName);

  return fileBase;
}

void QxrdAcquisition::getFileBaseAndName(QString filePattern, QString extent, int detNum, int fileIndex, int phase, int nphases, QString &fileBase, QString &fileName)
{
  int width = get_FileIndexWidth();
  int detWidth = get_DetectorNumberWidth();

  QxrdDataProcessorPtr proc(m_DataProcessor);
  int nDet = get_DetectorCount();

  if (proc) {
    if (nphases == 0 || phase < 0) {
      if (nDet <= 1) {
        fileBase = tr("%1-%2.dark.%3")
            .arg(filePattern).arg(fileIndex,width,10,QChar('0')).arg(extent);
      } else {
        fileBase = tr("%1-%2-%3.dark.%4")
            .arg(filePattern).arg(detNum,detWidth,10,QChar('0')).arg(fileIndex,width,10,QChar('0')).arg(extent);
      }

      fileName = QDir(proc -> darkOutputDirectory()).filePath(fileBase);
    } else {
      if (nphases > 1) {
        int phswidth = get_FilePhaseWidth();
        if (nDet <= 1) {
          fileBase = tr("%1-%2-%3.%4")
              .arg(filePattern).arg(fileIndex,width,10,QChar('0')).arg(phase,phswidth,10,QChar('0')).arg(extent);
        } else {
          fileBase = tr("%1-%2-%3-%4.%5")
              .arg(filePattern).arg(detNum,detWidth,10,QChar('0')).arg(fileIndex,width,10,QChar('0'))
              .arg(phase,phswidth,10,QChar('0')).arg(extent);
        }
      } else {
        if (nDet <= 1) {
          fileBase = tr("%1-%2.%3")
              .arg(filePattern).arg(fileIndex,width,10,QChar('0')).arg(extent);
        } else {
          fileBase = tr("%1-%2-%3.%4")
              .arg(filePattern).arg(detNum,detWidth,10,QChar('0')).arg(fileIndex,width,10,QChar('0')).arg(extent);
        }
      }
      fileName = QDir(proc -> rawOutputDirectory()).filePath(fileBase);
    }
  }
}

void QxrdAcquisition::processImage(QString filePattern, QString extent, int fileIndex, int phase, int nPhases, bool trig, QcepInt32ImageDataPtr image, QcepMaskDataPtr overflow)
{
  if (image) {
//    int w=image->get_Width(), h=image->get_Height();

//    QcepInt32ImageDataPtr proc = QcepAllocator::newInt32Image(m_Allocator, QcepAllocator::AllocateFromReserve, w,h);
//    QcepMaskDataPtr ovf = QcepAllocator::newMask(m_Allocator, QcepAllocator::AllocateFromReserve, w,h, 0);

//    if (proc == NULL || ovf == NULL) {
//      indicateDroppedFrame(0);
//      return;
//    }

//    proc->copyFrom(image);
//    overflow->copyMaskTo(ovf);

    if (qcepDebug(DEBUG_ACQUIRE) || qcepDebug(DEBUG_ACQUIRETIME)) {
      printMessage(tr("processAcquiredImage(%1,%2) %3 summed exposures")
                   .arg(fileIndex).arg(phase).arg(image->get_SummedExposures()));
    }

    QString fileName;
    QString fileBase;

    getFileBaseAndName(filePattern, extent, 0, fileIndex, phase, nPhases, fileBase, fileName);

    if (qcepDebug(DEBUG_ACQUIRE) || qcepDebug(DEBUG_ACQUIRETIME)) {
      printMessage(tr("Fn: %1, Fi: %2, Phs: %3")
                   .arg(fileName).arg(fileIndex).arg(phase));
    }

    set_FileBase(fileBase);
    //        m_DataProcessor -> set_FileName(fileName);

    QFileInfo finfo(fileName);

    QDateTime now = QDateTime::currentDateTime();
    double msec = QcepImageDataBase::secondsSinceEpoch();

    image -> set_FileBase(fileBase);
    image -> set_FileName(fileName);
    image -> set_Title(finfo.fileName());
    image -> set_ExposureTime(get_ExposureTime());
    image -> set_DateTime(now);
    image -> set_TimeStamp(msec);
    image -> set_HBinning(1);
    image -> set_VBinning(1);
    image -> set_DataType(QcepInt32ImageData::Raw32Data);
    image -> set_UserComment1(get_UserComment1());
    image -> set_UserComment2(get_UserComment2());
    image -> set_UserComment3(get_UserComment3());
    image -> set_UserComment4(get_UserComment4());
    image -> set_ObjectSaved(false);
    image -> set_Triggered(trig);
    image -> set_Normalization(get_Normalization());

    copyDynamicProperties(image.data());

    if (nPhases == 0) {
      if (qcepDebug(DEBUG_ACQUIRE) || qcepDebug(DEBUG_ACQUIRETIME)) {
        printMessage(tr("32 bit Dark Image acquired"));
      }

      image -> set_ImageNumber(-1);
      image -> set_PhaseNumber(-1);
      image -> set_NPhases(0);
    } else {
      if (qcepDebug(DEBUG_ACQUIRE) || qcepDebug(DEBUG_ACQUIRETIME)) {
        printMessage(tr("32 bit Image %1 acquired").arg(fileIndex));
      }

      image -> set_ImageNumber(fileIndex);
      image -> set_PhaseNumber(phase);
      image -> set_NPhases(nPhases);
    }

    QxrdDataProcessorPtr processor(m_DataProcessor);

    if (processor) {
      processor -> acquiredInt32Image(image, overflow);
    }
  }
}

void QxrdAcquisition::processImage(const QxrdProcessArgs &args)
{
  QThread::currentThread()->setObjectName("processImage");

  if (qcepDebug(DEBUG_ACQUIRETIME)) {
    printMessage(tr("QxrdAcquisition::processImage %1 %2 start").arg(args.m_FilePattern).arg(args.m_FileIndex));
  }

  processImage(args.m_FilePattern, args.m_Extension, args.m_FileIndex, args.m_Phase, args.m_NPhases, args.m_Trig, args.m_Image, args.m_Overflow);

  if (qcepDebug(DEBUG_ACQUIRETIME)) {
    printMessage(tr("QxrdAcquisition::processImage %1 %2 end").arg(args.m_FilePattern).arg(args.m_FileIndex));
  }
}

void QxrdAcquisition::processAcquiredImage(QString filePattern, QString extent, int fileIndex, int phase, int nPhases, bool trig, QcepInt32ImageDataPtr image, QcepMaskDataPtr overflow)
{
  //  printf("processAcquiredImage(""%s"",%d,%d,img,ovf)\n", qPrintable(filePattern), fileIndex, phase);

  //  processImage(filePattern, fileIndex, phase, nPhases, trig, image, overflow);

  if (qcepDebug(DEBUG_ACQUIRETIME)) {
    printMessage(tr("QxrdAcquisition::processAcquiredImage %1 %2 %3 (%4)").arg(filePattern).arg(fileIndex).arg(phase).arg((image?image->get_ImageNumber():-1)));
  }

  QtConcurrent::run(this, &QxrdAcquisition::processImage,
                    QxrdProcessArgs(filePattern, extent, fileIndex, phase, nPhases, trig, image, overflow));
}

void QxrdAcquisition::processDarkImage(QString filePattern, QString extent, int fileIndex, QcepInt32ImageDataPtr image, QcepMaskDataPtr overflow)
{
  //  printf("processDarkImage(""%s"",%d,img,ovf)\n", qPrintable(filePattern), fileIndex);

  processImage(filePattern, extent, fileIndex, -1, 0, true, image, overflow);
}

QxrdAcquisitionDialogPtr QxrdAcquisition::controlPanel(QxrdWindowWPtr win)
{
  if (win) {
    m_Window = win;

    m_ControlPanel = new QxrdAcquisitionDialog(m_Experiment,
                                               m_Window,
                                               myself(),
                                               m_DataProcessor,
                                               m_Window.data());

    return m_ControlPanel;
  } else {
    return NULL;
  }
}

void QxrdAcquisition::setNIDAQPlugin(QxrdNIDAQPluginInterfacePtr nidaqPlugin)
{
  if (m_SynchronizedAcquisition) {
    m_SynchronizedAcquisition -> setNIDAQPlugin(nidaqPlugin);
  }

  if (m_AcquisitionExtraInputs) {
    m_AcquisitionExtraInputs -> setNIDAQPlugin(nidaqPlugin);
  }
}

QxrdNIDAQPluginInterfacePtr QxrdAcquisition::nidaqPlugin() const
{
  if (m_SynchronizedAcquisition) {
    return m_SynchronizedAcquisition -> nidaqPlugin();
  } else {
    return QxrdNIDAQPluginInterfacePtr();
  }
}

QxrdSynchronizedAcquisitionPtr QxrdAcquisition::synchronizedAcquisition() const
{
  return m_SynchronizedAcquisition;
}

QxrdAcquisitionExtraInputsPtr QxrdAcquisition::acquisitionExtraInputs() const
{
  return m_AcquisitionExtraInputs;
}

int QxrdAcquisition::cancelling()
{
  int res = get_Cancelling();

  if (res) {
    printMessage(tr("Cancelling acquisition"));
    statusMessage(tr("Cancelling acquisition"));
  }

  return res;
}

void QxrdAcquisition::doAcquire()
{
  QxrdAcquisitionParameterPackPtr parmsp = acquisitionParameterPack();

  if (parmsp) {
    QTime acqTimer;
    acqTimer.start();

    QThread::currentThread()->setObjectName("doAcquire");

    stopIdling();

    QString fileBase = parmsp->fileBase();
    int fileIndex = get_FileIndex();
    double exposure = parmsp->exposure();
    int nsummed = parmsp->nsummed();
    int preTrigger = parmsp->preTrigger();
    int postTrigger = parmsp->postTrigger();
    int nphases = parmsp->nphases();
    int skipBefore = parmsp->skipBefore();

    if (skipBefore <= 0) {
      if (get_LastAcquired() != 1) {
        skipBefore = 1;
      }
    }

    set_LastAcquired(1);

    int skipBetween = parmsp->skipBetween();
    int nPreTriggered = 0;

    if (preTrigger <= 0) {
      set_Triggered(true);
    }

    int nDet = 0;
    QVector<QxrdDetectorPtr> dets;
    QVector<QxrdDetectorProcessorPtr> procs;
    QVector<QVector<QVector<QcepInt32ImageDataPtr> > >res;
    QVector<QVector<QVector<QcepMaskDataPtr> > >      ovf;

    for (int i=0; i<get_DetectorCount(); i++) {
      QxrdDetectorPtr det = detector(i);

      if (det && det->isEnabled()) {
        nDet++;
        dets.append(det);
        procs.append(det->processor());

        res.resize(nDet);
        ovf.resize(nDet);

        res[nDet-1].resize(nphases);
        ovf[nDet-1].resize(nphases);

        for (int p=0; p<nphases; p++) {
          res[nDet-1][p].resize(preTrigger+1);
          ovf[nDet-1][p].resize(preTrigger+1);
        }
      }
    }


    if (synchronizedAcquisition()) {
      synchronizedAcquisition()->prepareForAcquisition(parmsp);
    }

    if (acquisitionExtraInputs()) {
      acquisitionExtraInputs()->prepareForAcquisition(parmsp);
    }

    for (int d=0; d<nDet; d++) {
      dets[d] -> beginAcquisition(exposure);
    }

    printMessage(tr("acquire(\"%1\", %2, %3, %4, %5, %6) // fileIndex = %7")
                 .arg(fileBase).arg(exposure).arg(nsummed).arg(postTrigger).arg(preTrigger).arg(nphases).arg(fileIndex));


    set_CurrentPhase(0);
    set_CurrentSummation(-1);
    set_CurrentFile(fileIndex);

    for (int i=0; i<skipBefore; i++) {
      if (cancelling()) goto cancel;
      if (qcepDebug(DEBUG_ACQUIRETIME)) {
        printMessage(tr("Skipping %1 of %2").arg(i+1).arg(skipBefore));
      }

      for (int d=0; d<nDet; d++) {
        dets[d]->beginFrame();
      }

      for (int d=0; d<nDet; d++) {
        dets[d]->acquireFrame();
      }
    }

    for (int i=0; i<postTrigger; i += (get_Triggered() ? 1:0)) {
      if (cancelling()) goto cancel;

      set_CurrentFile(fileIndex);

      if (i != 0) {
        for (int k=0; k<skipBetween; k++) {
          if (cancelling()) goto cancel;
          if (qcepDebug(DEBUG_ACQUIRETIME)) {
            printMessage(tr("Skipping %1 of %2").arg(k+1).arg(skipBetween));
          }

          for (int d=0; d<nDet; d++) {
            dets[d]->acquireFrame();
          }

          if (qcepDebug(DEBUG_ACQUIRETIME)) {
            printMessage(tr("Frame after %1 msec").arg(acqTimer.restart()));
          }
        }
      }

      if (cancelling()) goto cancel;

      for (int s=0; s<nsummed;) {
        set_CurrentSummation(s);

        for (int p=0; p<nphases; p++) {
          set_CurrentPhase(p);

          for (int d=0; d<nDet; d++) {
            QxrdDetectorPtr det = dets[d];

            det->beginFrame();
          }

          for (int d=0; d<nDet; d++) {
            QxrdDetectorPtr det = dets[d];
            int nCols = det->get_NCols();
            int nRows = det->get_NRows();

            if (res[d][p][0] == NULL) {
              QcepInt32ImageDataPtr nres = QcepAllocator::newInt32Image(QcepAllocator::AllocateFromReserve,
                                                                        nCols, nRows, this);
              res[d][p][0] = nres;

              if (nres == NULL) {
                if (qcepDebug(DEBUG_ACQUIRETIME)) {
                  printMessage("Dropped frame allocation...");
                }
                indicateDroppedFrame(i);
              } else {
                QString fb, fn;
                if (qcepDebug(DEBUG_ACQUIRETIME)) {
                  printMessage(tr("Newly allocated image number %1").arg(nres->get_ImageNumber()));
                }

                nres -> set_SummedExposures(0);

                getFileBaseAndName(fileBase, det->get_Extension(),
                                   det->get_DetectorNumber(),
                                   fileIndex, p, nphases, fb, fn);

                nres -> set_FileBase(fb);
                nres -> set_FileName(fn);
              }
            }

            if (ovf[d][p][0] == NULL) {
              QcepMaskDataPtr novf = QcepAllocator::newMask(QcepAllocator::AllocateFromReserve,
                                                            nCols, nRows, 0, this);
              ovf[d][p][0] = novf;

              if (novf == NULL) {
                if (qcepDebug(DEBUG_ACQUIRETIME)) {
                  printMessage("Dropped mask frame allocation...");
                }
                indicateDroppedFrame(i);
              } else {
                if (qcepDebug(DEBUG_ACQUIRETIME)) {
                  printMessage(tr("Newly allocated mask number %1").arg(novf->get_ImageNumber()));
                }
              }

              if (novf) ovf[d][p][0] -> set_SummedExposures(0);
            }

            if (res[d][p][0]) {
              emit acquiredFrame(res[d][p][0]->get_FileBase(),
                  p, nphases,
                  s, nsummed,
                  i, postTrigger);
            }

            QcepImageDataBasePtr img = det -> acquireFrame();

            if (img && res[d][p][0] && ovf[d][p][0]) {
              accumulateAcquiredImage(img, res[d][p][0], ovf[d][p][0]);
            } else if (!cancelling()){
              indicateDroppedFrame(i);
            }

            if (qcepDebug(DEBUG_ACQUIRETIME)) {
              printMessage(tr("accumulateAcquiredImage %1 msec idx:%2 post:%3 sum: %4 ph:%5")
                           .arg(acqTimer.restart())
                           .arg(fileIndex)
                           .arg(i)
                           .arg(s)
                           .arg(p)
                           );
            }

            if (cancelling()) goto saveCancel;
          }
        }

        if (get_RetryDropped()) {
          int minSum = nsummed+10;

          for (int p=0; p<nphases; p++) {
            for (int d=0; d<nDet; d++) {
              if (res[d][p][0]) {
                int ns = res[d][p][0]->get_SummedExposures();

                if (ns < minSum) {
                  minSum = ns;
                }
              } else {
                minSum = 0;
              }
            }
          }

          if (qcepDebug(DEBUG_ACQUIRETIME)) {
            printMessage(tr("i = %1, Minsum = %2, s = %3, nsummed = %4").arg(i).arg(minSum).arg(s).arg(nsummed));
          }

          if (minSum == nsummed+10) {
            if (qcepDebug(DEBUG_ACQUIRETIME)) {
              printMessage("No acquired images allocated");
            }
            //          s = s+1;
          } else {
            s = minSum;
          }
        } else {
          s = s+1;
        }
      }

saveCancel:
      if (get_Triggered()) {
        int nPre = qMin(preTrigger, nPreTriggered);

        for (int ii=nPre; ii >= 1; ii--) {
          for (int p=0; p<nphases; p++) {
            for (int d=0; d<nDet; d++) {

              procs[d] -> processAcquiredImage(fileBase, fileIndex, p, nphases, false, res[d][p][ii], ovf[d][p][ii]);

              if (qcepDebug(DEBUG_ACQUIRETIME)) {
                printMessage(tr("processAcquiredImage(line %1) %2 msec idx:%3 pre:%4 ph:%5")
                             .arg(__LINE__)
                             .arg(acqTimer.restart())
                             .arg(fileIndex)
                             .arg(ii)
                             .arg(p)
                             );
              }

              res[d][p].pop_back();
              ovf[d][p].pop_back();
            }
          }

          fileIndex++;
          set_FileIndex(fileIndex);
        }

        nPreTriggered = 0;

        for (int p=0; p<nphases; p++) {
          for (int d=0; d<nDet; d++) {
            procs[d] -> processAcquiredImage(fileBase, fileIndex, p, nphases, true, res[d][p][0], ovf[d][p][0]);

            if (qcepDebug(DEBUG_ACQUIRETIME)) {
              printMessage(tr("processAcquiredImage(line %1) %2 msec idx:%3 pre:%4 ph:%5")
                           .arg(__LINE__)
                           .arg(acqTimer.restart())
                           .arg(fileIndex)
                           .arg(i)
                           .arg(p)
                           );
            }

            res[d][p][0] = QcepInt32ImageDataPtr();
            ovf[d][p][0] = QcepMaskDataPtr();
          }
        }
        fileIndex++;
        set_FileIndex(fileIndex);
      } else {
        nPreTriggered++;
        for (int p=0; p<nphases; p++) {
          for (int d=0; d<nDet; d++) {
            res[d][p].push_front(QcepInt32ImageDataPtr());
            ovf[d][p].push_front(QcepMaskDataPtr());
            res[d][p].pop_back();
            ovf[d][p].pop_back();
          }
        }
      }
    }

    statusMessage(tr("Acquisition complete"));
    printMessage(tr("Acquisition complete"));

cancel:
    if (synchronizedAcquisition()) {
      synchronizedAcquisition()->finishedAcquisition();
    }

    for (int d=0; d<nDet; d++) {
      dets[d]->endAcquisition();
    }

    startIdling();
  }

  m_Acquiring.unlock();

  emit acquireComplete();
}

void QxrdAcquisition::doAcquireDark()
{
  QxrdDarkAcquisitionParameterPackPtr parmsp = darkAcquisitionParameterPack();

  if (parmsp) {
    QThread::currentThread()->setObjectName("doAcquireDark");

    stopIdling();

    QString fileBase = parmsp->fileBase();
    int fileIndex = get_FileIndex();
    double exposure = parmsp->exposure();
    int nsummed = parmsp->nsummed();
    int skipBefore = parmsp->skipBefore();

    if (skipBefore <= 0) {
      if (get_LastAcquired() != -1) {
        skipBefore = 1;
      }
    }

    set_LastAcquired(-1);

    printMessage(tr("acquireDark(\"%1\", %2, %3) // fileIndex = %4")
                 .arg(fileBase).arg(exposure).arg(nsummed).arg(fileIndex));

    int nDet = 0;
    QVector<QxrdDetectorPtr> dets;
    QVector<QxrdDetectorProcessorPtr> procs;
    QVector<QcepInt32ImageDataPtr> res;
    QVector<QcepMaskDataPtr> overflow;

    set_CurrentPhase(-1);
    set_CurrentSummation(0);
    set_CurrentFile(fileIndex);

    for (int i=0; i<get_DetectorCount(); i++) {
      QxrdDetectorPtr det = detector(i);

      if (det && det->isEnabled()) {
        nDet++;
        dets.append(det);
        procs.append(det->processor());

        res.append(QcepAllocator::newInt32Image(QcepAllocator::AllocateFromReserve,
                                                det->get_NCols(), det->get_NRows(), this));

        overflow.append(QcepAllocator::newMask(QcepAllocator::AllocateFromReserve,
                                               det->get_NCols(), det->get_NRows(),0, this));
      }
    }

    if (synchronizedAcquisition()) {
      synchronizedAcquisition()->prepareForDarkAcquisition(parmsp);
    }

    QString fb, fn;

    for (int d=0; d<nDet; d++) {
      if (res[d] == NULL || overflow[d] == NULL) {
        criticalMessage("Insufficient memory for acquisition operation");
        goto cancel;
      }
    }

    for (int d=0; d<nDet; d++) {
      dets[d] -> beginAcquisition(exposure);
    }

    for (int d=0; d<nDet; d++) {
      QxrdDetectorPtr det = dets[d];

      getFileBaseAndName(fileBase, det->get_Extension(), det->get_DetectorNumber(), fileIndex, -1, 1, fb, fn);

      res[d] -> set_FileBase(fb);
      res[d] -> set_FileName(fn);
    }

    for (int i=0; i<skipBefore; i++) {
      if (cancelling()) goto cancel;

      set_CurrentSummation(-1);

      for (int d=0; d<nDet; d++) {
        dets[i] -> beginFrame();
      }

      if (qcepDebug(DEBUG_ACQUIRETIME)) {
        printMessage(tr("Skipping %1 of %2").arg(i+1).arg(skipBefore));
      }

      for (int d=0; d<nDet; d++) {
        dets[i] -> acquireFrame();
      }
    }

    for (int i=0; i<nsummed;) {
      if (cancelling()) goto cancel;

      set_CurrentSummation(i);

      for (int d=0; d<nDet; d++) {
        dets[d] -> beginFrame();
      }

      for (int d=0; d<nDet; d++) {
        emit acquiredFrame(res[d]->get_FileBase(), 0, 1, i, nsummed, 0, 1);

        QcepImageDataBasePtr img =
            dets[d] -> acquireFrame();

        if (img) {
          accumulateAcquiredImage(img, res[d], overflow[d]);
        } else {
          if (!cancelling()){
            indicateDroppedFrame(0);
          } else {
            goto cancel;
          }
        }
      }

      if (get_RetryDropped()) {
        int minSum = nsummed+10;

        for (int d=0; d<nDet; d++) {
          if (res[d]) {
            int ns = res[d]->get_SummedExposures();

            if (ns < minSum) {
              minSum = ns;
            }
          } else {
            minSum = 0;
          }
        }

        i = minSum;
      } else{
        i = i+1;
      }
    }

    //saveCancel:

    for (int d=0; d<nDet; d++) {
      procs[d]->processDarkImage(fileBase, fileIndex, res[d], overflow[d]);
    }

    statusMessage(tr("Acquisition complete"));
    printMessage(tr("Acquisition complete"));

cancel:
    if (synchronizedAcquisition()) {
      synchronizedAcquisition()->finishedAcquisition();
    }

    startIdling();
  }

  m_Acquiring.unlock();

  emit acquireComplete();
}

void QxrdAcquisition::stopIdling()
{
  m_Idling.fetchAndStoreOrdered(0);
//  flushImageQueue();

  if (get_AcquisitionCancelsLiveView()) {
    set_LiveViewAtIdle(false);
  }
}

void QxrdAcquisition::startIdling()
{
  m_Idling.fetchAndStoreOrdered(1);
}

void QxrdAcquisition::onIdleTimeout()
{
  if (m_Idling.fetchAndAddOrdered(0)) {
    for (int i=0; i<get_DetectorCount(); i++) {
      QxrdDetectorPtr det = detector(i);

      if (det && det->isEnabled()) {
        QcepImageDataBasePtr res = det -> acquireFrameIfAvailable();

        QxrdDetectorProcessorPtr proc = det->processor();

        if (proc) {
          proc->processIdleImage(res);
        }
      }
    }
//    get_ExposureTime());
//    QxrdDataProcessorPtr proc(m_DataProcessor);

//    if (res && proc) {
//      res -> set_ExposureTime(get_ExposureTime());
//      res -> set_DateTime(QDateTime::currentDateTime());
//      res -> set_HBinning(1);
//      res -> set_VBinning(1);
//      res -> set_DataType(QcepInt32ImageData::Raw32Data);
//      res -> set_UserComment1(get_UserComment1());
//      res -> set_UserComment2(get_UserComment2());
//      res -> set_UserComment3(get_UserComment3());
//      res -> set_UserComment4(get_UserComment4());
//      res -> set_ObjectSaved(false);

//      proc->idleInt16Image(res, this->get_LiveViewAtIdle());
//    }
  }
}

//void QxrdAcquisition::flushImageQueue()
//{
//  int n = m_NAcquiredImages.available();
//  m_NAcquiredImages.acquire(n);

//  for (int i=0; i<n; i++) {
//    m_AcquiredImages.dequeue();
//  }
//}

//QcepInt16ImageDataPtr QxrdAcquisition::acquireFrame(double exposure)
//{
//  if (qcepDebug(DEBUG_ACQUIRE)) {
//    QcepAllocatorPtr alloc(m_Allocator);

//    if (alloc) {
//      printMessage(tr("acquireFrame(%1) : allocated %2 MB : %3 images available")
//                   .arg(exposure)
//                   .arg(alloc->allocatedMemoryMB())
//                   .arg(m_NAcquiredImages.available())
//                   );
//    }
//  }

//  m_NAcquiredImages.acquire(1);

//  QcepInt16ImageDataPtr res = m_AcquiredImages.dequeue();

//  if (qcepDebug(DEBUG_EXTRAINPUTS) && res) {
//    QcepDoubleList extra = res->get_ExtraInputs();

//    for (int i=0; i<extra.count(); i++) {
//      printMessage(tr("QxrdAcquisition::acquireFrame : Extra Inputs[%1] = %2").arg(i).arg(extra[i]));
//    }
//  }

//  return res;
//}

//QcepInt16ImageDataPtr QxrdAcquisition::acquireFrameIfAvailable(double exposure)
//{
//  if (qcepDebug(DEBUG_ACQUIRE)) {
//    QcepAllocatorPtr alloc(m_Allocator);

//    if (alloc) {
//      printMessage(tr("acquireFrameIfAvailable(%1) : allocated %2 MB").arg(exposure).arg(alloc->allocatedMemoryMB()));
//    }
//  }

//  QcepInt16ImageDataPtr res;

//  while (m_NAcquiredImages.available() >= 1) {
//    res = acquireFrame(exposure);
//  }

//  return res;
//}

//void QxrdAcquisition::enqueueAcquiredFrame(QcepInt16ImageDataPtr img)
//{
//  if (m_AcquisitionExtraInputs) {
//    m_AcquisitionExtraInputs->acquire();
//    m_AcquisitionExtraInputs->logToImage(img);
//  }

//  if (qcepDebug(DEBUG_EXTRAINPUTS) && img) {
//    QcepDoubleList extra = img->get_ExtraInputs();

//    printMessage(tr("QxrdAcquisition::enqueueAcquiredFrame : %1 Extra Inputs").arg(extra.count()));

//    for (int i=0; i<extra.count(); i++) {
//      printMessage(tr("QxrdAcquisition::enqueueAcquiredFrame : Extra Inputs[%1] = %2").arg(i).arg(extra[i]));
//    }
//  }

//  m_AcquiredImages.enqueue(img);

//  m_NAcquiredImages.release(1);
//}

void QxrdAcquisition::Message(QString msg)
{
  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    expt->statusMessage(msg);
  }
}

void QxrdAcquisition::propertyList()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  const QMetaObject *meta = metaObject();

  int count = meta->propertyCount();
  int offset = 1; /*meta->propertyOffset();*/

  for (int i=offset; i<count; i++) {
    QMetaProperty metaproperty = meta->property(i);

    const char *name = metaproperty.name();
    QVariant value = property(name);

    if (g_Application) {
      g_Application->printMessage(tr("Property %1: %2 = %3").arg(i).arg(name).arg(value.toString()));
    }
  }

  QList<QByteArray> dynProps = dynamicPropertyNames();

  foreach(QByteArray name, dynProps) {
    g_Application->printMessage(tr("Dynamic Property %1 = %2").arg(QString(name)).arg(property(name).toString()));
  }
}

void QxrdAcquisition::setupExposureMenu(QDoubleSpinBox *cb)
{
}

