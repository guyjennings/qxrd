#include "qxrddebug.h"
#include "qxrddetectordriverperkinelmer.h"
#include "qxrddetectorsettingsperkinelmer.h"
#include "qxrdacquisition.h"
#include "qxrdapplication.h"
#include "qxrdexperiment.h"
#include "qcepallocator.h"
#include "qcepmutexlocker.h"

#ifdef HAVE_PERKIN_ELMER
#include "qxrdperkinelmerplugininterface.h"
//static QxrdDetectorPerkinElmer *g_Detector = NULL;
static void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc);
static void CALLBACK OnEndAcqCallback(HACQDESC hAcqDesc);
#endif

QxrdDetectorDriverPerkinElmer::QxrdDetectorDriverPerkinElmer(
    QString name,
    QxrdDetectorSettingsPerkinElmerPtr perkinElmer,
    QxrdExperimentPtr expt,
    QxrdAcquisitionPtr acq) :
  QxrdDetectorDriver(name, perkinElmer, expt, acq),
#ifdef HAVE_PERKIN_ELMER
  m_Mutex(QMutex::Recursive),
  m_BufferSize(0),
  m_AcqDesc(NULL),
  m_StartupDelayed(0),
  m_PROMID(-1),
  m_HeaderID(-1),
  m_CameraType(-1),
  m_CameraModel(""),
  m_CurrentGain(-1),
  m_SyncMode(HIS_SYNCMODE_INTERNAL_TIMER),
  m_Counter(0),
  m_PerkinElmerPlugin(),
#endif
  m_PerkinElmer(perkinElmer)
{
#ifndef QT_NO_DEBUG
#ifdef HAVE_PERKIN_ELMER
  printf("Perkin Elmer Detector Driver \"%s\" Constructed\n", qPrintable(name));
#else
  printf("Perkin Elmer Detector Driver \"%s\" Constructed (Non-Implemented)\n", qPrintable(name));
#endif
#endif
}

QxrdDetectorDriverPerkinElmer::~QxrdDetectorDriverPerkinElmer()
{
#ifndef QT_NO_DEBUG
  printf("Perkin Elmer Detector Driver \"%s\" Destroyed\n", qPrintable(get_Name()));
#endif

#ifdef HAVE_PERKIN_ELMER
  QxrdPerkinElmerPluginInterfacePtr plugin(m_PerkinElmerPlugin);

  if (plugin && m_AcqDesc) {
    plugin->Acquisition_SetAcqData(m_AcqDesc, NULL);
    plugin->Acquisition_SetCallbacksAndMessages(m_AcqDesc, NULL, 0, 0, NULL, NULL);
    plugin->Acquisition_Abort(m_AcqDesc);
    plugin->Acquisition_CloseAll();
  }
#endif
}

QString QxrdDetectorDriverPerkinElmer::acquisitionErrorString(int n)
{
  QString res = "No Error";

#ifdef HAVE_PERKIN_ELMER
  switch (n) {
  case HIS_ALL_OK:
    res = "HIS_ALL_OK";
    break;

  case HIS_ERROR_MEMORY:
    res = "HIS_ERROR_MEMORY";
    break;

  case HIS_ERROR_BOARDINIT:
    res = "HIS_ERROR_BOARDINIT";
    break;

  case HIS_ERROR_NOCAMERA:
    res = "HIS_ERROR_NOCAMERA";
    break;

  case HIS_ERROR_CORRBUFFER_INCOMPATIBLE:
    res = "HIS_ERROR_CORRBUFFER_INCOMPATIBLE";
    break;

  case HIS_ERROR_ACQ_ALREADY_RUNNING:
    res = "HIS_ERROR_ACQ_ALREADY_RUNNING";
    break;

  case HIS_ERROR_TIMEOUT:
    res = "HIS_ERROR_TIMEOUT";
    break;

  case HIS_ERROR_INVALIDACQDESC:
    res = "HIS_ERROR_INVALIDACQDESC";
    break;

  case HIS_ERROR_VXDNOTFOUND:
    res = "HIS_ERROR_VXDNOTFOUND";
    break;

  case HIS_ERROR_VXDNOTOPEN:
    res = "HIS_ERROR_VXDNOTOPEN";
    break;

  case HIS_ERROR_VXDUNKNOWNERROR:
    res = "HIS_ERROR_VXDUNKNOWNERROR";
    break;

  case HIS_ERROR_VXDGETDMAADR:
    res = "HIS_ERROR_VXDGETDMAADR";
    break;

  case HIS_ERROR_ACQABORT:
    res = "HIS_ERROR_ACQABORT";
    break;

  case HIS_ERROR_ACQUISITION:
    res = "HIS_ERROR_ACQUISITION";
    break;

  case HIS_ERROR_VXD_REGISTER_IRQ:
    res = "HIS_ERROR_VXD_REGISTER_IRQ";
    break;

  case HIS_ERROR_VXD_REGISTER_STATADR:
    res = "HIS_ERROR_MEMORY";
    break;

  case HIS_ERROR_GETOSVERSION:
    res = "HIS_ERROR_GETOSVERSION";
    break;

  case HIS_ERROR_SETFRMSYNC:
    res = "HIS_ERROR_SETFRMSYNC";
    break;

  case HIS_ERROR_SETFRMSYNCMODE:
    res = "HIS_ERROR_SETFRMSYNCMODE";
    break;

  case HIS_ERROR_SETTIMERSYNC:
    res = "HIS_ERROR_SETTIMERSYNC";
    break;

  case HIS_ERROR_INVALID_FUNC_CALL:
    res = "HIS_ERROR_INVALID_FUNC_CALL";
    break;

  case HIS_ERROR_ABORTCURRFRAME:
    res = "HIS_ERROR_ABORTCURRFRAME";
    break;

  case HIS_ERROR_GETHWHEADERINFO:
    res = "HIS_ERROR_GETHWHEADERINFO";
    break;

  case HIS_ERROR_HWHEADER_INV:
    res = "HIS_ERROR_HWHEADER_INV";
    break;

  case HIS_ERROR_SETLINETRIG_MODE:
    res = "HIS_ERROR_SETLINETRIG_MODE";
    break;

  case HIS_ERROR_WRITE_DATA:
    res = "HIS_ERROR_WRITE_DATA";
    break;

  case HIS_ERROR_READ_DATA:
    res = "HIS_ERROR_READ_DATA";
    break;

  case HIS_ERROR_SETBAUDRATE:
    res = "HIS_ERROR_SETBAUDRATE";
    break;

  case HIS_ERROR_NODESC_AVAILABLE:
    res = "HIS_ERROR_NODESC_AVAILABLE";
    break;

  case HIS_ERROR_BUFFERSPACE_NOT_SUFF:
    res = "HIS_ERROR_BUFFERSPACE_NOT_SUFF";
    break;

  case HIS_ERROR_SETCAMERAMODE:
    res = "HIS_ERROR_SETCAMERAMODE";
    break;

  case HIS_ERROR_FRAME_INV:
    res = "HIS_ERROR_FRAME_INV";
    break;

  case HIS_ERROR_SLOW_SYSTEM:
    res = "HIS_ERROR_SLOW_SYSTEM";
    break;

  case HIS_ERROR_GET_NUM_BOARDS:
    res = "HIS_ERROR_GET_NUM_BOARDS";
    break;

  case HIS_ERROR_HW_ALREADY_OPEN_BY_ANOTHER_PROCESS:
    res = "HIS_ERROR_HW_ALREADY_OPEN_BY_ANOTHER_PROCESS";
    break;

  case HIS_ERROR_CREATE_MEMORYMAPPING:
    res = "HIS_ERROR_CREATE_MEMORYMAPPING";
    break;

  case HIS_ERROR_VXD_REGISTER_DMA_ADDRESS:
    res = "HIS_ERROR_VXD_REGISTER_DMA_ADDRESS";
    break;

  case HIS_ERROR_VXD_REGISTER_STAT_ADDR:
    res = "HIS_ERROR_VXD_REGISTER_STAT_ADDR";
    break;

  case HIS_ERROR_VXD_UNMASK_IRQ:
    res = "HIS_ERROR_VXD_UNMASK_IRQ";
    break;

  case HIS_ERROR_LOADDRIVER:
    res = "HIS_ERROR_LOADDRIVER";
    break;

  case HIS_ERROR_FUNC_NOTIMPL:
    res = "HIS_ERROR_FUNC_NOTIMPL";
    break;

  case HIS_ERROR_MEMORY_MAPPING:
    res = "HIS_ERROR_MEMORY_MAPPING";
    break;

  case HIS_ERROR_CREATE_MUTEX:
    res = "HIS_ERROR_CREATE_MUTEX";
    break;

  case HIS_ERROR_ACQ:
    res = "HIS_ERROR_ACQ";
    break;

  case HIS_ERROR_DESC_NOT_LOCAL:
    res = "HIS_ERROR_DESC_NOT_LOCAL";
    break;

  case HIS_ERROR_INVALID_PARAM:
    res = "HIS_ERROR_INVALID_PARAM";
    break;

  case HIS_ERROR_ABORT:
    res = "HIS_ERROR_ABORT";
    break;

  case HIS_ERROR_WRONGBOARDSELECT:
    res = "HIS_ERROR_WRONGBOARDSELECT";
    break;

  case HIS_ERROR_WRONG_CAMERA_MODE:
    res = "HIS_ERROR_WRONG_CAMERA_MODE";
    break;

  case HIS_ERROR_AVERAGED_LOST:
    res = "HIS_ERROR_AVERAGED_LOST";
    break;

  case HIS_ERROR_BAD_SORTING_PARAM:
    res = "HIS_ERROR_BAD_SORTING_PARAM";
    break;

  case HIS_ERROR_UNKNOWN_IP_MAC_NAME:
    res = "HIS_ERROR_UNKNOWN_IP_MAC_NAME";
    break;

  case HIS_ERROR_NO_BOARD_IN_SUBNET:
    res = "HIS_ERROR_NO_BOARD_IN_SUBNET";
    break;

  case HIS_ERROR_UNABLE_TO_OPEN_BOARD:
    res = "HIS_ERROR_UNABLE_TO_OPEN_BOARD";
    break;

  case HIS_ERROR_UNABLE_TO_CLOSE_BOARD:
    res = "HIS_ERROR_UNABLE_TO_CLOSE_BOARD";
    break;

  case HIS_ERROR_UNABLE_TO_ACCESS_DETECTOR_FLASH:
    res = "HIS_ERROR_UNABLE_TO_ACCESS_DETECTOR_FLASH";
    break;

  case HIS_ERROR_HEADER_TIMEOUT:
    res = "HIS_ERROR_HEADER_TIMEOUT";
    break;

  case HIS_ERROR_NO_PING_ACK:
    res = "HIS_ERROR_NO_PING_ACK";
    break;

  case HIS_ERROR_NR_OF_BOARDS_CHANGED:
    res = "HIS_ERROR_NR_OF_BOARDS_CHANGED";
    break;
  }
#endif

  return res;
}

void QxrdDetectorDriverPerkinElmer::acquisitionError(const char *fn, int ln, int n)
{
  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    acq -> cancel();
  }

  printMessage(tr("Acquisition Error %1 [%2] at line %3 in file %4")
               .arg(n).arg(acquisitionErrorString(n)).arg(ln).arg(fn));

  criticalMessage(tr("Acquisition Error %1 [%2] at line %3 in file %4")
                  .arg(n).arg(acquisitionErrorString(n)).arg(ln).arg(fn));
}

bool QxrdDetectorDriverPerkinElmer::checkPluginAvailable()
{
#ifdef HAVE_PERKIN_ELMER
  QxrdApplication *app = qobject_cast<QxrdApplication*>(g_Application);

  if (app && m_PerkinElmerPlugin == NULL) {
    m_PerkinElmerPlugin = app->perkinElmerPlugin();
  }

  if (m_PerkinElmerPlugin == NULL) {
    criticalMessage("No perkin elmer plugin available");
  }

  return m_PerkinElmerPlugin;
#else
  return false;
#endif
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

bool QxrdDetectorDriverPerkinElmer::startDetectorDriver()
{
  THREAD_CHECK;

  if (qcepDebug(DEBUG_PERKINELMER)) {
    printMessage(tr("QxrdAcquisitionPerkinElmer::start"));
  }

  //    QxrdDetectorSettings::startDetector();

#ifdef HAVE_PERKIN_ELMER
  QxrdDetectorSettingsPerkinElmerPtr det(m_PerkinElmer);

  if (det && det -> checkDetectorEnabled() && checkPluginAvailable()) {
    int nRet = HIS_ALL_OK;
    UINT nSensors;
    BOOL bEnableIRQ = true;
    BOOL bSelfInit = true;
    BOOL bAlwaysOpen = true;
    ACQDESCPOS Pos = 0;
    UINT nChannelType;
    int nChannelNr;
    UINT dwSortFlags, dwDataType, dwRows, dwColumns, dwFrames;
    DWORD dwAcqType, dwSystemID, dwSyncMode, dwHwAccess;
    WORD binningMode;

    QxrdExperimentPtr exp(m_Experiment);

    //    if (exp) {
    //      m_DetectorNumber = exp->get_DetectorNumber();
    //      m_SubType        = exp->get_DetectorSubType();
    //      m_Address        = exp->get_DetectorAddress();
    //    }

    QxrdPerkinElmerPluginInterfacePtr plugin(m_PerkinElmerPlugin);

    int subType = det -> get_DetectorSubType();

    if (subType == QxrdDetectorSettingsPerkinElmer::PCI_SubType) {
      printMessage("Initialising PCI/PCIe Perkin Elmer Detector");

      if (plugin) {
        nRet = plugin->Acquisition_EnumSensors(&nSensors, bEnableIRQ, FALSE);
      }

      if (qcepDebug(DEBUG_PERKINELMER)) {
        printMessage(tr("Acquisition_EnumSensors = %1").arg(nRet));
      }

      if (nRet != HIS_ALL_OK) {
        acquisitionInitError(__FILE__, __LINE__, nRet);
        return false;
      }

      if (qcepDebug(DEBUG_PERKINELMER)) {
        printMessage(tr("Number of sensors = %1").arg(nSensors));
      }

      if (det -> get_DetectorNumber() == 0 && nSensors != 1) {
        acquisitionNSensorsError(__FILE__, __LINE__, nRet);
        return false;
      } else if (det -> get_DetectorNumber() < 0 || det -> get_DetectorNumber() > nSensors) {
        acquisitionNSensorsError(__FILE__, __LINE__, nRet);
        return false;
      }

      for (int i=1; i<=(det -> get_DetectorNumber() ? det -> get_DetectorNumber() : 1); i++) {
        if (plugin && (nRet = plugin->Acquisition_GetNextSensor(&Pos, &m_AcqDesc))!=HIS_ALL_OK) {
          acquisitionNSensorsError(__FILE__, __LINE__, nRet);
          return false;
        }
      }
    } else if (subType == QxrdDetectorSettingsPerkinElmer::GBIF_IP_SubType) {
      printMessage(tr("Attempting to connect to Perkin Elmer detector on the network at IP Address %1")
                   .arg(det -> get_DetectorAddress()));

      if (plugin && (nRet = plugin->Acquisition_GbIF_Init(&m_AcqDesc, 0, bEnableIRQ,
                                                          1024, 1024, bSelfInit,
                                                          bAlwaysOpen, 1,
                                                          (GBIF_STRING_DATATYPE*)
                                                          qPrintable(det -> get_DetectorAddress()))) != HIS_ALL_OK) {
        acquisitionInitError(__FILE__, __LINE__, nRet);
        return false;
      }
    } else if (subType == QxrdDetectorSettingsPerkinElmer::GBIF_MAC_SubType) {
      printMessage(tr("Attempting to connect to Perkin Elmer detector on the network at MAC address %1").arg(det -> get_DetectorAddress()));

      if (plugin && (nRet = plugin->Acquisition_GbIF_Init(&m_AcqDesc, 0, bEnableIRQ,
                                                          1024, 1024, bSelfInit,
                                                          bAlwaysOpen, 2,
                                                          (GBIF_STRING_DATATYPE*)
                                                          qPrintable(det -> get_DetectorAddress()))) != HIS_ALL_OK) {
        acquisitionInitError(__FILE__, __LINE__, nRet);
        return false;
      }
    } else if (subType == QxrdDetectorSettingsPerkinElmer::GBIF_Name_SubType) {
      printMessage(tr("Attempting to connect to Perkin Elmer detector on the network at device name %1").arg(det -> get_DetectorAddress()));

      if (plugin && (nRet = plugin->Acquisition_GbIF_Init(&m_AcqDesc, 0, bEnableIRQ,
                                                          1024, 1024, bSelfInit,
                                                          bAlwaysOpen, 3,
                                                          (GBIF_STRING_DATATYPE*)
                                                          qPrintable(det -> get_DetectorAddress()))) != HIS_ALL_OK) {
        acquisitionInitError(__FILE__, __LINE__, nRet);
        return false;
      }
    } else if (subType == QxrdDetectorSettingsPerkinElmer::GBIF_Scan_SubType) {
      printMessage("Searching for Perkin Elmer Detectors on the network");

      long nBoards=0;

      if (plugin && (nRet = plugin->Acquisition_GbIF_GetDeviceCnt(&nBoards)) != HIS_ALL_OK) {
        acquisitionNSensorsError(__FILE__, __LINE__, nRet);
        return false;
      }

      QVector<GBIF_DEVICE_PARAM> devs(nBoards);

      if (plugin && (nRet = plugin->Acquisition_GbIF_GetDeviceList(devs.data(), nBoards))) {
        acquisitionNSensorsError(__FILE__, __LINE__, nRet);
        return false;
      }

      printMessage(tr("Found %1 Detectors").arg(nBoards));

      if (nBoards == 0) {
        criticalMessage("No detectors found");
        return false;
      }

      for (int i=0; i<nBoards; i++) {
        printMessage(tr("Device %1: %2 found at IP: %3, MAC: %4, Name: %5")
                     .arg(i+1)
                     .arg(devs[i].cModelName)
                     .arg((char*)(devs[i].ucIP))
                     .arg((char*)(devs[i].ucMacAddress))
                     .arg(devs[i].cDeviceName));
      }

      if (det -> get_DetectorNumber() >= 0 && det -> get_DetectorNumber() <= nBoards) {
        int n = (det -> get_DetectorNumber() ? det -> get_DetectorNumber() : 1);

        if (plugin && (nRet = plugin->Acquisition_GbIF_Init(&m_AcqDesc, 0, bEnableIRQ,
                                                            1024, 1024, bSelfInit,
                                                            bAlwaysOpen, 1,
                                                            devs[n-1].ucIP)) != HIS_ALL_OK) {
          acquisitionInitError(__FILE__, __LINE__, nRet);
          return false;
        }
      }
    } else {
      criticalMessage(tr("Unrecognized detector type %1").arg(subType));
      return false;
    }

    if (plugin && (nRet = plugin->Acquisition_GetCommChannel(m_AcqDesc, &nChannelType, &nChannelNr))!=HIS_ALL_OK) {
      acquisitionError(__FILE__, __LINE__, nRet);
      return false;
    }

    if (qcepDebug(DEBUG_PERKINELMER)) {
      printMessage(tr("Acquisition_GetCommChannel channel type = %1, channel no = %2")
                   .arg(nChannelType).arg(nChannelNr));
    }

    if (plugin && (nRet=plugin->Acquisition_GetConfiguration(m_AcqDesc, &dwFrames, &dwRows, &dwColumns, &dwDataType,
                                                             &dwSortFlags, &bEnableIRQ, &dwAcqType,
                                                             &dwSystemID, &dwSyncMode, &dwHwAccess))!=HIS_ALL_OK) {
      acquisitionError(__FILE__, __LINE__, nRet);
      return false;
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
      det -> set_NRows(dwRows);
      det -> set_NCols(dwColumns);
    }

    if (plugin && (nRet=plugin->Acquisition_GetCameraBinningMode(m_AcqDesc, &binningMode)) != HIS_ALL_OK) {
      acquisitionError(__FILE__, __LINE__, nRet);
      return false;
    }

    if (qcepDebug(DEBUG_PERKINELMER)) {
      printMessage(tr("Acquisition_GetCameraBinningMode mode = %1").arg(binningMode));
    }

    CHwHeaderInfo hwHeaderInfo;

    if (plugin && (nRet=plugin->Acquisition_GetHwHeaderInfo(m_AcqDesc, &hwHeaderInfo)) != HIS_ALL_OK) {
      acquisitionError(__FILE__, __LINE__, nRet);
      return false;
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
        return false;
      }

      if (qcepDebug(DEBUG_PERKINELMER)) {
        printMessage(tr("Camera Type %1").arg(hdrx.wCameratype));
        printMessage(tr("Binning Mode %1").arg(hdrx.wBinningMode));
      }

      m_CameraType = hdrx.wCameratype;
    }

    int nReadoutTimes = 8;
    double readoutTimes[8];

    if (plugin && (nRet = plugin->Acquisition_GetIntTimes(m_AcqDesc, readoutTimes, &nReadoutTimes)) != HIS_ALL_OK) {
      acquisitionError(__FILE__, __LINE__, nRet);
      return false;
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
      return false;
    }

    if (plugin && (nRet = plugin->Acquisition_SetCallbacksAndMessages(m_AcqDesc, NULL, 0,
                                                                      0, OnEndFrameCallback, OnEndAcqCallback))!=HIS_ALL_OK) {
      acquisitionError(__FILE__, __LINE__, nRet);
      return false;
    }

    m_BufferSize = 4;
    m_BufferIndex = 0;

    if (acq) {
      m_Buffer.resize(det -> get_NRows() * det -> get_NCols() * m_BufferSize);
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
        return false;
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
      onBinningModeChanged();

      onCameraGainChanged();

      if (acq->get_ExposureTime() <= 0) {
        acq->set_ExposureTime(0.135);
      }

      changeExposureTime(acq->get_ExposureTime());

      if (plugin && (nRet=plugin->Acquisition_DefineDestBuffers(m_AcqDesc, m_Buffer.data(), m_BufferSize,
                                                                det -> get_NRows(), det -> get_NCols())) != HIS_ALL_OK) {
        acquisitionError(__FILE__, __LINE__, nRet);
        return false;
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

  return true;
#else
  return false;
#endif
}

bool QxrdDetectorDriverPerkinElmer::stopDetectorDriver()
{
#ifdef HAVE_PERKIN_ELMER
  QxrdDetectorSettingsPerkinElmerPtr det(m_PerkinElmer);

  if (det) {
    printMessage(tr("Stopping Perkin Elmer Detector %1").arg(det -> get_DetectorName()));
  }
  return true;
#else
  return false;
#endif
}

bool QxrdDetectorDriverPerkinElmer::changeExposureTime(double expos)
{
#ifdef HAVE_PERKIN_ELMER
  QxrdDetectorSettingsPerkinElmerPtr det(m_PerkinElmer);

  if (det && det -> isEnabled() && checkPluginAvailable()) {
    QxrdPerkinElmerPluginInterfacePtr plugin(m_PerkinElmerPlugin);
    QxrdAcquisitionPtr acq(m_Acquisition);

    if (plugin && acq) {
//      double newTime = acq->get_ExposureTime();
//      if (newTime*1e6 < m_ReadoutTimes.value(0)) {
//        printMessage(tr("Attempt to set exposure time less than minimum supported (%1 < %2)")
//                     .arg(newTime).arg(m_ReadoutTimes.value(0)/1e6));

//        newTime = m_ReadoutTimes.value(0)/1e6;
//      }

      printMessage(tr("Exposure time changed to %1").arg(expos));

      DWORD tmp = (int)(expos*1e6);

      printMessage(tr("SetTimerSync %1").arg(tmp));

      int nRet;

      if ((nRet=plugin->Acquisition_SetTimerSync(m_AcqDesc, &tmp)) != HIS_ALL_OK) {
        acquisitionError(__FILE__, __LINE__, nRet);
        return false;
      }

      printMessage(tr("TimerSync = %1").arg(tmp));

//      acq->set_ExposureTime(tmp/1.0e6);
    }
  }
  return true;
#else
  return false;
#endif
}

void QxrdDetectorDriverPerkinElmer::onBinningModeChanged()
{
//  return;
#ifdef HAVE_PERKIN_ELMER
  QxrdDetectorSettingsPerkinElmerPtr det(m_PerkinElmer);

  if (det && det -> isEnabled() && checkPluginAvailable()) {
    QxrdPerkinElmerPluginInterfacePtr plugin(m_PerkinElmerPlugin);
    QxrdAcquisitionPtr acq(m_Acquisition);

    if (plugin && acq) {
      if (m_HeaderID == 14) {
        int newMode = 1;

        switch (det -> get_DetectorBinning()) {
        case QxrdDetectorSettingsPerkinElmer::Binning1x1:
          newMode = 1;
          break;

        case QxrdDetectorSettingsPerkinElmer::Binning2x2Average:
          newMode = 2 + 256;
          break;

        case QxrdDetectorSettingsPerkinElmer::Binning2x2Summed:
          newMode = 2 + 512;
          break;
        }

        printMessage(tr("Change binning mode to %1").arg(newMode));

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

        printMessage(tr("Initial binning mode = %1").arg(originalMode));

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

        if (binningMode <= 1) {
          det -> set_HBinning(1);
          det -> set_VBinning(1);
        } else {
          det -> set_HBinning(2);
          det -> set_VBinning(2);
        }
      }
    }
  }
#endif
}

void QxrdDetectorDriverPerkinElmer::onCameraGainChanged()
{
#ifdef HAVE_PERKIN_ELMER
  QxrdDetectorSettingsPerkinElmerPtr det(m_PerkinElmer);

  if (det && det -> isEnabled() && checkPluginAvailable()) {
    QxrdPerkinElmerPluginInterfacePtr plugin(m_PerkinElmerPlugin);
    QxrdAcquisitionPtr  acq(m_Acquisition);

    if (acq && plugin) {
      if (m_HeaderID >= 11) {
        int newGain = det -> get_DetectorGain();

        printMessage(tr("Camera Gain Changed to %1").arg(newGain));

        printMessage("Setting camera gain");

        int nRet;

        if (m_CurrentGain != det -> get_DetectorGain()) {
          if ((nRet=plugin->Acquisition_SetCameraGain(m_AcqDesc, det -> get_DetectorGain())) != HIS_ALL_OK) {
            acquisitionError(__FILE__, __LINE__, nRet);
            return;
          }
          m_CurrentGain = det -> get_DetectorGain();
        }

        printMessage("Set camera gain");
      }
    }
  }
#endif
}

void QxrdDetectorDriverPerkinElmer::startupAcquisition()
{
#ifdef HAVE_PERKIN_ELMER
  QxrdDetectorSettingsPerkinElmerPtr det(m_PerkinElmer);

  if (det && det -> checkDetectorEnabled() && checkPluginAvailable()) {
    QxrdPerkinElmerPluginInterfacePtr plugin(m_PerkinElmerPlugin);

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
#endif
}

bool QxrdDetectorDriverPerkinElmer::beginAcquisition(double exposure)
{
  THREAD_CHECK;

#ifdef HAVE_PERKIN_ELMER
  if (m_StartupDelayed) {
    if (qcepDebug(DEBUG_PERKINELMER)) {
      printMessage("Delayed Acquisition Startup");
    }

    startupAcquisition();
    m_StartupDelayed = 0;
  }

  m_Counter.fetchAndStoreOrdered(0);

  QxrdPerkinElmerPluginInterfacePtr plugin(m_PerkinElmerPlugin);

  if (plugin && m_AcqDesc) {
    if (qcepDebug(DEBUG_PERKINELMER)) {
      printMessage("Reset frame counter");
    }

    plugin -> Acquisition_ResetFrameCnt(m_AcqDesc);
  }
  return true;
#else
  return false;
#endif
}

bool QxrdDetectorDriverPerkinElmer::endAcquisition()
{
  THREAD_CHECK;

#ifdef HAVE_PERKIN_ELMER
  return true;
#else
  return false;
#endif
}

void QxrdDetectorDriverPerkinElmer::beginFrame()
{
}

bool QxrdDetectorDriverPerkinElmer::shutdownAcquisition()
{
  THREAD_CHECK;

#ifdef HAVE_PERKIN_ELMER
  QxrdPerkinElmerPluginInterfacePtr plugin(m_PerkinElmerPlugin);

  if (plugin && m_AcqDesc) {
    plugin->Acquisition_Abort(m_AcqDesc);
  }
  return true;
#else
  return false;
#endif
}

void QxrdDetectorDriverPerkinElmer::onEndFrame(int counter, unsigned int n1, unsigned int n2)
{
  THREAD_CHECK;

#ifdef HAVE_PERKIN_ELMER
  //  QTime tic;
  //  tic.start();

  QxrdDetectorSettingsPerkinElmerPtr det(m_PerkinElmer);

  if (det && det -> checkDetectorEnabled() && checkPluginAvailable()) {
    QxrdPerkinElmerPluginInterfacePtr plugin(m_PerkinElmerPlugin);
    QxrdAcquisitionPtr acq(m_Acquisition);

    if (plugin && acq) {
      QcepUInt16ImageDataPtr image = QcepAllocator::newInt16Image(tr("frame-%1").arg(counter),
                                                                 det -> get_NCols(), det -> get_NRows(),
                                                                 QcepAllocator::AllocateFromReserve);

      //    printf("allocator took %d msec\n", tic.restart());

      if (qcepDebug(DEBUG_PERKINELMERIDLING)) {
        printMessage(tr("QxrdDetectorPerkinElmer::onEndFrame(%1,%2,%3)")
                     .arg(counter).arg(n1).arg(n2));
      }

      QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

      //  if (get_Cancelling()) {
      //    set_Cancelling(false);
      //    return /*true*/;
      //  }

      long npixels = det -> get_NRows() * det -> get_NCols();

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
        if (qcepDebug(DEBUG_PERKINELMERIDLING)) {
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

      if (qcepDebug(DEBUG_PERKINELMERIDLING)) {
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

      if (image) {
        image->set_ImageSequenceNumber(counter1);
        image->set_ImageNumber(n1);
      }

      det -> enqueueAcquiredFrame(image);
    }
  }
#endif
}

void QxrdDetectorDriverPerkinElmer::acquisitionInitError(const char *fn, int ln, int n)
{
  acquisitionError(fn, ln, n);

  criticalMessage("Detector Initialization Failed");
}

void QxrdDetectorDriverPerkinElmer::acquisitionNSensorsError(const char *fn, int ln, int n)
{
  acquisitionError(fn, ln, n);

  criticalMessage("Detector Initialization Failed");
}

//void QxrdDetectorDriverPerkinElmer::setupCameraGainMenu(QComboBox *cb, int initialGain)
//{
//  if (qcepDebug(DEBUG_PERKINELMER)) {
//    printMessage(tr("QxrdDetectorDriverPerkinElmer::setupCameraGainMenu m_HeaderID == %1").arg(m_HeaderID));
//  }

//  bool b = cb -> blockSignals(true);

//  if (m_HeaderID == 11) { /* AM type */
//    for (int i=0; i<16; i++) {
//      QString msg;

//      if (i==0) {
//        msg = "High: ";
//      } else if (i==15) {
//        msg = "Low: ";
//      }

//      double value = 0.1;
//      if (i & 1) {
//        value += 0.3;
//      }

//      if (i & 2) {
//        value += 0.9;
//      }

//      if (i & 4) {
//        value += 4.7;
//      }

//      if (i & 8) {
//        value += 10.0;
//      }

//      msg += tr("%1 pF").arg(value);

//      cb -> addItem(msg);
//    }
//  } else if (m_HeaderID >= 12) { /* AN type */
//    cb -> addItem(tr("High: 0.25 pF"));
//    cb -> addItem(tr("0.5 pF"));
//    cb -> addItem(tr("1 pF"));
//    cb -> addItem(tr("2 pF"));
//    cb -> addItem(tr("4 pF"));
//    cb -> addItem(tr("Low: 8 pF"));
//  } else {
//    cb -> addItem(tr("Gain not settable"));
//  }

//  cb -> setCurrentIndex(initialGain);

//  cb -> blockSignals(b);
//}

//void QxrdDetectorDriverPerkinElmer::setupCameraBinningModeMenu(QComboBox *cb, int initialBinning)
//{
////  printf("QxrdDetectorDriverPerkinElmer::setupCameraBinningModeMenu m_HeaderID == %d, m_CameraType == %d\n",
////         m_HeaderID, m_CameraType);
//  bool b = cb -> blockSignals(true);

//  if (m_HeaderID == 14) {
//    if (m_CameraType == 1) {
//      cb -> addItem(tr("No binning"));
//      cb -> addItem(tr("2x2 binning"));
//    } else {
//      cb -> addItem(tr("No binning"));
//      cb -> addItem(tr("2x2 binning"));
//    }
//  } else {
//    cb -> addItem(tr("No binning"));
//  }

//  cb -> setCurrentIndex(initialBinning);

//  cb -> blockSignals(b);
//}

void QxrdDetectorDriverPerkinElmer::onEndFrameCallback()
{
#ifdef HAVE_PERKIN_ELMER
//  QTime tic;
  //  tic.start();

  QxrdDetectorSettingsPerkinElmerPtr det(m_PerkinElmer);

  if (det && det -> checkDetectorEnabled() && checkPluginAvailable()) {
    QxrdPerkinElmerPluginInterfacePtr plugin(m_PerkinElmerPlugin);
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

      int counter = m_Counter.fetchAndAddOrdered(1) + 1;

      //    printf("m_Counter.fetchAndAddOrdered took %d msec\n", tic.restart());

      QMetaObject::invokeMethod(this, "onEndFrame", Q_ARG(int, counter), Q_ARG(uint, actualFrame), Q_ARG(uint, actSecFrame));
//      onEndFrame(counter, actualFrame, actSecFrame);
    }
  }
#endif
}

#ifdef HAVE_PERKIN_ELMER
static void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc)
{
  QxrdApplication *app = qobject_cast<QxrdApplication*>(g_Application);

  if (app) {
    QxrdPerkinElmerPluginInterfacePtr plugin(app->perkinElmerPlugin());

    if (hAcqDesc && plugin) {
      ACQDATATYPE object;

      plugin->Acquisition_GetAcqData(hAcqDesc, &object);

      QxrdDetectorDriverPerkinElmer *detector = reinterpret_cast<QxrdDetectorDriverPerkinElmer*>(object);

      if (detector /*&& detector->isEnabled()*/) {
        detector->onEndFrameCallback();
      }
    }
  }
}

static void CALLBACK OnEndAcqCallback(HACQDESC /*hAcqDesc*/)
{
}
#endif

void QxrdDetectorDriverPerkinElmer::dumpHeaderInfo()
{
#ifdef HAVE_PERKIN_ELMER
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
#endif
}