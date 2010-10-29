//#ifdef HAVE_PERKIN_ELMER

#include "qxrdacquisitionperkinelmer.h"
//#include "qxrdacquisitionthread.h"
#include "qxrddataprocessor.h"
#include "qxrdapplication.h"
#include "qxrdimagedata.h"
#include "qxrdwindow.h"

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
             printf("QxrdAcquisitionPerkinElmer::QxrdAcquisitionPerkinElmer\n");
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
    emit criticalMessage("No perkin elmer plugin available\n");
  }

  return m_PerkinElmer;
}

void QxrdAcquisitionPerkinElmer::onExposureTimeChanged(double newTime)
{
  if (checkPluginAvailable()) {

    QCEP_DEBUG(DEBUG_PERKINELMER,
               printf("Exposure time changed to %g\n", newTime);
    );

    emit printMessage(tr("Exposure time changed to %1").arg(newTime));

    DWORD tmp = (int)(get_ExposureTime()*1e6);

    emit printMessage(tr("SetTimerSync %1").arg(tmp));

    int nRet;

    if ((nRet=m_PerkinElmer->Acquisition_SetTimerSync(m_AcqDesc, &tmp)) != HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    emit printMessage(tr("TimerSync = %1").arg(tmp));

    set_ExposureTime(tmp/1.0e6);
  }
}

void QxrdAcquisitionPerkinElmer::onBinningModeChanged(int newMode)
{
  if (checkPluginAvailable()) {

    QCEP_DEBUG(DEBUG_PERKINELMER,
               printf("Binning mode changed to %d\n", newMode);
    );

    emit printMessage(tr("Binning mode changed to %1").arg(newMode));

    //    WORD binningMode = get_BinningMode();
    //    emit printMessage(tr("Setting binning mode = %1").arg(binningMode));
    //
    //    if ((nRet=Acquisition_SetCameraBinningMode(m_AcqDesc, binningMode)) != HIS_ALL_OK) {
    //      acquisitionError(__LINE__, nRet);
    //      return;
    //    }
    //
    //    if ((nRet=Acquisition_GetCameraBinningMode(m_AcqDesc, &binningMode)) != HIS_ALL_OK) {
    //      acquisitionError(__LINE__, nRet);
    //      return;
    //    }
    //
    //    emit printMessage(tr("Binning mode was set to %1").arg(binningMode));
  }
}

void QxrdAcquisitionPerkinElmer::onCameraGainChanged(int newGain)
{
  if (checkPluginAvailable()) {

    QCEP_DEBUG(DEBUG_PERKINELMER,
               printf("Camera gain changed to %d\n", newGain);
    );

    emit printMessage(tr("Camera Gain Changed to %1").arg(newGain));

    emit printMessage("Setting camera gain");

    int nRet;

    if (m_CurrentGain != get_CameraGain()) {
      if ((nRet=m_PerkinElmer->Acquisition_SetCameraGain(m_AcqDesc, get_CameraGain())) != HIS_ALL_OK) {
        acquisitionError(__LINE__, nRet);
        return;
      }
      m_CurrentGain = get_CameraGain();
    }

    emit printMessage("Set camera gain");
  }
}

void QxrdAcquisitionPerkinElmer::initialize()
{
  if (checkPluginAvailable()) {

    QCEP_DEBUG(DEBUG_PERKINELMER,
               printf("QxrdAcquisitionPerkinElmer::initialize\n");
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

    nRet = m_PerkinElmer->Acquisition_EnumSensors(&nSensors, bEnableIRQ, FALSE);

    QCEP_DEBUG(DEBUG_PERKINELMER,
               printf("Acquisition_EnumSensors = %d\n",nRet);
    );

    if (nRet != HIS_ALL_OK) {
      acquisitionInitError(nRet);
      return;
    }

    QCEP_DEBUG(DEBUG_PERKINELMER,
               printf("Number of sensors = %d\n",nSensors);
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

    if ((nRet=m_PerkinElmer->Acquisition_GetConfiguration(m_AcqDesc, &dwFrames, &dwRows, &dwColumns, &dwDataType,
                                           &dwSortFlags, &bEnableIRQ, &dwAcqType,
                                           &dwSystemID, &dwSyncMode, &dwHwAccess))!=HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    set_NRows(dwRows);
    set_NCols(dwColumns);

    QxrdAcquisition::initialize();

    CHwHeaderInfo hwHeaderInfo;

    if ((nRet=m_PerkinElmer->Acquisition_GetHwHeaderInfo(m_AcqDesc, &hwHeaderInfo)) != HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    QCEP_DEBUG(DEBUG_PERKINELMER,
               printf("Prom ID %d, Header ID %d\n",
                      (int) hwHeaderInfo.dwPROMID,
                      (int) hwHeaderInfo.dwHeaderID);
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
                 printf("Camera Type %d\n", hdrx.wCameratype);
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

    QCEP_DEBUG(DEBUG_PERKINELMER,
               printf("channel type: %d, ChannelNr: %d\n",nChannelType,nChannelNr);
               printf("frames: %d rows: %d, columns: %d\n",dwFrames,dwRows,dwColumns);
    );

    if ((nRet=m_PerkinElmer->Acquisition_SetCallbacksAndMessages(m_AcqDesc, NULL, 0,
                                                  0, OnEndFrameCallback, OnEndAcqCallback))!=HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    m_BufferSize = 4;
    m_BufferIndex = 0;

    m_Buffer.resize(get_NRows()*get_NCols()*m_BufferSize);
    m_Buffer.fill(0);

    QCEP_DEBUG(DEBUG_PERKINELMER,
               printf("Exposure Time = %g\n", get_ExposureTime());
               printf("SetFrameSyncMode HIS_SYNCMODE_INTERNAL_TIMER\n");
    );

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
               printf("Define Dest Buffers");
    );

    m_CurrentExposure = 0;
    m_CurrentFile = 0;

    if ((nRet=m_PerkinElmer->Acquisition_Acquire_Image(m_AcqDesc, m_BufferSize,
                                        0, HIS_SEQ_CONTINUOUS, NULL, NULL, NULL)) != HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    QCEP_DEBUG(DEBUG_PERKINELMER,
               printf("Acquire");
    );
  }
}

void QxrdAcquisitionPerkinElmer::onEndFrame(int counter, unsigned int n1, unsigned int n2)
{
  if (checkPluginAvailable()) {

    QTime tic;
    tic.start();

    QCEP_DEBUG(DEBUG_PERKINELMER,
               emit printMessage("QxrdAcquisitionPerkinElmer::onEndFrame()");
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
      printf("%d frames behind [%d,%d], skipping\n", counter1-counter, counter, counter1);
      return;
    }

    if (((actSecFrame-1)%m_BufferSize) != m_BufferIndex) {
      QCEP_DEBUG(DEBUG_PERKINELMER,
                 emit printMessage(tr("actSecFrame %1, m_BufferIndex %2").arg(actSecFrame).arg(m_BufferIndex));
      )
    }

    quint16* current = (m_AcquiredInt16Data ? m_AcquiredInt16Data->data() : NULL);
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
               emit printMessage(tr("Frame checksum 0x%1, avg %2\n").arg(cksum,8,16,QChar('0')).arg(avg/npixels));
    );

    m_BufferIndex = (m_BufferIndex+1)%m_BufferSize;

    acquiredFrameAvailable();
  }
}

void QxrdAcquisitionPerkinElmer::acquisitionInitError(int n)
{
  acquisitionError(n);

  emit criticalMessage("Detector Initialization Failed");
}

void QxrdAcquisitionPerkinElmer::acquisitionNSensorsError(int n)
{
  acquisitionError(n);

  emit criticalMessage("Detector Initialization Failed");
}

void QxrdAcquisitionPerkinElmer::setupExposureMenu(QDoubleSpinBox *cb)
{
//  foreach(double t, m_ReadoutTimes) {
//    cb -> addItem(tr("%1").arg(t/1e6,5,'f'));
//  }
}

void QxrdAcquisitionPerkinElmer::setupCameraGainMenu(QComboBox *cb)
{
  cb -> addItem(tr("High: 0.25 pF"));
  cb -> addItem(tr("0.5 pF"));
  cb -> addItem(tr("1 pF"));
  cb -> addItem(tr("2 pF"));
  cb -> addItem(tr("4 pF"));
  cb -> addItem(tr("Low: 8 pF"));
}

void QxrdAcquisitionPerkinElmer::setupCameraBinningModeMenu(QComboBox *cb)
{
  cb -> addItem(tr("1x1 - 2048x2048 pixels"));
  cb -> addItem(tr("2x2 - 1024x1024 pixels"));
}

void QxrdAcquisitionPerkinElmer::onEndFrameCallback()
{
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
