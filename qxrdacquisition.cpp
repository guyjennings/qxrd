#include "qxrdacquisition.h"
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

static QxrdAcquisition *g_Acquisition = NULL;

QxrdAcquisition::QxrdAcquisition(QxrdAcquisitionThread *thread)
  : QObject(),
    m_AcquisitionThread(thread),
    m_Cancelling(0),
    m_NRows(0),
    m_NCols(0),
    m_IntegMode(0),
    m_NSums(0),
    m_NFrames(0),
    m_NBufferFrames(0),
    m_CurrentFrame(0),
    m_AcquiringDark(0),
    m_AcquiredData(NULL),
    m_NIntTimes(0)
{
  emit printMessage("Enter QxrdAcquisition::QxrdAcquisition\n");

  g_Acquisition = this;
}

QxrdAcquisition::~QxrdAcquisition()
{
  if (m_AcquiredData) {
    delete m_AcquiredData;
  }
}

static void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc);
static void CALLBACK OnEndAcqCallback(HACQDESC hAcqDesc);

static HACQDESC m_AcqDesc = NULL;

void QxrdAcquisition::initialize()
{
  emit printMessage("QxrdAcquisition::initialize()\n");

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

  m_NIntTimes = 8;
  if ((nRet=Acquisition_GetIntTimes(m_AcqDesc, m_IntTimes, &m_NIntTimes)) != HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
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
    m_AcquisitionThread -> returnImageToPool(new QxrdImageData(m_NCols, m_NRows));
  }
}

bool QxrdAcquisition::canStart()
{
  return true;

  return m_Acquiring.tryLock();
}

void QxrdAcquisition::acquire(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum, int nframes)
{
//  printf("QxrdAcquisition::acquire (thread=%p)\n", QThread::currentThread());
  QMutexLocker lock(&m_Acquiring);

  if (nsum <= 0) nsum = 1;
  if (nframes <= 0) nframes = 1;

  emit printMessage(tr("QxrdAcquisition::acquire(\"%1\",\"%2\",%3,%4,%5,%6)\n")
                    .arg(outDir).arg(filePattern).arg(fileIndex).arg(integmode).arg(nsum).arg(nframes));
  emit statusMessage("Starting acquisition");

  m_OutputDir   = outDir;
  m_FilePattern = filePattern;
  m_FileIndex = fileIndex;
  m_IntegMode = integmode;
  m_NSums = nsum;
  m_NFrames = nframes;
  m_NBufferFrames = 10;
  m_BufferFrame = 0;

  m_AcquiringDark = 0;

  acquisition();
}

void QxrdAcquisition::acquireDark(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum)
{
//  printf("QxrdAcquisition::acquireDark (thread=%p)\n", QThread::currentThread());
  QMutexLocker lock(&m_Acquiring);

  if (nsum <= 0) nsum = 1;

  emit printMessage(tr("QxrdAcquisition::acquireDark(\"%1\",\"%2\",%3,%4,%5)\n")
                    .arg(outDir).arg(filePattern).arg(fileIndex).arg(integmode).arg(nsum));
  emit statusMessage("Starting dark acquisition");

  m_OutputDir   = outDir;
  m_FilePattern = filePattern;
  m_FileIndex = fileIndex;
  m_IntegMode = integmode;
  m_NSums = nsum;
  m_NFrames = 1;
  m_NBufferFrames = 10;
  m_BufferFrame = 0;

  m_AcquiringDark= 1;

  acquisition();
}

void QxrdAcquisition::acquisition()
{
  int nRet = HIS_ALL_OK;

  if (m_AcquiredData == NULL) {
    m_AcquiredData = m_AcquisitionThread -> takeNextFreeImage();
  }

  m_AcquiredData->resize(m_NCols, m_NRows);
  m_AcquiredData->clear();

  m_Buffer.resize(m_NRows*m_NCols*m_NBufferFrames);
  m_Buffer.fill(0);

  if ((nRet=Acquisition_SetCallbacksAndMessages(m_AcqDesc, NULL, 0,
                                                0, OnEndFrameCallback, OnEndAcqCallback))!=HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  if ((nRet=Acquisition_SetCameraMode(m_AcqDesc, m_IntegMode)) != HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  if ((nRet=Acquisition_DefineDestBuffers(m_AcqDesc, m_Buffer.data(), m_NBufferFrames, m_NRows, m_NCols)) != HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  m_CurrentSum = 0;
  m_CurrentFrame = 0;

  if ((nRet=Acquisition_Acquire_Image(m_AcqDesc, m_NBufferFrames, 0, HIS_SEQ_CONTINUOUS, NULL, NULL, NULL)) != HIS_ALL_OK) {
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

bool QxrdAcquisition::onEndFrame()
{
  printf("QxrdAcquisition::onEndFrame()\n");

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

  if (m_AcquiringDark) {
    fileName = QDir(m_OutputDir).filePath(m_FilePattern+tr("-%1.dark.tif").arg(m_FileIndex,5,10,QChar('0')));
  } else {
    fileName = QDir(m_OutputDir).filePath(m_FilePattern+tr("-%1.tif").arg(m_FileIndex,5,10,QChar('0')));
  }

  emit acquiredFrame(fileName, m_FileIndex, m_CurrentSum,m_NSums, m_CurrentFrame, m_NFrames);
  // sum current frame

  long npixels = m_NRows*m_NCols;
  double* current = m_AcquiredData->data();

  unsigned short* frame = m_Buffer.data() + m_BufferFrame*npixels;

//   printf("m_AcquiredImage.data() = %p\n", current);

//   QTime tic;
//   tic.start();

  for (long i=0; i<npixels; i++) {
    *current += *frame;
    current++; frame++;
  }

  m_CurrentSum++;
  m_BufferFrame++;

//    printf("Frame sum took %d msec\n", tic.elapsed());

  if (m_BufferFrame >= m_NBufferFrames) {
    m_BufferFrame = 0;
  }

//  Acquisition_SetReady(m_AcqDesc, true);

  if (m_CurrentSum >= m_NSums) {
    m_CurrentSum = 0;

    QFileInfo finfo(fileName);

    m_AcquiredData -> setFilename(fileName);
    m_AcquiredData -> setTitle(finfo.fileName());
    m_AcquiredData -> setIntegrationMode(m_IntegMode);
    m_AcquiredData -> setNSummed(m_NSums);

    if (m_AcquiringDark) {
      m_AcquiredData -> setFrameNumber(-1);
    } else {
      m_AcquiredData -> setFrameNumber(m_CurrentFrame);
    }

    m_AcquisitionThread -> newAcquiredImage(m_AcquiredData);

    m_AcquiredData = m_AcquisitionThread -> takeNextFreeImage();
    m_AcquiredData -> resize(m_NCols, m_NRows);
    m_AcquiredData -> clear();

    emit statusMessage("Saving "+fileName);
    emit printMessage("Saving """+fileName+"""");

    m_FileIndex++;
    emit fileIndexChanged(m_FileIndex);
    m_CurrentFrame++;

    if (m_CurrentFrame >= m_NFrames) {
      emit printMessage("Acquisition ended\n");
      emit printMessage("Aborted acquisition\n");

      haltAcquire();

      return true;
    }
  }

  return false;
}

void QxrdAcquisition::onEndAcquisition()
{
  m_Cancelling = false;

  m_StatusWaiting.wakeAll();

  emit printMessage("(CB) Acquisition ended\n");

  emit statusMessage("Waiting for saves");

  emit statusMessage("Acquire Complete");
  emit acquireComplete();
}

int QxrdAcquisition::acquisitionStatus(double time)
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

void QxrdAcquisition::acquisitionError(int n)
{
  emit printMessage(tr("Acquisition Error %1\n").arg(n));
}

void QxrdAcquisition::haltAcquire()
{
//  printf("QxrdAcquisition::haltAcquire (thread = %p)\n", QThread::currentThread());
  m_Cancelling = true;

  Acquisition_Abort(m_AcqDesc);

  emit acquireComplete();
//
//  m_Acquiring.unlock();
}

void QxrdAcquisition::cancel()
{
  haltAcquire();
}

void QxrdAcquisition::cancelDark()
{
  haltAcquire();
}

void QxrdAcquisition::onEndFrameCallback()
{
  m_AcquisitionWaiting.wakeAll();
}

static void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc)
{
  printf("OnEndFrameCallback\n");

  g_Acquisition -> onEndFrameCallback();
}

static void CALLBACK OnEndAcqCallback(HACQDESC hAcqDesc)
{
}

QVector<double> QxrdAcquisition::integrationTimes()
{
  QVector<double> res;

  for (int i=0; i<m_NIntTimes; i++) {
    res.append(m_IntTimes[i]);
  }

  return res;
}
