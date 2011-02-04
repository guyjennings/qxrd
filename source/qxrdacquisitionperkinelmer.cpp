//#ifdef HAVE_PERKIN_ELMER

#include "qxrdacquisitionperkinelmer.h"
//#include "qxrdacquisitionthread.h"
#include "qxrddataprocessor.h"
#include "qxrdapplication.h"
#include "qxrdimagedata.h"
#include "qxrdwindow.h"
#include "qxrdsynchronizedacquisition.h"

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
//#include <QMutexLocker>
#include "qxrdmutexlocker.h"
#include <QMetaObject>
#include <QMetaProperty>
//#include "qxrdxislinterface.h"

static QxrdAcquisitionPerkinElmer * g_Acquisition = NULL;
static void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc);
static void CALLBACK OnEndAcqCallback(HACQDESC hAcqDesc);
static HACQDESC m_AcqDesc = NULL;

QxrdAcquisitionPerkinElmer::QxrdAcquisitionPerkinElmer(QxrdDataProcessorPtr proc, QxrdAllocatorPtr allocator)
  : QxrdAcquisition(proc, allocator),
    m_Mutex(QMutex::Recursive),
    m_BufferSize(0),
    m_PROMID(-1),
    m_HeaderID(-1),
    m_CameraType(-1),
    m_CameraModel(""),
    m_CurrentGain(-1),
    m_Counter(0),
    m_PerkinElmer(0)
{
  QCEP_DEBUG(DEBUG_PERKINELMER,
             emit printMessage(QDateTime::currentDateTime(),
                               "QxrdAcquisitionPerkinElmer::QxrdAcquisitionPerkinElmer");
  );

  ::g_Acquisition = this;
}

//QxrdAcquisitionPerkinElmer::~QxrdAcquisitionPerkinElmer()
//{
//}

bool QxrdAcquisitionPerkinElmer::checkPluginAvailable()
{
  if (m_PerkinElmer == NULL) {
    m_PerkinElmer = g_Application->perkinElmerPlugin();
  }

  if (m_PerkinElmer == NULL) {
    emit criticalMessage(QDateTime::currentDateTime(),
                         "No perkin elmer plugin available");
  }

  return m_PerkinElmer;
}

void QxrdAcquisitionPerkinElmer::onExposureTimeChanged(double newTime)
{
  if (checkPluginAvailable()) {

    emit printMessage(QDateTime::currentDateTime(),
                      tr("Exposure time changed to %1").arg(newTime));

    DWORD tmp = (int)(get_ExposureTime()*1e6);

    emit printMessage(QDateTime::currentDateTime(),
                      tr("SetTimerSync %1").arg(tmp));

    int nRet;

    if ((nRet=m_PerkinElmer->Acquisition_SetTimerSync(m_AcqDesc, &tmp)) != HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    emit printMessage(QDateTime::currentDateTime(),
                      tr("TimerSync = %1").arg(tmp));

    set_ExposureTime(tmp/1.0e6);
  }
}

void QxrdAcquisitionPerkinElmer::onBinningModeChanged(int newMode)
{
  if (checkPluginAvailable()) {

    if (m_HeaderID == 14) {
      emit printMessage(QDateTime::currentDateTime(),
                        tr("Binning mode changed to %1").arg(newMode));

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
        acquisitionError(__LINE__, nRet);
        return;
      }

      emit printMessage(QDateTime::currentDateTime(),
                        tr("Starting binning mode = %1").arg(originalMode));

      emit printMessage(QDateTime::currentDateTime(),
                        tr("Setting binning mode = %1").arg(newMode));

      if ((nRet=m_PerkinElmer->Acquisition_SetCameraBinningMode(m_AcqDesc, newMode)) != HIS_ALL_OK) {
        acquisitionError(__LINE__, nRet);
        return;
      }

      if ((nRet=m_PerkinElmer->Acquisition_GetCameraBinningMode(m_AcqDesc, &binningMode)) != HIS_ALL_OK) {
        acquisitionError(__LINE__, nRet);
        return;
      }

      emit printMessage(QDateTime::currentDateTime(),
                        tr("Binning mode was set to %1").arg(binningMode));
    }
  }
}

void QxrdAcquisitionPerkinElmer::onCameraGainChanged(int newGain)
{
  if (checkPluginAvailable()) {

    if (m_HeaderID >= 11) {
      emit printMessage(QDateTime::currentDateTime(),
                        tr("Camera Gain Changed to %1").arg(newGain));

      emit printMessage(QDateTime::currentDateTime(), "Setting camera gain");

      int nRet;

      if (m_CurrentGain != get_CameraGain()) {
        if ((nRet=m_PerkinElmer->Acquisition_SetCameraGain(m_AcqDesc, get_CameraGain())) != HIS_ALL_OK) {
          acquisitionError(__LINE__, nRet);
          return;
        }
        m_CurrentGain = get_CameraGain();
      }

      emit printMessage(QDateTime::currentDateTime(), "Set camera gain");
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
  printf("QxrdAcquisitionPerkinElmer::initialize\n");

  if (checkPluginAvailable()) {

    QCEP_DEBUG(DEBUG_PERKINELMER,
               emit printMessage(QDateTime::currentDateTime(),
                                 tr("QxrdAcquisitionPerkinElmer::initialize"));
    );

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

    QCEP_DEBUG(DEBUG_PERKINELMER,
               emit printMessage(QDateTime::currentDateTime(),
                                 tr("Acquisition_EnumSensors = %1").arg(nRet));
    );

    if (nRet != HIS_ALL_OK) {
      acquisitionInitError(nRet);
      return;
    }

    QCEP_DEBUG(DEBUG_PERKINELMER,
               emit printMessage(QDateTime::currentDateTime(),
                                 tr("Number of sensors = %1").arg(nSensors));
    );

    if (nSensors != 1) {
      acquisitionNSensorsError(nRet);
      return;
    }

    if ((nRet = m_PerkinElmer->Acquisition_GetNextSensor(&Pos, &m_AcqDesc))!=HIS_ALL_OK) {
      acquisitionNSensorsError(nRet);
      return;
    }

    if ((nRet=m_PerkinElmer->Acquisition_GetCommChannel(m_AcqDesc, &nChannelType, &nChannelNr))!=HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    QCEP_DEBUG(DEBUG_PERKINELMER,
               emit printMessage(QDateTime::currentDateTime(),
                                 tr("Acquisition_GetCommChannel channel type = %1, channel no = %2")
                                 .arg(nChannelType).arg(nChannelNr));
    );

    if ((nRet=m_PerkinElmer->Acquisition_GetConfiguration(m_AcqDesc, &dwFrames, &dwRows, &dwColumns, &dwDataType,
                                           &dwSortFlags, &bEnableIRQ, &dwAcqType,
                                           &dwSystemID, &dwSyncMode, &dwHwAccess))!=HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    QCEP_DEBUG(DEBUG_PERKINELMER, {
      emit printMessage(QDateTime::currentDateTime(),
                        tr("Acquisition_GetConfiguration frames = %1, rows = %2, cols = %3")
                        .arg(dwFrames).arg(dwRows).arg(dwColumns));
      emit printMessage(QDateTime::currentDateTime(),
                        tr("Acquisition_GetConfiguration data type = %1, sort flags = %2, IRQ = %3")
                        .arg(dwDataType).arg(dwSortFlags).arg(bEnableIRQ));
      emit printMessage(QDateTime::currentDateTime(),
                        tr("Acquisition_GetConfiguration acq type = %1, systemID = %2, syncMode = %3, hwAccess = %4")
                        .arg(dwAcqType).arg(dwSystemID).arg(dwSyncMode).arg(dwHwAccess));
    })

    set_NRows(dwRows);
    set_NCols(dwColumns);

    if ((nRet=m_PerkinElmer->Acquisition_GetCameraBinningMode(m_AcqDesc, &binningMode)) != HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    if (gCEPDebug & DEBUG_PERKINELMER) {
      emit printMessage(QDateTime::currentDateTime(),
                        tr("Acquisition_GetCameraBinningMode mode = %1").arg(binningMode));
    }

    QxrdAcquisition::initialize();

    CHwHeaderInfo hwHeaderInfo;

    if ((nRet=m_PerkinElmer->Acquisition_GetHwHeaderInfo(m_AcqDesc, &hwHeaderInfo)) != HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    QCEP_DEBUG(DEBUG_PERKINELMER,
      emit printMessage(QDateTime::currentDateTime(),
                        tr("Prom ID %1, Header ID %2").arg(hwHeaderInfo.dwPROMID).arg(hwHeaderInfo.dwHeaderID));
    );

    m_PROMID = hwHeaderInfo.dwPROMID;
    m_HeaderID = hwHeaderInfo.dwHeaderID;

    if (hwHeaderInfo.dwHeaderID >= 14) {
      CHwHeaderInfoEx hdrx;

      if ((nRet = m_PerkinElmer->Acquisition_GetHwHeaderInfoEx(m_AcqDesc, &hwHeaderInfo, &hdrx)) != HIS_ALL_OK) {
        acquisitionError(__LINE__, nRet);
        return;
      }

      QCEP_DEBUG(DEBUG_PERKINELMER,
        emit printMessage(QDateTime::currentDateTime(),
                          tr("Camera Type %1").arg(hdrx.wCameratype));
      );

      m_CameraType = hdrx.wCameratype;
    }

    int nReadoutTimes = 8;
    double readoutTimes[8];

    if ((nRet=m_PerkinElmer->Acquisition_GetIntTimes(m_AcqDesc, readoutTimes, &nReadoutTimes)) != HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    for (int i=0; i<nReadoutTimes; i++) {
      m_ReadoutTimes.append(readoutTimes[i]);
    }

    if ((nRet=m_PerkinElmer->Acquisition_SetCallbacksAndMessages(m_AcqDesc, NULL, 0,
                                                  0, OnEndFrameCallback, OnEndAcqCallback))!=HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    m_BufferSize = 4;
    m_BufferIndex = 0;

    m_Buffer.resize(get_NRows()*get_NCols()*m_BufferSize);
    m_Buffer.fill(0);

    QCEP_DEBUG(DEBUG_PERKINELMER, {
      emit printMessage(QDateTime::currentDateTime(),
                        tr("Exposure Time = %1").arg(get_ExposureTime()));
      emit printMessage(QDateTime::currentDateTime(),
                        tr("SetFrameSyncMode HIS_SYNCMODE_INTERNAL_TIMER"));
    });

    if ((nRet=m_PerkinElmer->Acquisition_SetFrameSyncMode(m_AcqDesc, HIS_SYNCMODE_INTERNAL_TIMER)) != HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    set_BinningMode(0);

    if ((nRet=m_PerkinElmer->Acquisition_DefineDestBuffers(m_AcqDesc, m_Buffer.data(), m_BufferSize,
                                            get_NRows(), get_NCols())) != HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    QCEP_DEBUG(DEBUG_PERKINELMER,
               emit printMessage(QDateTime::currentDateTime(),
                                 tr("Define Dest Buffers"));
    );

    m_CurrentExposure = 0;
    m_CurrentFile = 0;

    if ((nRet=m_PerkinElmer->Acquisition_Acquire_Image(m_AcqDesc, m_BufferSize,
                                        0, HIS_SEQ_CONTINUOUS, NULL, NULL, NULL)) != HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    QCEP_DEBUG(DEBUG_PERKINELMER,
               emit printMessage(QDateTime::currentDateTime(), "Acquire");
    );
  }
}

void QxrdAcquisitionPerkinElmer::onEndFrame(int counter, unsigned int n1, unsigned int n2)
{
  if (checkPluginAvailable()) {

    QTime tic;
    tic.start();

    QCEP_DEBUG(DEBUG_PERKINELMER,
               emit printMessage(QDateTime::currentDateTime(),
                                 "QxrdAcquisitionPerkinElmer::onEndFrame()");
    );

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
      emit printMessage(QDateTime::currentDateTime(),
                        tr("%1 frames behind [%2,%3], skipping").arg(counter1-counter).arg(counter).arg(counter1));
      return;
    }

    if (((actSecFrame-1)%m_BufferSize) != m_BufferIndex) {
      QCEP_DEBUG(DEBUG_PERKINELMER,
                 emit printMessage(QDateTime::currentDateTime(),
                                   tr("actSecFrame %1, m_BufferIndex %2").arg(actSecFrame).arg(m_BufferIndex));
      )
    }

    quint16* current = (m_AcquiredInt16Data[0] ? m_AcquiredInt16Data[0]->data() : NULL);
    quint32  cksum = 0;
    double   avg = 0;

    for (long i=0; i<npixels; i++) {
      unsigned short val = *frame++;
      cksum += val;
      avg += val;
      if (current) {
        *current++ = val;
      }
    }

  //  set_Average(avg/npixels);

    QCEP_DEBUG(DEBUG_PERKINELMER,
               emit printMessage(QDateTime::currentDateTime(),
                                 tr("Frame checksum 0x%1, avg %2\n").arg(cksum,8,16,QChar('0')).arg(avg/npixels));
    );

    m_BufferIndex = (m_BufferIndex+1)%m_BufferSize;

    acquiredFrameAvailable();
  }
}

void QxrdAcquisitionPerkinElmer::acquisitionInitError(int n)
{
  acquisitionError(n);

  emit criticalMessage(QDateTime::currentDateTime(), "Detector Initialization Failed");
}

void QxrdAcquisitionPerkinElmer::acquisitionNSensorsError(int n)
{
  acquisitionError(n);

  emit criticalMessage(QDateTime::currentDateTime(), "Detector Initialization Failed");
}

void QxrdAcquisitionPerkinElmer::setupExposureMenu(QDoubleSpinBox *cb)
{
//  foreach(double t, m_ReadoutTimes) {
//    cb -> addItem(tr("%1").arg(t/1e6,5,'f'));
//  }
}

void QxrdAcquisitionPerkinElmer::setupCameraGainMenu(QComboBox *cb)
{
  printf("QxrdAcquisitionPerkinElmer::setupCameraGainMenu m_HeaderID == %d\n", m_HeaderID);

  if (m_HeaderID == 11) { /* AM type */
  } else if (m_HeaderID >= 12) { /* AN type */
    cb -> addItem(tr("High: 0.25 pF"));
    cb -> addItem(tr("0.5 pF"));
    cb -> addItem(tr("1 pF"));
    cb -> addItem(tr("2 pF"));
    cb -> addItem(tr("4 pF"));
    cb -> addItem(tr("Low: 8 pF"));
  }
}

void QxrdAcquisitionPerkinElmer::setupCameraBinningModeMenu(QComboBox *cb)
{
  printf("QxrdAcquisitionPerkinElmer::setupCameraBinningModeMenu m_HeaderID == %d, m_CameraType == %d\n",
         m_HeaderID, m_CameraType);

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
  static int fc = 0;

  if (synchronizedAcquisition()) {
    synchronizedAcquisition()->acquiredFrameAvailable((fc++) % 10/*m_CurrentExposure*/, m_CurrentFile);
  }

  if (checkPluginAvailable()) {

    DWORD actualFrame, actSecFrame;

    m_PerkinElmer->Acquisition_GetActFrame(m_AcqDesc, &actualFrame, &actSecFrame);

    int counter = m_Counter.fetchAndAddOrdered(1);

    INVOKE_CHECK(QMetaObject::invokeMethod(g_Acquisition, "onEndFrame", Qt::QueuedConnection,
                                           Q_ARG(int, counter),
                                           Q_ARG(unsigned int, actualFrame), Q_ARG(unsigned int, actSecFrame)));
  }
}

static void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc)
{
//  INVOKE_CHECK(QMetaObject::invokeMethod(g_Acquisition, "onEndFrame", Qt::QueuedConnection));
  g_Acquisition->onEndFrameCallback();
}

static void CALLBACK OnEndAcqCallback(HACQDESC /*hAcqDesc*/)
{
}

//#endif // HAVE_PERKIN_ELMER
