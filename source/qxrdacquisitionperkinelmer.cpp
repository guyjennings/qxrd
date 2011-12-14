//#ifdef HAVE_PERKIN_ELMER

#include "qxrdacquisitionperkinelmer.h"
//#include "qxrdacquisitionthread.h"
#include "qxrddataprocessor.h"
#include "qxrdapplication.h"
#include "qxrdimagedata.h"
#include "qxrdwindow.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdallocator.h"

#include <string.h>

//#ifdef Q_OS_UNIX
//#include "AcqLinuxTypes.h"
//#endif

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

#include <math.h>

#include "Acq.h"
#include <QThread>
#include <QDir>
#include <QFile>
#include <QTime>
#include <QComboBox>
//#include <QMutexLocker>
#include "qxrdmutexlocker.h"
#include <QMetaObject>
#include <QMetaProperty>
//#include "qxrdxislinterface.h"

static QxrdAcquisitionPerkinElmer * g_Acquisition = NULL;
static void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc);
static void CALLBACK OnEndAcqCallback(HACQDESC hAcqDesc);
static HACQDESC m_AcqDesc = NULL;

QxrdAcquisitionPerkinElmer::QxrdAcquisitionPerkinElmer(QxrdSettingsSaver *saver,
                                                       QxrdExperiment *doc,
                                                       QxrdDataProcessor *proc,
                                                       QxrdAllocator *allocator,
                                                       QSettings *settings,
                                                       QString section)
  : QxrdAcquisition(saver, doc, proc, allocator),
    m_Mutex(QMutex::Recursive),
    m_BufferSize(0),
    m_StartupDelayed(0),
    m_PROMID(-1),
    m_HeaderID(-1),
    m_CameraType(-1),
    m_CameraModel(""),
    m_CurrentGain(-1),
    m_SyncMode(HIS_SYNCMODE_INTERNAL_TIMER),
    m_Counter(0),
    m_PerkinElmer(0)
{
  if (qcepDebug(DEBUG_PERKINELMER)) {
    m_Experiment->printMessage("QxrdAcquisitionPerkinElmer::QxrdAcquisitionPerkinElmer()");
  }

  readSettings(settings, section);

  ::g_Acquisition = this;
}

QxrdAcquisitionPerkinElmer::~QxrdAcquisitionPerkinElmer()
{
  if (qcepDebug(DEBUG_PERKINELMER)) {
    m_Experiment->printMessage("QxrdAcquisitionPerkinElmer::~QxrdAcquisitionPerkinElmer()");
  }

  if (m_PerkinElmer && m_AcqDesc) {
    m_PerkinElmer->Acquisition_Abort(m_AcqDesc);

    m_PerkinElmer->Acquisition_CloseAll();
  }
}

bool QxrdAcquisitionPerkinElmer::checkPluginAvailable()
{
  if (m_PerkinElmer == NULL) {
    m_PerkinElmer = g_Application->perkinElmerPlugin();
  }

  if (m_PerkinElmer == NULL) {
    m_Experiment->criticalMessage("No perkin elmer plugin available");
  }

  return m_PerkinElmer;
}

void QxrdAcquisitionPerkinElmer::onExposureTimeChanged(double newTime)
{
  if (checkPluginAvailable()) {
    if (newTime*1e6 < m_ReadoutTimes.at(0)) {
      m_Experiment->printMessage(tr("Attempt to set exposure time less than minimum supported (%1 < %2)")
                        .arg(newTime).arg(m_ReadoutTimes.at(0)/1e6));

      newTime = m_ReadoutTimes.at(0)/1e6;
    }

    m_Experiment->printMessage(tr("Exposure time changed to %1").arg(newTime));

    DWORD tmp = (int)(newTime*1e6);

    m_Experiment->printMessage(tr("SetTimerSync %1").arg(tmp));

    int nRet;

    if ((nRet=m_PerkinElmer->Acquisition_SetTimerSync(m_AcqDesc, &tmp)) != HIS_ALL_OK) {
      acquisitionError(__FILE__, __LINE__, nRet);
      return;
    }

    m_Experiment->printMessage(tr("TimerSync = %1").arg(tmp));

    set_ExposureTime(tmp/1.0e6);
  }
}

void QxrdAcquisitionPerkinElmer::onBinningModeChanged(int newMode)
{
  return;

  if (checkPluginAvailable()) {

    if (m_HeaderID == 14) {
      m_Experiment->printMessage(tr("Binning mode changed to %1").arg(newMode));

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
      if ((nRet=m_PerkinElmer->Acquisition_GetCameraBinningMode(m_AcqDesc, &originalMode)) != HIS_ALL_OK) {
        acquisitionError(__FILE__, __LINE__, nRet);
        return;
      }

      m_Experiment->printMessage(tr("Starting binning mode = %1").arg(originalMode));

      m_Experiment->printMessage(tr("Setting binning mode = %1").arg(newMode));

      if ((nRet=m_PerkinElmer->Acquisition_SetCameraBinningMode(m_AcqDesc, newMode)) != HIS_ALL_OK) {
        acquisitionError(__FILE__, __LINE__, nRet);
        return;
      }

      if ((nRet=m_PerkinElmer->Acquisition_GetCameraBinningMode(m_AcqDesc, &binningMode)) != HIS_ALL_OK) {
        acquisitionError(__FILE__, __LINE__, nRet);
        return;
      }

      m_Experiment->printMessage(tr("Binning mode was set to %1").arg(binningMode));
    }
  }
}

void QxrdAcquisitionPerkinElmer::onCameraGainChanged(int newGain)
{
  if (checkPluginAvailable()) {

    if (m_HeaderID >= 11) {
      m_Experiment->printMessage(tr("Camera Gain Changed to %1").arg(newGain));

      m_Experiment->printMessage("Setting camera gain");

      int nRet;

      if (m_CurrentGain != get_CameraGain()) {
        if ((nRet=m_PerkinElmer->Acquisition_SetCameraGain(m_AcqDesc, get_CameraGain())) != HIS_ALL_OK) {
          acquisitionError(__FILE__, __LINE__, nRet);
          return;
        }
        m_CurrentGain = get_CameraGain();
      }

      m_Experiment->printMessage("Set camera gain");
    }
  }
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

void QxrdAcquisitionPerkinElmer::initialize()
{
  if (qcepDebug(DEBUG_PERKINELMER)) {
    m_Experiment->printMessage(tr("QxrdAcquisitionPerkinElmer::initialize"));
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

    nRet = m_PerkinElmer->Acquisition_EnumSensors(&nSensors, bEnableIRQ, FALSE);

    if (qcepDebug(DEBUG_PERKINELMER)) {
      m_Experiment->printMessage(tr("Acquisition_EnumSensors = %1").arg(nRet));
    }

    if (nRet != HIS_ALL_OK) {
      acquisitionInitError(nRet);
      return;
    }

    if (qcepDebug(DEBUG_PERKINELMER)) {
      m_Experiment->printMessage(tr("Number of sensors = %1").arg(nSensors));
    }

    if (nSensors != 1) {
      acquisitionNSensorsError(nRet);
      return;
    }

    if ((nRet = m_PerkinElmer->Acquisition_GetNextSensor(&Pos, &m_AcqDesc))!=HIS_ALL_OK) {
      acquisitionNSensorsError(nRet);
      return;
    }

    if ((nRet=m_PerkinElmer->Acquisition_GetCommChannel(m_AcqDesc, &nChannelType, &nChannelNr))!=HIS_ALL_OK) {
      acquisitionError(__FILE__, __LINE__, nRet);
      return;
    }

    if (qcepDebug(DEBUG_PERKINELMER)) {
      m_Experiment->printMessage(tr("Acquisition_GetCommChannel channel type = %1, channel no = %2")
                        .arg(nChannelType).arg(nChannelNr));
    }

    if ((nRet=m_PerkinElmer->Acquisition_GetConfiguration(m_AcqDesc, &dwFrames, &dwRows, &dwColumns, &dwDataType,
                                           &dwSortFlags, &bEnableIRQ, &dwAcqType,
                                           &dwSystemID, &dwSyncMode, &dwHwAccess))!=HIS_ALL_OK) {
      acquisitionError(__FILE__, __LINE__, nRet);
      return;
    }

    if (qcepDebug(DEBUG_PERKINELMER)) {
      m_Experiment->printMessage(tr("Acquisition_GetConfiguration frames = %1, rows = %2, cols = %3")
                        .arg(dwFrames).arg(dwRows).arg(dwColumns));
      m_Experiment->printMessage(tr("Acquisition_GetConfiguration data type = %1, sort flags = %2, IRQ = %3")
                        .arg(dwDataType).arg(dwSortFlags).arg(bEnableIRQ));
      m_Experiment->printMessage(tr("Acquisition_GetConfiguration acq type = %1, systemID = %2, syncMode = %3, hwAccess = %4")
                        .arg(dwAcqType).arg(dwSystemID).arg(dwSyncMode).arg(dwHwAccess));
    }

    set_NRows(dwRows);
    set_NCols(dwColumns);

    if ((nRet=m_PerkinElmer->Acquisition_GetCameraBinningMode(m_AcqDesc, &binningMode)) != HIS_ALL_OK) {
      acquisitionError(__FILE__, __LINE__, nRet);
      return;
    }

    if (qcepDebug(DEBUG_PERKINELMER)) {
      m_Experiment->printMessage(tr("Acquisition_GetCameraBinningMode mode = %1").arg(binningMode));
    }

    QxrdAcquisition::initialize();

    CHwHeaderInfo hwHeaderInfo;

    if ((nRet=m_PerkinElmer->Acquisition_GetHwHeaderInfo(m_AcqDesc, &hwHeaderInfo)) != HIS_ALL_OK) {
      acquisitionError(__FILE__, __LINE__, nRet);
      return;
    }

    if (qcepDebug(DEBUG_PERKINELMER)) {
      m_Experiment->printMessage(tr("Prom ID %1, Header ID %2").arg(hwHeaderInfo.dwPROMID).arg(hwHeaderInfo.dwHeaderID));
    }

    m_PROMID = hwHeaderInfo.dwPROMID;
    m_HeaderID = hwHeaderInfo.dwHeaderID;

    if (hwHeaderInfo.dwHeaderID >= 14) {
      CHwHeaderInfoEx hdrx;

      if ((nRet = m_PerkinElmer->Acquisition_GetHwHeaderInfoEx(m_AcqDesc, &hwHeaderInfo, &hdrx)) != HIS_ALL_OK) {
        acquisitionError(__FILE__, __LINE__, nRet);
        return;
      }

      if (qcepDebug(DEBUG_PERKINELMER)) {
        m_Experiment->printMessage(tr("Camera Type %1").arg(hdrx.wCameratype));
      }

      m_CameraType = hdrx.wCameratype;
    }

    int nReadoutTimes = 8;
    double readoutTimes[8];

    if ((nRet=m_PerkinElmer->Acquisition_GetIntTimes(m_AcqDesc, readoutTimes, &nReadoutTimes)) != HIS_ALL_OK) {
      acquisitionError(__FILE__, __LINE__, nRet);
      return;
    }

    for (int i=0; i<nReadoutTimes; i++) {
      m_ReadoutTimes.append(readoutTimes[i]);
    }

    if (qcepDebug(DEBUG_PERKINELMER)) {
      m_Experiment->printMessage(tr("%1 predefined exposure times available").arg(m_ReadoutTimes.count()));

      for (int i=0; i<nReadoutTimes; i++) {
        m_Experiment->printMessage(tr("Exp %1 = %2").arg(i).arg(m_ReadoutTimes[i]));
      }
    }

    if ((nRet=m_PerkinElmer->Acquisition_SetCallbacksAndMessages(m_AcqDesc, NULL, 0,
                                                  0, OnEndFrameCallback, OnEndAcqCallback))!=HIS_ALL_OK) {
      acquisitionError(__FILE__, __LINE__, nRet);
      return;
    }

    m_BufferSize = 4;
    m_BufferIndex = 0;

    m_Buffer.resize(get_NRows()*get_NCols()*m_BufferSize);
    m_Buffer.fill(0);

    if (qcepDebug(DEBUG_PERKINELMER)) {
      m_Experiment->printMessage(tr("Exposure Time = %1").arg(get_ExposureTime()));
      m_Experiment->printMessage(tr("SetFrameSyncMode HIS_SYNCMODE_INTERNAL_TIMER"));
    }

    if ((nRet=m_PerkinElmer->Acquisition_SetFrameSyncMode(m_AcqDesc, HIS_SYNCMODE_INTERNAL_TIMER)) != HIS_ALL_OK) {
      if ((nRet=m_PerkinElmer->Acquisition_SetFrameSyncMode(m_AcqDesc, HIS_SYNCMODE_FREE_RUNNING)) != HIS_ALL_OK) {
        acquisitionError(__FILE__, __LINE__, nRet);
        return;
      } else {
        m_SyncMode = HIS_SYNCMODE_FREE_RUNNING;
      }
    } else {
      m_SyncMode = HIS_SYNCMODE_INTERNAL_TIMER;
    }

    if (qcepDebug(DEBUG_PERKINELMER)) {
      m_Experiment->printMessage(tr("Sync Mode = %1").arg(m_SyncMode));
    }

    onCameraGainChanged(get_CameraGain());

    if (get_ExposureTime() <= 0) {
      set_ExposureTime(0.135);
    }

    onExposureTimeChanged(get_ExposureTime());

    set_BinningMode(0);

    if ((nRet=m_PerkinElmer->Acquisition_DefineDestBuffers(m_AcqDesc, m_Buffer.data(), m_BufferSize,
                                            get_NRows(), get_NCols())) != HIS_ALL_OK) {
      acquisitionError(__FILE__, __LINE__, nRet);
      return;
    }

    if (qcepDebug(DEBUG_PERKINELMER)) {
      m_Experiment->printMessage(tr("Define Dest Buffers"));
    }

    if (qcepDebug(DEBUG_DELAY_ACQ)) {
      m_StartupDelayed = 1;
    } else {
      startupAcquisition();
    }
  }
}

void QxrdAcquisitionPerkinElmer::startupAcquisition()
{
  if (checkPluginAvailable()) {
    int nRet;

    if ((nRet=m_PerkinElmer->Acquisition_Acquire_Image(m_AcqDesc, m_BufferSize,
                                        0, HIS_SEQ_CONTINUOUS, NULL, NULL, NULL)) != HIS_ALL_OK) {
      acquisitionError(__FILE__, __LINE__, nRet);
      return;
    }

    if (qcepDebug(DEBUG_PERKINELMER)) {
      m_Experiment->printMessage("Acquire");
    }
  }
}

void QxrdAcquisitionPerkinElmer::beginAcquisition()
{
  if (m_StartupDelayed) {
    if (qcepDebug(DEBUG_PERKINELMER)) {
      m_Experiment->printMessage("Delayed Acquisition Startup");
    }

    startupAcquisition();
    m_StartupDelayed = 0;
  }

  m_Counter.fetchAndStoreOrdered(0);

  if (m_PerkinElmer && m_AcqDesc) {
    if (qcepDebug(DEBUG_PERKINELMER)) {
      m_Experiment->printMessage("Reset frame counter");
    }

    m_PerkinElmer -> Acquisition_ResetFrameCnt(m_AcqDesc);
  }
}

void QxrdAcquisitionPerkinElmer::endAcquisition()
{
}

void QxrdAcquisitionPerkinElmer::shutdownAcquisition()
{
  if (m_PerkinElmer && m_AcqDesc) {
    m_PerkinElmer->Acquisition_Abort(m_AcqDesc);
  }
}

void QxrdAcquisitionPerkinElmer::onEndFrame(int counter, unsigned int n1, unsigned int n2)
{
//  QTime tic;
//  tic.start();

  if (checkPluginAvailable()) {
    QxrdInt16ImageDataPtr image = m_Allocator->newInt16Image(QxrdAllocator::AllocateFromReserve,
                                                             get_NCols(), get_NRows());

//    printf("allocator took %d msec\n", tic.restart());

    if (qcepDebug(DEBUG_PERKINELMER)) {
      m_Experiment->printMessage(tr("QxrdAcquisitionPerkinElmer::onEndFrame(%1,%2,%3)")
                        .arg(counter).arg(n1).arg(n2));
    }

    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  //  if (get_Cancelling()) {
  //    set_Cancelling(false);
  //    return /*true*/;
  //  }

    long npixels = get_NRows()*get_NCols();

    unsigned short* frame = m_Buffer.data() + m_BufferIndex*npixels;

    DWORD actualFrame = n1, actSecFrame = n2;

  //  this->Acquisition_GetActFrame(m_AcqDesc, &actualFrame, &actSecFrame);
  //
  //  if (n1 != actualFrame || n2 != actSecFrame) {
  //    printf("Lost frame, %d[%d], %d[%d]\n", n1, (int) actualFrame, n2, (int) actSecFrame);
  //    return;
  //  }

    int counter1 = m_Counter;

    if ((counter1 - counter) > 5) {
      m_Experiment->printMessage(tr("%1 frames behind [%2,%3], skipping")
			.arg(counter1-counter).arg(counter).arg(counter1));
      return;
    }

    if (((actSecFrame-1)%m_BufferSize) != m_BufferIndex) {
      if (qcepDebug(DEBUG_PERKINELMER)) {
        m_Experiment->printMessage(tr("actSecFrame %1, m_BufferIndex %2")
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
      m_Experiment->printMessage(tr("Frame checksum 0x%1, avg %2\n")
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

        m_Experiment->printMessage(tr("Frame %1 checksum 0x%2, avg %3\n")
                          .arg(f).arg(cksum,8,16,QChar('0')).arg(avg/npixels));
      }
    }

    m_BufferIndex = (m_BufferIndex+1)%m_BufferSize;

//    acquiredFrameAvailable(image);

    enqueueAcquiredFrame(image);
//    INVOKE_CHECK(QMetaObject::invokeMethod(g_Acquisition, "acquiredFrameAvailable", Qt::QueuedConnection,
//                                           Q_ARG(QxrdInt16ImageDataPtr, image), Q_ARG(int,counter)));

//    printf("Invoke took %d msec\n", tic.restart());
  }
}

void QxrdAcquisitionPerkinElmer::acquisitionInitError(int n)
{
  acquisitionError(__FILE__, __LINE__, n);

  m_Experiment->criticalMessage("Detector Initialization Failed");
}

void QxrdAcquisitionPerkinElmer::acquisitionNSensorsError(int n)
{
  acquisitionError(__FILE__, __LINE__, n);

  m_Experiment->criticalMessage("Detector Initialization Failed");
}

void QxrdAcquisitionPerkinElmer::setupExposureMenu(QDoubleSpinBox *cb)
{
//  foreach(double t, m_ReadoutTimes) {
//    cb -> addItem(tr("%1").arg(t/1e6,5,'f'));
//  }
}

void QxrdAcquisitionPerkinElmer::setupCameraGainMenu(QComboBox *cb)
{
  if (qcepDebug(DEBUG_PERKINELMER)) {
    m_Experiment->printMessage(
          tr("QxrdAcquisitionPerkinElmer::setupCameraGainMenu m_HeaderID == %1").arg(m_HeaderID));
  }

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
}

void QxrdAcquisitionPerkinElmer::setupCameraBinningModeMenu(QComboBox *cb)
{
//  printf("QxrdAcquisitionPerkinElmer::setupCameraBinningModeMenu m_HeaderID == %d, m_CameraType == %d\n",
//         m_HeaderID, m_CameraType);

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
}

void QxrdAcquisitionPerkinElmer::onEndFrameCallback()
{
//  QTime tic;
//  tic.start();

  if (synchronizedAcquisition()) {
    synchronizedAcquisition()->acquiredFrameAvailable(m_Counter);
  }

//  printf("syncAcq->acquiredFrameAvailable took %d msec\n", tic.restart());

  if (checkPluginAvailable()) {

    DWORD actualFrame, actSecFrame;

    m_PerkinElmer->Acquisition_GetActFrame(m_AcqDesc, &actualFrame, &actSecFrame);
    m_PerkinElmer->Acquisition_GetLatestFrameHeader(m_AcqDesc, &m_HwHeaderInfo, &m_HwHeaderInfoEx);

//    printf("m_PerkinElmer->Acquisition_GetActFrame took %d msec\n", tic.restart());

    int counter = m_Counter.fetchAndAddOrdered(1);

//    printf("m_Counter.fetchAndAddOrdered took %d msec\n", tic.restart());

    //    INVOKE_CHECK(QMetaObject::invokeMethod(g_Acquisition, "onEndFrame", Qt::QueuedConnection,
//                                           Q_ARG(int, counter),
//                                           Q_ARG(unsigned int, actualFrame), Q_ARG(unsigned int, actSecFrame)));
    onEndFrame(counter, actualFrame, actSecFrame);
  }

//  printf("onEndFrameCallback took %d msec\n", tic.elapsed());
}

static void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc)
{
//  INVOKE_CHECK(QMetaObject::invokeMethod(g_Acquisition, "onEndFrame", Qt::QueuedConnection));
  g_Acquisition->onEndFrameCallback();
}

static void CALLBACK OnEndAcqCallback(HACQDESC /*hAcqDesc*/)
{
}

void QxrdAcquisitionPerkinElmer::dumpHeaderInfo()
{
  m_Experiment->printMessage(tr("Detector Header Info"));
  m_Experiment->printMessage(tr("HeaderID %1  PROMID %2  Frame %3  Msec %4   Usec %5  Status %6")
                              .arg(m_HwHeaderInfo.dwHeaderID)
                              .arg(m_HwHeaderInfo.dwPROMID)
                              .arg(m_HwHeaderInfoEx.wFrameCnt)
                              .arg(m_HwHeaderInfoEx.wRealInttime_milliSec)
                              .arg(m_HwHeaderInfoEx.wRealInttime_microSec)
                              .arg(m_HwHeaderInfoEx.wStatus));

  m_Experiment->printMessage(tr("Cmd1 %1  Cmd2 %2  Cmd3 %3  Cmd4 %4")
                              .arg(m_HwHeaderInfoEx.wCommand1)
                              .arg(m_HwHeaderInfoEx.wCommand2)
                              .arg(m_HwHeaderInfoEx.wCommand3)
                              .arg(m_HwHeaderInfoEx.wCommand4));
}

//#endif // HAVE_PERKIN_ELMER
