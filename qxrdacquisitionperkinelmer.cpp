/******************************************************************
*
*  $Id: qxrdacquisitionperkinelmer.cpp,v 1.10 2009/07/10 22:54:23 jennings Exp $
*
*******************************************************************/

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
#include <QMetaObject>
#include <QMetaProperty>

static QxrdAcquisitionPerkinElmer * g_Acquisition = NULL;

QxrdAcquisitionPerkinElmer::QxrdAcquisitionPerkinElmer(QxrdAcquisitionThread *thread, QxrdDataProcessor *proc)
  : QxrdAcquisitionOperations(thread, proc),
    m_Mutex(QMutex::Recursive),
    m_Cancelling(0),
    m_AcquireDark(0),
    m_NRows(0),
    m_NCols(0),
    m_ExposuresToSum(0),
    m_CurrentFile(0),
    m_BufferSize(0),
    m_AcquiredData(NULL),
    SOURCE_IDENT("$Id: qxrdacquisitionperkinelmer.cpp,v 1.10 2009/07/10 22:54:23 jennings Exp $")
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
  THREAD_CHECK;

//  printf("QxrdAcquisitionPerkinElmer::acquire (thread()=%p, currentThread()=%p)\n",
//         thread(), QThread::currentThread());
  m_Acquiring.lock();

  emit printMessage(tr("QxrdAcquisitionPerkinElmer::acquire()"));
  emit statusMessage("Starting acquisition");
  emit acquireStarted(0);

  acquisition(0);
}

void QxrdAcquisitionPerkinElmer::acquireDark()
{
  THREAD_CHECK;

//  printf("QxrdAcquisitionPerkinElmer::acquireDark (thread()=%p, currentThread()=%p)\n",
//         thread(), QThread::currentThread());
  m_Acquiring.lock();

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
  THREAD_CHECK;

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
    acquisitionInitError(nRet);
    return;
  }

  emit printMessage(tr("Number of sensors = %1\n").arg(nSensors));

  if (nSensors != 1) {
    acquisitionNSensorsError(nRet);
    return;
  }

  if ((nRet = Acquisition_GetNextSensor(&Pos, &m_AcqDesc))!=HIS_ALL_OK) {
    acquisitionNSensorsError(nRet);
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

    emit oneReadoutModeChanged(i, readoutTimes[i]);
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
  QMutexLocker lock(&m_Mutex);

  int nRet = HIS_ALL_OK;
  m_AcquireDark = isDark;
  m_BufferSize = 10;
  m_BufferIndex = 0;
  m_Cancelling = 0;

  if (m_AcquireDark) {
    m_ExposuresToSum = get_DarkSummedExposures();
    m_FilesInSequence = 1;
  } else {
    m_ExposuresToSum = get_SummedExposures();
    m_FilesInSequence = get_FilesInSequence();
  }

  if (m_FilesInSequence <= 0) {
    m_FilesInSequence = 1;
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

  if ((nRet=Acquisition_SetCameraMode(m_AcqDesc, get_ReadoutMode())) != HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  emit printMessage(tr("Readout time = %1, Exposure Time = %2")
                    .arg(readoutTime()).arg(get_ExposureTime()));

  if (readoutTime() >= get_ExposureTime()) {
    emit printMessage("SetFrameSyncMode HIS_SYNCMODE_FREE_RUNNING");
    if ((nRet=Acquisition_SetFrameSyncMode(m_AcqDesc, HIS_SYNCMODE_FREE_RUNNING)) != HIS_ALL_OK) {
      acquisitionError(nRet);
      return;
    }
  } else {
    emit printMessage("SetFrameSyncMode HIS_SYNCMODE_INTERNAL_TIMER");
    if ((nRet=Acquisition_SetFrameSyncMode(m_AcqDesc, HIS_SYNCMODE_INTERNAL_TIMER)) != HIS_ALL_OK) {
      acquisitionError(nRet);
      return;
    }

    DWORD tmp = (int)(get_ExposureTime()*1e6);
    emit printMessage(tr("SetTimerSync %1").arg(tmp));

    if ((nRet=Acquisition_SetTimerSync(m_AcqDesc, &tmp)) != HIS_ALL_OK) {
      acquisitionError(nRet);
      return;
    }

    emit printMessage(tr("TimerSync = %1").arg(tmp));

    set_ExposureTime(tmp/1.0e6);
  }

  if ((nRet=Acquisition_SetCameraGain(m_AcqDesc, get_CameraGain())) != HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
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
}

void QxrdAcquisitionPerkinElmer::onEndFrame()
{
//  emit printMessage("QxrdAcquisitionPerkinElmer::onEndFrame()");
//
  QMutexLocker lock(&m_Mutex);

  if (m_Cancelling) {
    m_Cancelling = false;
    return /*true*/;
  }

  QString fileName;
  QString fileBase;

  if (m_AcquireDark) {
    fileBase = get_FilePattern()+tr("-%1.dark.tif").arg(get_FileIndex(),5,10,QChar('0'));
    fileName = QDir(get_OutputDirectory())
               .filePath(get_FilePattern()+tr("-%1.dark.tif")
                         .arg(get_FileIndex(),5,10,QChar('0')));
  } else {
    fileBase = get_FilePattern()+tr("-%1.tif").arg(get_FileIndex(),5,10,QChar('0'));
    fileName = QDir(get_OutputDirectory())
               .filePath(get_FilePattern()+tr("-%1.tif")
                         .arg(get_FileIndex(),5,10,QChar('0')));
  }

//  emit printMessage(tr("Fn: %1, Fi: %2, Exp: %3, Nexp %4, Fil: %5, NFil: %6")
//                    .arg(fileName).arg(fileIndex())
//                    .arg(m_CurrentExposure).arg(m_ExposuresToSum)
//                    .arg(m_CurrentFile).arg(m_FilesInSequence));

  set_FileBase(fileBase);
  set_FileName(fileName);

  emit acquiredFrame(fileName, get_FileIndex(),
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

    m_AcquiredData -> set_FileName(fileName);
    m_AcquiredData -> set_Title(finfo.fileName());
    m_AcquiredData -> set_ReadoutMode(get_ReadoutMode());
    m_AcquiredData -> set_ExposureTime(get_ExposureTime());
    m_AcquiredData -> set_SummedExposures(m_ExposuresToSum);

    if (m_AcquireDark) {
      m_AcquiredData -> set_ImageNumber(-1);
    } else {
      m_AcquiredData -> set_ImageNumber(m_CurrentFile);
    }

    newAcquiredImage(m_AcquiredData);

    m_AcquiredData = takeNextFreeImage();
    m_AcquiredData -> resize(m_NCols, m_NRows);
    m_AcquiredData -> clear();

    emit statusMessage("Saving "+fileName);
    emit printMessage("Saving """+fileName+"""");

    set_FileIndex(get_FileIndex()+1);
    m_CurrentFile++;

    if (m_CurrentFile >= m_FilesInSequence) {
      emit printMessage("Acquisition ended\n");
      emit printMessage("Aborted acquisition\n");

      haltAcquire();

//      return true;
    }
  }

//  return false;
}

void QxrdAcquisitionPerkinElmer::haltAcquire()
{
  if (QThread::currentThread() != thread()) {
    printf("Ooops...\n");
  }

  m_Cancelling = true;

  Acquisition_Abort(m_AcqDesc);

  emit statusMessage("Acquire Complete");
  emit acquireComplete(m_AcquireDark);

  m_Acquiring.tryLock();
  m_Acquiring.unlock();

  m_StatusWaiting.wakeAll();
}

int QxrdAcquisitionPerkinElmer::acquisitionStatus(double time)
{
  if (m_Acquiring.tryLock()) {
    m_Acquiring.unlock();

//    printf("m_Acquiring.tryLock() succeeded\n");

    return 1;
  }

  QMutex mutex;
  QMutexLocker lock(&mutex);

  if (m_StatusWaiting.wait(&mutex, (int)(time*1000))) {
//    printf("m_StatusWaiting.wait succeeded\n");
    return 1;
  } else {
//    printf("m_StatusWaiting.wait failed\n");
    return 0;
  }
}

void QxrdAcquisitionPerkinElmer::acquisitionError(int n)
{
  haltAcquire();

  emit printMessage(tr("Acquisition Error %1\n").arg(n));
  emit statusMessage(tr("Acquisition Error %1\n").arg(n));
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

void QxrdAcquisitionPerkinElmer::onEndFrameCallback()
{
//  m_AcquisitionWaiting.wakeAll();
}

QVector<double> QxrdAcquisitionPerkinElmer::readoutTimes()
{
  QMutexLocker lock(&m_Mutex);

  return m_ReadoutTimes;
}

double QxrdAcquisitionPerkinElmer::readoutTime() const
{
  QMutexLocker lock(&m_Mutex);

  int n = get_ReadoutMode();

  return m_ReadoutTimes.value(n)/1e6;
}

static void CALLBACK OnEndFrameCallback(HACQDESC /*hAcqDesc*/)
{
//  printf("OnEndFrameCallback\n");

//  g_Acquisition -> onEndFrameCallback();

  QMetaObject::invokeMethod(g_Acquisition, "onEndFrame", Qt::QueuedConnection);
//  QTimer::singleShot(0, , SLOT(onEndFrame()));
}

static void CALLBACK OnEndAcqCallback(HACQDESC /*hAcqDesc*/)
{
}


/******************************************************************
*
*  $Log: qxrdacquisitionperkinelmer.cpp,v $
*  Revision 1.10  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.9  2009/07/09 01:15:09  jennings
*  Added some locks
*
*  Revision 1.8  2009/06/28 16:33:20  jennings
*  Eliminated compiler warnings
*
*  Revision 1.7  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

