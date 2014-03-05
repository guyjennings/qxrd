#include "qxrddebug.h"
#include "qxrddetectorperkinelmer.h"
#include "qxrdapplication.h"
#include "qxrdperkinelmerplugininterface.h"
#include "qxrdacquisition.h"
#include "qxrdallocator.h"
#include "qxrdmutexlocker.h"

//static QxrdDetectorPerkinElmer *g_Detector = NULL;
static void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc);
static void CALLBACK OnEndAcqCallback(HACQDESC hAcqDesc);

QxrdDetectorPerkinElmer::QxrdDetectorPerkinElmer(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq) :
  QxrdDetector(expt, acq),
  m_Mutex(QMutex::Recursive),
  m_BufferSize(0),
  m_AcqDesc(NULL),
  m_StartupDelayed(0),
  m_DetectorNumber(0),
  m_PROMID(-1),
  m_HeaderID(-1),
  m_CameraType(-1),
  m_CameraModel(""),
  m_CurrentGain(-1),
  m_SyncMode(HIS_SYNCMODE_INTERNAL_TIMER),
  m_Counter(0),
  m_PerkinElmer()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorPerkinElmer::QxrdDetectorPerkinElmer(%p)\n", this);
  }

  if (qcepDebug(DEBUG_PERKINELMER)) {
    printMessage("QxrdDetectorPerkinElmer::QxrdDetectorPerkinElmer()");
  }

//  ::g_Detector = this;
}

QxrdDetectorPerkinElmer::~QxrdDetectorPerkinElmer()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorPerkinElmer::~QxrdDetectorPerkinElmer(%p)\n", this);
  }

  if (qcepDebug(DEBUG_PERKINELMER)) {
    printMessage("QxrdDetectorPerkinElmer::~QxrdDetectorPerkinElmer()");
  }

  QxrdPerkinElmerPluginInterfacePtr plugin(m_PerkinElmer);

  if (plugin && m_AcqDesc) {
    plugin->Acquisition_SetAcqData(m_AcqDesc, NULL);
    plugin->Acquisition_SetCallbacksAndMessages(m_AcqDesc, NULL, 0, 0, NULL, NULL);
    plugin->Acquisition_Abort(m_AcqDesc);
    plugin->Acquisition_CloseAll();
  }
}

int QxrdDetectorPerkinElmer::detectorType() const
{
  return QxrdDetectorThread::PerkinElmerDetector;
}

void QxrdDetectorPerkinElmer::printMessage(QString msg, QDateTime ts)
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->printMessage(msg, ts);
  }
}

void QxrdDetectorPerkinElmer::criticalMessage(QString msg, QDateTime ts)
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->criticalMessage(msg);
  }
}

void QxrdDetectorPerkinElmer::statusMessage(QString msg, QDateTime ts)
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->statusMessage(msg);
  }
}

void QxrdDetectorPerkinElmer::acquisitionError(const char *fn, int ln, int n)
{
  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    acq -> cancel();
  }

  criticalMessage(tr("Acquisition Error %1 at line %2 in file %3").arg(n).arg(ln).arg(fn));
}

bool QxrdDetectorPerkinElmer::checkPluginAvailable()
{
  if (g_Application && m_PerkinElmer == NULL) {
    m_PerkinElmer = g_Application->perkinElmerPlugin();
  }

  if (m_PerkinElmer == NULL) {
    criticalMessage("No perkin elmer plugin available");
  }

  return m_PerkinElmer;
}

/*
 Sorting Mode Values:

 Det.             S Mode     Hdr ID
 RID 128            1
 RID 256            2
 RID 128-400        3
 RID 1024-100       4
 RID 512-400 A0     5
 XRD 512-400 A1/A2  6
 XRD 0840           6
 XRD 512-400 E      7
 XRD 1640 A         8
 XRD 0820           8
 XRD 1620 A         8
 XRD 1640 A         9
 XRD 1620/21 AM/AN  11
 XRD 1620/40 AN CS  12

 Header ID Values:
 All except AM/AN   10
 XRD 16x0 AM        11
 XRD 16x0 AN        12,13
 XRD 1621 AN        14
 */

void QxrdDetectorPerkinElmer::initialize()
{
  if (qcepDebug(DEBUG_PERKINELMER)) {
    printMessage(tr("QxrdAcquisitionPerkinElmer::initialize"));
  }

  if (checkPluginAvailable()) {
    THREAD_CHECK;

    int nRet = HIS_ALL_OK;
    UINT nSensors;
    BOOL bEnableIRQ = true;
    ACQDESCPOS Pos = 0;
    UINT nChannelType;
    int nChannelNr;
    UINT dwSortFlags, dwDataType, dwRows, dwColumns, dwFrames;
    DWORD dwAcqType, dwSystemID, dwSyncMode, dwHwAccess;
    WORD binningMode;

    QxrdExperimentPtr exp(m_Experiment);

    int detNum  = 0;

    if (exp) {
      detNum = exp->get_DetectorNumber();
    }

    QxrdPerkinElmerPluginInterfacePtr plugin(m_PerkinElmer);

    if (plugin) {
      nRet = plugin->Acquisition_EnumSensors(&nSensors, bEnableIRQ, FALSE);
    }

    if (qcepDebug(DEBUG_PERKINELMER)) {
      printMessage(tr("Acquisition_EnumSensors = %1").arg(nRet));
    }

    if (nRet != HIS_ALL_OK) {
      acquisitionInitError(nRet);
      return;
    }

    if (qcepDebug(DEBUG_PERKINELMER)) {
      printMessage(tr("Number of sensors = %1").arg(nSensors));
    }

    if (detNum == 0 && nSensors != 1) {
      acquisitionNSensorsError(nRet);
      return;
    } else if (detNum < 0 || detNum > nSensors) {
      acquisitionNSensorsError(nRet);
      return;
    }

    for (int i=1; i<=(detNum?detNum:1); i++) {
      if (plugin && (nRet = plugin->Acquisition_GetNextSensor(&Pos, &m_AcqDesc))!=HIS_ALL_OK) {
        acquisitionNSensorsError(nRet);
        return;
      }
    }

    if (plugin && (nRet = plugin->Acquisition_GetCommChannel(m_AcqDesc, &nChannelType, &nChannelNr))!=HIS_ALL_OK) {
      acquisitionError(__FILE__, __LINE__, nRet);
      return;
    }

    if (qcepDebug(DEBUG_PERKINELMER)) {
      printMessage(tr("Acquisition_GetCommChannel channel type = %1, channel no = %2")
                        .arg(nChannelType).arg(nChannelNr));
    }

    if (plugin && (nRet=plugin->Acquisition_GetConfiguration(m_AcqDesc, &dwFrames, &dwRows, &dwColumns, &dwDataType,
                                           &dwSortFlags, &bEnableIRQ, &dwAcqType,
                                           &dwSystemID, &dwSyncMode, &dwHwAccess))!=HIS_ALL_OK) {
      acquisitionError(__FILE__, __LINE__, nRet);
      return;
    }

    if (qcepDebug(DEBUG_PERKINELMER)) {
      printMessage(tr("Acquisition_GetConfiguration frames = %1, rows = %2, cols = %3")
                   .arg(dwFrames).arg(dwRows).arg(dwColumns));
      printMessage(tr("Acquisition_GetConfiguration data type = %1, sort flags = %2, IRQ = %3")
                   .arg(dwDataType).arg(dwSortFlags).arg(bEnableIRQ));
      printMessage(tr("Acquisition_GetConfiguration acq type = %1, systemID = %2, syncMode = %3, hwAccess = %4")
                   .arg(dwAcqType).arg(dwSystemID).arg(dwSyncMode).arg(dwHwAccess));
    }

    QxrdAcquisitionPtr acq(m_Acquisition);

    if (acq) {
      acq->set_NRows(dwRows);
      acq->set_NCols(dwColumns);
    }

    if (plugin && (nRet=plugin->Acquisition_GetCameraBinningMode(m_AcqDesc, &binningMode)) != HIS_ALL_OK) {
      acquisitionError(__FILE__, __LINE__, nRet);
      return;
    }

    if (qcepDebug(DEBUG_PERKINELMER)) {
      printMessage(tr("Acquisition_GetCameraBinningMode mode = %1").arg(binningMode));
    }

    CHwHeaderInfo hwHeaderInfo;

    if (plugin && (nRet=plugin->Acquisition_GetHwHeaderInfo(m_AcqDesc, &hwHeaderInfo)) != HIS_ALL_OK) {
      acquisitionError(__FILE__, __LINE__, nRet);
      return;
    }

    if (qcepDebug(DEBUG_PERKINELMER)) {
      printMessage(tr("Prom ID %1, Header ID %2").arg(hwHeaderInfo.dwPROMID).arg(hwHeaderInfo.dwHeaderID));
    }

    m_PROMID = hwHeaderInfo.dwPROMID;
    m_HeaderID = hwHeaderInfo.dwHeaderID;

    if (hwHeaderInfo.dwHeaderID >= 14) {
      CHwHeaderInfoEx hdrx;

      if (plugin && (nRet = plugin->Acquisition_GetHwHeaderInfoEx(m_AcqDesc, &hwHeaderInfo, &hdrx)) != HIS_ALL_OK) {
        acquisitionError(__FILE__, __LINE__, nRet);
        return;
      }

      if (qcepDebug(DEBUG_PERKINELMER)) {
        printMessage(tr("Camera Type %1").arg(hdrx.wCameratype));
      }

      m_CameraType = hdrx.wCameratype;
    }

    int nReadoutTimes = 8;
    double readoutTimes[8];

    if (plugin && (nRet = plugin->Acquisition_GetIntTimes(m_AcqDesc, readoutTimes, &nReadoutTimes)) != HIS_ALL_OK) {
      acquisitionError(__FILE__, __LINE__, nRet);
      return;
    }

    for (int i=0; i<nReadoutTimes; i++) {
      m_ReadoutTimes.append(readoutTimes[i]);
    }

    if (qcepDebug(DEBUG_PERKINELMER)) {
      printMessage(tr("%1 predefined exposure times available").arg(m_ReadoutTimes.count()));

      for (int i=0; i<nReadoutTimes; i++) {
        printMessage(tr("Exp %1 = %2").arg(i).arg(m_ReadoutTimes[i]));
      }
    }

    if (plugin && (nRet = plugin->Acquisition_SetAcqData(m_AcqDesc, (ACQDATATYPE) this)) != HIS_ALL_OK) {
      acquisitionError(__FILE__, __LINE__, nRet);
      return;
    }

    if (plugin && (nRet = plugin->Acquisition_SetCallbacksAndMessages(m_AcqDesc, NULL, 0,
                                                  0, OnEndFrameCallback, OnEndAcqCallback))!=HIS_ALL_OK) {
      acquisitionError(__FILE__, __LINE__, nRet);
      return;
    }

    m_BufferSize = 4;
    m_BufferIndex = 0;

    if (acq) {
      m_Buffer.resize(acq->get_NRows()*acq->get_NCols()*m_BufferSize);
      m_Buffer.fill(0);

      if (qcepDebug(DEBUG_PERKINELMER)) {
        printMessage(tr("Exposure Time = %1").arg(acq->get_ExposureTime()));
      }
    }

    if (qcepDebug(DEBUG_PERKINELMER)) {
      printMessage(tr("SetFrameSyncMode HIS_SYNCMODE_INTERNAL_TIMER"));
    }

    if (plugin && (nRet=plugin->Acquisition_SetFrameSyncMode(m_AcqDesc, HIS_SYNCMODE_INTERNAL_TIMER)) != HIS_ALL_OK) {
      if (plugin && (nRet=plugin->Acquisition_SetFrameSyncMode(m_AcqDesc, HIS_SYNCMODE_FREE_RUNNING)) != HIS_ALL_OK) {
        acquisitionError(__FILE__, __LINE__, nRet);
        return;
      } else {
        m_SyncMode = HIS_SYNCMODE_FREE_RUNNING;
      }
    } else {
      m_SyncMode = HIS_SYNCMODE_INTERNAL_TIMER;
    }

    if (qcepDebug(DEBUG_PERKINELMER)) {
      printMessage(tr("Sync Mode = %1").arg(m_SyncMode));
    }

    if (acq) {
      onCameraGainChanged();

      if (acq->get_ExposureTime() <= 0) {
        acq->set_ExposureTime(0.135);
      }

      onExposureTimeChanged();

      acq->set_BinningMode(0);

      if (plugin && (nRet=plugin->Acquisition_DefineDestBuffers(m_AcqDesc, m_Buffer.data(), m_BufferSize,
                                                                       acq->get_NRows(), acq->get_NCols())) != HIS_ALL_OK) {
        acquisitionError(__FILE__, __LINE__, nRet);
        return;
      }
    }

    if (qcepDebug(DEBUG_PERKINELMER)) {
      printMessage(tr("Define Dest Buffers"));
    }

    if (qcepDebug(DEBUG_DELAY_ACQ)) {
      m_StartupDelayed = 1;
    } else {
      startupAcquisition();
    }
  }
}

void QxrdDetectorPerkinElmer::onExposureTimeChanged()
{
  if (checkPluginAvailable()) {
    QxrdPerkinElmerPluginInterfacePtr plugin(m_PerkinElmer);
    QxrdAcquisitionPtr acq(m_Acquisition);

    if (plugin && acq) {
      double newTime = acq->get_ExposureTime();
      if (newTime*1e6 < m_ReadoutTimes.value(0)) {
        printMessage(tr("Attempt to set exposure time less than minimum supported (%1 < %2)")
                     .arg(newTime).arg(m_ReadoutTimes.value(0)/1e6));

        newTime = m_ReadoutTimes.value(0)/1e6;
      }

      printMessage(tr("Exposure time changed to %1").arg(newTime));

      DWORD tmp = (int)(newTime*1e6);

      printMessage(tr("SetTimerSync %1").arg(tmp));

      int nRet;

      if ((nRet=plugin->Acquisition_SetTimerSync(m_AcqDesc, &tmp)) != HIS_ALL_OK) {
        acquisitionError(__FILE__, __LINE__, nRet);
        return;
      }

      printMessage(tr("TimerSync = %1").arg(tmp));

      acq->set_ExposureTime(tmp/1.0e6);
    }
  }
}

void QxrdDetectorPerkinElmer::onBinningModeChanged()
{
  return;

  if (checkPluginAvailable()) {
    QxrdPerkinElmerPluginInterfacePtr plugin(m_PerkinElmer);
    QxrdAcquisitionPtr acq(m_Acquisition);

    if (plugin && acq) {
      if (m_HeaderID == 14) {
        int newMode = acq->get_BinningMode();

        printMessage(tr("Binning mode changed to %1").arg(newMode));

        int nRet;
        WORD binningMode = newMode;
        WORD originalMode;

        //      if (newMode == 1) {
        //        if (m_CameraType == 1) {
        //          newMode = 2;
        //        } else {
        //          newMode = 258;
        //        }
        //      }
        if ((nRet=plugin->Acquisition_GetCameraBinningMode(m_AcqDesc, &originalMode)) != HIS_ALL_OK) {
          acquisitionError(__FILE__, __LINE__, nRet);
          return;
        }

        printMessage(tr("Starting binning mode = %1").arg(originalMode));

        printMessage(tr("Setting binning mode = %1").arg(newMode));

        if ((nRet=plugin->Acquisition_SetCameraBinningMode(m_AcqDesc, newMode)) != HIS_ALL_OK) {
          acquisitionError(__FILE__, __LINE__, nRet);
          return;
        }

        if ((nRet=plugin->Acquisition_GetCameraBinningMode(m_AcqDesc, &binningMode)) != HIS_ALL_OK) {
          acquisitionError(__FILE__, __LINE__, nRet);
          return;
        }

        printMessage(tr("Binning mode was set to %1").arg(binningMode));
      }
    }
  }
}

void QxrdDetectorPerkinElmer::onCameraGainChanged()
{
  if (checkPluginAvailable()) {
    QxrdPerkinElmerPluginInterfacePtr plugin(m_PerkinElmer);
    QxrdAcquisitionPtr  acq(m_Acquisition);

    if (acq && plugin) {
      if (m_HeaderID >= 11) {
        int newGain = acq->get_CameraGain();

        printMessage(tr("Camera Gain Changed to %1").arg(newGain));

        printMessage("Setting camera gain");

        int nRet;

        if (m_CurrentGain != acq->get_CameraGain()) {
          if ((nRet=plugin->Acquisition_SetCameraGain(m_AcqDesc, acq->get_CameraGain())) != HIS_ALL_OK) {
            acquisitionError(__FILE__, __LINE__, nRet);
            return;
          }
          m_CurrentGain = acq->get_CameraGain();
        }

        printMessage("Set camera gain");
      }
    }
  }
}

void QxrdDetectorPerkinElmer::startupAcquisition()
{
  if (checkPluginAvailable()) {
    QxrdPerkinElmerPluginInterfacePtr plugin(m_PerkinElmer);

    int nRet;

    if (plugin && (nRet=plugin->Acquisition_Acquire_Image(m_AcqDesc, m_BufferSize,
                                                          0, HIS_SEQ_CONTINUOUS, NULL, NULL, NULL)) != HIS_ALL_OK) {
      acquisitionError(__FILE__, __LINE__, nRet);
      return;
    }

    if (qcepDebug(DEBUG_PERKINELMER)) {
      printMessage("Acquire");
    }
  }
}

void QxrdDetectorPerkinElmer::beginAcquisition()
{
  if (m_StartupDelayed) {
    if (qcepDebug(DEBUG_PERKINELMER)) {
      printMessage("Delayed Acquisition Startup");
    }

    startupAcquisition();
    m_StartupDelayed = 0;
  }

  m_Counter.fetchAndStoreOrdered(0);

  QxrdPerkinElmerPluginInterfacePtr plugin(m_PerkinElmer);

  if (plugin && m_AcqDesc) {
    if (qcepDebug(DEBUG_PERKINELMER)) {
      printMessage("Reset frame counter");
    }

    plugin -> Acquisition_ResetFrameCnt(m_AcqDesc);
  }
}

void QxrdDetectorPerkinElmer::endAcquisition()
{
}

void QxrdDetectorPerkinElmer::shutdownAcquisition()
{
  QxrdPerkinElmerPluginInterfacePtr plugin(m_PerkinElmer);

  if (plugin && m_AcqDesc) {
    plugin->Acquisition_Abort(m_AcqDesc);
  }
}

void QxrdDetectorPerkinElmer::onEndFrame(int counter, unsigned int n1, unsigned int n2)
{
  //  QTime tic;
  //  tic.start();

  if (checkPluginAvailable()) {
    QxrdPerkinElmerPluginInterfacePtr plugin(m_PerkinElmer);
    QxrdAcquisitionPtr acq(m_Acquisition);

    if (plugin && acq) {
      QxrdInt16ImageDataPtr image = QxrdAllocator::newInt16Image(acq->allocator(),
                                                                 QxrdAllocator::AllocateFromReserve,
                                                                 acq->get_NCols(), acq->get_NRows());

      //    printf("allocator took %d msec\n", tic.restart());

      if (qcepDebug(DEBUG_PERKINELMER)) {
        printMessage(tr("QxrdDetectorPerkinElmer::onEndFrame(%1,%2,%3)")
                     .arg(counter).arg(n1).arg(n2));
      }

      QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

      //  if (get_Cancelling()) {
      //    set_Cancelling(false);
      //    return /*true*/;
      //  }

      long npixels = acq->get_NRows()*acq->get_NCols();

      unsigned short* frame = m_Buffer.data() + m_BufferIndex*npixels;

      DWORD /*actualFrame = n1,*/ actSecFrame = n2;

      //  this->Acquisition_GetActFrame(m_AcqDesc, &actualFrame, &actSecFrame);
      //
      //  if (n1 != actualFrame || n2 != actSecFrame) {
      //    printf("Lost frame, %d[%d], %d[%d]\n", n1, (int) actualFrame, n2, (int) actSecFrame);
      //    return;
      //  }

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
      int counter1 = m_Counter.load();
#else
      int counter1 = m_Counter;
#endif

      if ((counter1 - counter) > 5) {
        printMessage(tr("%1 frames behind [%2,%3], skipping")
                     .arg(counter1-counter).arg(counter).arg(counter1));
        return;
      }

      if (((actSecFrame-1)%m_BufferSize) != m_BufferIndex) {
        if (qcepDebug(DEBUG_PERKINELMER)) {
          printMessage(tr("actSecFrame %1, m_BufferIndex %2")
                       .arg(actSecFrame).arg(m_BufferIndex));
        }
      }

      quint16* current = (image ? image->data() : NULL);
      quint32  cksum = 0;
      double   avg = 0;

      unsigned short *fp = frame;

      for (long i=0; i<npixels; i++) {
        unsigned short val = *fp++;
        cksum += val;
        avg += val;
      }

      if (current && frame) {
        ::memcpy(current, frame, npixels*sizeof(quint16));
      }

      //    printf("Image copy took %d msec\n", tic.restart());

      //  set_Average(avg/npixels);

      if (qcepDebug(DEBUG_PERKINELMER)) {
        printMessage(tr("Frame checksum 0x%1, avg %2\n")
                     .arg(cksum,8,16,QChar('0')).arg(avg/npixels));

        for (int f=0; f<m_BufferSize; f++) {
          unsigned short* fp = m_Buffer.data() + f*npixels;

          quint32  cksum = 0;
          double   avg = 0;

          for (long i=0; i<npixels; i++) {
            unsigned short val = *fp++;
            cksum += val;
            avg += val;
          }

          printMessage(tr("Frame %1 checksum 0x%2, avg %3\n")
                       .arg(f).arg(cksum,8,16,QChar('0')).arg(avg/npixels));
        }
      }

      m_BufferIndex = (m_BufferIndex+1)%m_BufferSize;

      //    acquiredFrameAvailable(image);

      acq->enqueueAcquiredFrame(image);
      //    INVOKE_CHECK(QMetaObject::invokeMethod(g_Acquisition, "acquiredFrameAvailable", Qt::QueuedConnection,
      //                                           Q_ARG(QxrdInt16ImageDataPtr, image), Q_ARG(int,counter)));

      //    printf("Invoke took %d msec\n", tic.restart());
    }
  }
}

void QxrdDetectorPerkinElmer::acquisitionInitError(int n)
{
  acquisitionError(__FILE__, __LINE__, n);

  criticalMessage("Detector Initialization Failed");
}

void QxrdDetectorPerkinElmer::acquisitionNSensorsError(int n)
{
  acquisitionError(__FILE__, __LINE__, n);

  criticalMessage("Detector Initialization Failed");
}

void QxrdDetectorPerkinElmer::setupExposureMenu(QDoubleSpinBox * /*cb*/, double /*initialExposure*/)
{
//  foreach(double t, m_ReadoutTimes) {
//    cb -> addItem(tr("%1").arg(t/1e6,5,'f'));
//  }
}

void QxrdDetectorPerkinElmer::setupCameraGainMenu(QComboBox *cb, int initialGain)
{
  if (qcepDebug(DEBUG_PERKINELMER)) {
    printMessage(tr("QxrdAcquisitionPerkinElmer::setupCameraGainMenu m_HeaderID == %1").arg(m_HeaderID));
  }

  bool b = cb -> blockSignals(true);

  if (m_HeaderID == 11) { /* AM type */
    for (int i=0; i<16; i++) {
      QString msg;

      if (i==0) {
        msg = "High: ";
      } else if (i==15) {
        msg = "Low: ";
      }

      double value = 0.1;
      if (i & 1) {
        value += 0.3;
      }

      if (i & 2) {
        value += 0.9;
      }

      if (i & 4) {
        value += 4.7;
      }

      if (i & 8) {
        value += 10.0;
      }

      msg += tr("%1 pF").arg(value);

      cb -> addItem(msg);
    }
  } else if (m_HeaderID >= 12) { /* AN type */
    cb -> addItem(tr("High: 0.25 pF"));
    cb -> addItem(tr("0.5 pF"));
    cb -> addItem(tr("1 pF"));
    cb -> addItem(tr("2 pF"));
    cb -> addItem(tr("4 pF"));
    cb -> addItem(tr("Low: 8 pF"));
  } else {
    cb -> addItem(tr("Gain not settable"));
  }

  cb -> setCurrentIndex(initialGain);

  cb -> blockSignals(b);
}

void QxrdDetectorPerkinElmer::setupCameraBinningModeMenu(QComboBox *cb, int initialBinning)
{
//  printf("QxrdAcquisitionPerkinElmer::setupCameraBinningModeMenu m_HeaderID == %d, m_CameraType == %d\n",
//         m_HeaderID, m_CameraType);
  bool b = cb -> blockSignals(true);

  if (m_HeaderID == 14) {
    if (m_CameraType == 1) {
      cb -> addItem(tr("No binning"));
      cb -> addItem(tr("2x2 binning"));
    } else {
      cb -> addItem(tr("No binning"));
      cb -> addItem(tr("2x2 binning"));
    }
  } else {
    cb -> addItem(tr("No binning"));
  }

  cb -> setCurrentIndex(initialBinning);

  cb -> blockSignals(b);
}

void QxrdDetectorPerkinElmer::onEndFrameCallback()
{
//  QTime tic;
  //  tic.start();
  if (checkPluginAvailable()) {
    QxrdPerkinElmerPluginInterfacePtr plugin(m_PerkinElmer);
    QxrdAcquisitionPtr acq(m_Acquisition);

    if (plugin && acq) {
      QxrdSynchronizedAcquisitionPtr sync(acq->synchronizedAcquisition());

      if (sync) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        sync->acquiredFrameAvailable(m_Counter.load());
#else
        sync->acquiredFrameAvailable(m_Counter);
#endif
      }

      //  printf("syncAcq->acquiredFrameAvailable took %d msec\n", tic.restart());

      DWORD actualFrame, actSecFrame;

      plugin->Acquisition_GetActFrame(m_AcqDesc, &actualFrame, &actSecFrame);
      plugin->Acquisition_GetLatestFrameHeader(m_AcqDesc, &m_HwHeaderInfo, &m_HwHeaderInfoEx);

      //    printf("m_PerkinElmer->Acquisition_GetActFrame took %d msec\n", tic.restart());

      int counter = m_Counter.fetchAndAddOrdered(1);

      //    printf("m_Counter.fetchAndAddOrdered took %d msec\n", tic.restart());

      //    INVOKE_CHECK(QMetaObject::invokeMethod(g_Acquisition, "onEndFrame", Qt::QueuedConnection,
      //                                           Q_ARG(int, counter),
      //                                           Q_ARG(unsigned int, actualFrame), Q_ARG(unsigned int, actSecFrame)));
      onEndFrame(counter, actualFrame, actSecFrame);
    }
  }
    //  printf("onEndFrameCallback took %d msec\n", tic.elapsed());
}

static void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc)
{
  //  INVOKE_CHECK(QMetaObject::invokeMethod(g_Acquisition, "onEndFrame", Qt::QueuedConnection));
  //  g_Detector->onEndFrameCallback();
  if (g_Application) {
    QxrdPerkinElmerPluginInterfacePtr plugin(g_Application->perkinElmerPlugin());

    if (hAcqDesc && plugin) {
      ACQDATATYPE object;

      plugin->Acquisition_GetAcqData(hAcqDesc, &object);

      QxrdDetectorPerkinElmer *detector = reinterpret_cast<QxrdDetectorPerkinElmer*>(object);

      if (detector) {
        detector->onEndFrameCallback();
      }
    }
  }
}

static void CALLBACK OnEndAcqCallback(HACQDESC /*hAcqDesc*/)
{
}

void QxrdDetectorPerkinElmer::dumpHeaderInfo()
{
  printMessage(tr("Detector Header Info"));
  printMessage(tr("HeaderID %1  PROMID %2  Frame %3  Msec %4   Usec %5  Status %6")
               .arg(m_HwHeaderInfo.dwHeaderID)
               .arg(m_HwHeaderInfo.dwPROMID)
               .arg(m_HwHeaderInfoEx.wFrameCnt)
               .arg(m_HwHeaderInfoEx.wRealInttime_milliSec)
               .arg(m_HwHeaderInfoEx.wRealInttime_microSec)
               .arg(m_HwHeaderInfoEx.wStatus));

  printMessage(tr("Cmd1 %1  Cmd2 %2  Cmd3 %3  Cmd4 %4")
               .arg(m_HwHeaderInfoEx.wCommand1)
               .arg(m_HwHeaderInfoEx.wCommand2)
               .arg(m_HwHeaderInfoEx.wCommand3)
               .arg(m_HwHeaderInfoEx.wCommand4));
}


