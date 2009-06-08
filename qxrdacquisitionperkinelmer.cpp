#include "qxrdacquisitionperkinelmer.h"
#include "qxrdacquisitionthread.h"
#include "qxrdapplication.h"
#include "qxrdimagedata.h"
#include "qxrdwindow.h"

#ifdef Q_OS_UNIX
#include "AcqLinuxTypes.h"
#endif

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

#include <math.h>

#include "Acq.h"
#include <QThread>
#include <QDir>
#include <QFile>
#include <QTime>
#include <QMutexLocker>
#include <QMetaProperty>

static QxrdAcquisitionPerkinElmer * g_Acquisition = NULL;

QxrdAcquisitionPerkinElmer::QxrdAcquisitionPerkinElmer(QxrdAcquisitionThread *thread)
  : inherited(thread),
    m_Cancelling(0),
    m_AcquireDark(0),
    m_NRows(0),
    m_NCols(0),
    m_ExposuresToSum(0),
    m_CurrentFile(0),
    m_BufferSize(0),
    m_AcquiredData(NULL)
{
  ::g_Acquisition = this;
}

QxrdAcquisitionPerkinElmer::~QxrdAcquisitionPerkinElmer()
{
  if (m_AcquiredData) {
    delete m_AcquiredData;
  }
}

void QxrdAcquisitionPerkinElmer::acquire()
{
//  printf("QxrdAcquisitionPerkinElmer::acquire (thread()=%p, currentThread()=%p)\n",
//         thread(), QThread::currentThread());
  QMutexLocker lock(&m_Acquiring);

  emit printMessage(tr("QxrdAcquisitionPerkinElmer::acquire()"));
  emit statusMessage("Starting acquisition");
  emit acquireStarted(0);

  acquisition(0);
}

void QxrdAcquisitionPerkinElmer::acquireDark()
{
//  printf("QxrdAcquisitionPerkinElmer::acquireDark (thread()=%p, currentThread()=%p)\n",
//         thread(), QThread::currentThread());
  QMutexLocker lock(&m_Acquiring);

  emit printMessage(tr("QxrdAcquisitionPerkinElmer::acquireDark()\n"));
  emit statusMessage("Starting dark acquisition");
  emit acquireStarted(1);

  acquisition(1);
}

void QxrdAcquisitionPerkinElmer::cancel()
{
  haltAcquire();
}

void QxrdAcquisitionPerkinElmer::cancelDark()
{
  haltAcquire();
}

static void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc);
static void CALLBACK OnEndAcqCallback(HACQDESC hAcqDesc);

static HACQDESC m_AcqDesc = NULL;

void QxrdAcquisitionPerkinElmer::initialize()
{
  emit printMessage("QxrdAcquisitionPerkinElmer::initialize()\n");

  int nRet = HIS_ALL_OK;
  UINT nSensors;
  BOOL bEnableIRQ = true;
  ACQDESCPOS Pos = 0;
  UINT nChannelType;
  int nChannelNr;
  UINT dwSortFlags, dwDataType, dwRows, dwColumns, dwFrames;
  DWORD dwAcqType, dwSystemID, dwSyncMode, dwHwAccess;

  nRet = Acquisition_EnumSensors(&nSensors, bEnableIRQ, FALSE);

  emit printMessage(tr("Acquisition_EnumSensors = %1\n").arg(nRet));

  if (nRet != HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  emit printMessage(tr("Number of sensors = %1\n").arg(nSensors));

  if ((nRet = Acquisition_GetNextSensor(&Pos, &m_AcqDesc))!=HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  if ((nRet=Acquisition_GetCommChannel(m_AcqDesc, &nChannelType, &nChannelNr))!=HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  if ((nRet=Acquisition_GetConfiguration(m_AcqDesc, &dwFrames, &dwRows, &dwColumns, &dwDataType,
                                         &dwSortFlags, &bEnableIRQ, &dwAcqType,
                                         &dwSystemID, &dwSyncMode, &dwHwAccess))!=HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  m_NRows = dwRows;
  m_NCols = dwColumns;

  int nReadoutTimes = 8;
  double readoutTimes[8];

  if ((nRet=Acquisition_GetIntTimes(m_AcqDesc, readoutTimes, &nReadoutTimes)) != HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  for (int i=0; i<nReadoutTimes; i++) {
    m_ReadoutTimes.append(readoutTimes[i]);
  }

  emit printMessage(tr("channel type: %1, ChannelNr: %2\n").arg(nChannelType).arg(nChannelNr));
  emit printMessage(tr("frames: %1\n").arg(dwFrames));
  emit printMessage(tr("rows: %1\ncolumns: %2\n").arg(dwRows).arg(dwColumns));

  if ((nRet=Acquisition_SetCallbacksAndMessages(m_AcqDesc, NULL, 0,
                                                0, OnEndFrameCallback, OnEndAcqCallback))!=HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  for (int i=0; i<10; i++) {
    returnImageToPool(new QxrdImageData(m_NCols, m_NRows));
  }
}

void QxrdAcquisitionPerkinElmer::acquisition(int isDark)
{
  int nRet = HIS_ALL_OK;
  m_AcquireDark = isDark;
  m_BufferSize = 10;
  m_BufferIndex = 0;

  if (m_AcquireDark) {
    m_ExposuresToSum = darkSummedExposures();
    m_FilesInSequence = 1;
  } else {
    m_ExposuresToSum = summedExposures();
    m_FilesInSequence = filesInSequence();
  }

  if (m_AcquiredData == NULL) {
    m_AcquiredData = takeNextFreeImage();
  }

  m_AcquiredData->resize(m_NCols, m_NRows);
  m_AcquiredData->clear();

  m_Buffer.resize(m_NRows*m_NCols*m_BufferSize);
  m_Buffer.fill(0);

  if ((nRet=Acquisition_SetCallbacksAndMessages(m_AcqDesc, NULL, 0,
                                                0, OnEndFrameCallback, OnEndAcqCallback))!=HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  if ((nRet=Acquisition_SetCameraMode(m_AcqDesc, readoutMode())) != HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  if (readoutTime() >= exposureTime()) {
    if ((nRet=Acquisition_SetFrameSyncMode(m_AcqDesc, HIS_SYNCMODE_FREE_RUNNING)) != HIS_ALL_OK) {
      acquisitionError(nRet);
    }
  } else {
    if ((nRet=Acquisition_SetFrameSyncMode(m_AcqDesc, HIS_SYNCMODE_INTERNAL_TIMER)) != HIS_ALL_OK) {
      acquisitionError(nRet);
    }

    DWORD tmp = (int)(exposureTime()*1e6);

    if ((nRet=Acquisition_SetTimerSync(m_AcqDesc, &tmp)) != HIS_ALL_OK) {
      acquisitionError(nRet);
    }

    setExposureTime(tmp/1.0e6);
  }

  if ((nRet=Acquisition_SetCameraGain(m_AcqDesc, cameraGain())) != HIS_ALL_OK) {
    acquisitionError(nRet);
  }

  if ((nRet=Acquisition_DefineDestBuffers(m_AcqDesc, m_Buffer.data(), m_BufferSize,
                                          m_NRows, m_NCols)) != HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  m_CurrentExposure = 0;
  m_CurrentFile = 0;

  if ((nRet=Acquisition_Acquire_Image(m_AcqDesc, m_BufferSize,
                                      0, HIS_SEQ_CONTINUOUS, NULL, NULL, NULL)) != HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  forever {
    QMutex mutex;
    QMutexLocker lock(&mutex);
    if (m_AcquisitionWaiting.wait(&mutex, 5000)) {
      if (onEndFrame()) {
        break;
      }
    } else {
      break;
    }
  }

  onEndAcquisition();
}

bool QxrdAcquisitionPerkinElmer::onEndFrame()
{
  printf("QxrdAcquisitionPerkinElmer::onEndFrame()\n");

  if (m_Cancelling) return true;

//  CHwHeaderInfo info;
//  CHwHeaderInfoEx infoEx;
//
//  if (Acquisition_GetHwHeaderInfoEx(m_AcqDesc, &info, &infoEx) == 0) {
//    if (info.dwHeaderID == 14) {
//      printf("Frame %d\n", infoEx.wFrameCnt);
//      emit printMessage(tr("Frame %1").arg(infoEx.wFrameCnt));
//    }
//  }

  QString fileName;

  if (m_AcquireDark) {
    fileName = QDir(outputDirectory())
               .filePath(filePattern()+tr("-%1.dark.tif")
                         .arg(fileIndex(),5,10,QChar('0')));
  } else {
    fileName = QDir(outputDirectory())
               .filePath(filePattern()+tr("-%1.tif")
                         .arg(fileIndex(),5,10,QChar('0')));
  }

  emit acquiredFrame(fileName, fileIndex(),
                     m_CurrentExposure,m_ExposuresToSum,
                     m_CurrentFile, m_FilesInSequence);
  // sum current frame

  long npixels = m_NRows*m_NCols;
  double* current = m_AcquiredData->data();

  unsigned short* frame = m_Buffer.data() + m_BufferIndex*npixels;

//   printf("m_AcquiredImage.data() = %p\n", current);

//   QTime tic;
//   tic.start();

  for (long i=0; i<npixels; i++) {
    *current += *frame;
    current++; frame++;
  }

  m_CurrentExposure++;
  m_BufferIndex++;

//    printf("Frame sum took %d msec\n", tic.elapsed());

  if (m_BufferIndex >= m_BufferSize) {
    m_BufferIndex = 0;
  }

//  Acquisition_SetReady(m_AcqDesc, true);

  if (m_CurrentExposure >= m_ExposuresToSum) {
    m_CurrentExposure = 0;

    QFileInfo finfo(fileName);

    m_AcquiredData -> setFilename(fileName);
    m_AcquiredData -> setTitle(finfo.fileName());
    m_AcquiredData -> setReadoutMode(readoutMode());
    m_AcquiredData -> setExposureTime(exposureTime());
    m_AcquiredData -> setSummedExposures(m_ExposuresToSum);

    if (m_AcquireDark) {
      m_AcquiredData -> setImageNumber(-1);
    } else {
      m_AcquiredData -> setImageNumber(m_CurrentFile);
    }

    newAcquiredImage(m_AcquiredData);

    m_AcquiredData = takeNextFreeImage();
    m_AcquiredData -> resize(m_NCols, m_NRows);
    m_AcquiredData -> clear();

    emit statusMessage("Saving "+fileName);
    emit printMessage("Saving """+fileName+"""");

    setFileIndex(fileIndex()+1);
    m_CurrentFile++;

    if (m_CurrentFile >= m_FilesInSequence) {
      emit printMessage("Acquisition ended\n");
      emit printMessage("Aborted acquisition\n");

      haltAcquire();

      return true;
    }
  }

  return false;
}

void QxrdAcquisitionPerkinElmer::onEndAcquisition()
{
  m_Cancelling = false;

  m_StatusWaiting.wakeAll();

  emit printMessage("(CB) Acquisition ended\n");

  emit statusMessage("Waiting for saves");

  emit statusMessage("Acquire Complete");
  emit acquireComplete(m_AcquireDark);
}

int QxrdAcquisitionPerkinElmer::acquisitionStatus(double time)
{
  if (m_Acquiring.tryLock()) {
    m_Acquiring.unlock();

    printf("m_Acquiring.tryLock() succeeded\n");

    return 1;
  }

  QMutex mutex;
  QMutexLocker lock(&mutex);

  if (m_StatusWaiting.wait(&mutex, (int)(time*1000))) {
    printf("m_StatusWaiting.wait succeeded\n");
    return 1;
  } else {
    printf("m_StatusWaiting.wait failed\n");
    return 0;
  }
}

void QxrdAcquisitionPerkinElmer::acquisitionError(int n)
{
  emit printMessage(tr("Acquisition Error %1\n").arg(n));
}

void QxrdAcquisitionPerkinElmer::haltAcquire()
{
//  printf("QxrdAcquisitionPerkinElmer::haltAcquire (thread = %p)\n", QThread::currentThread());
  m_Cancelling = true;

  Acquisition_Abort(m_AcqDesc);

  emit acquireComplete(m_AcquireDark);
//
//  m_Acquiring.unlock();
}

void QxrdAcquisitionPerkinElmer::onEndFrameCallback()
{
  m_AcquisitionWaiting.wakeAll();
}

QVector<double> QxrdAcquisitionPerkinElmer::readoutTimes()
{
  return m_ReadoutTimes;
}

double QxrdAcquisitionPerkinElmer::readoutTime() const
{
  int n = readoutMode();

  return m_ReadoutTimes.value(n);
}

static void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc)
{
  printf("OnEndFrameCallback\n");

  g_Acquisition -> onEndFrameCallback();
}

static void CALLBACK OnEndAcqCallback(HACQDESC hAcqDesc)
{
}

