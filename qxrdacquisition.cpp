#include "qxrdacquisition.h"
#include "qxrdacquisitionthread.h"
#include "qxrdapplication.h"

#ifdef Q_OS_UNIX
#include "AcqLinuxTypes.h"
#endif

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

#include "Acq.h"
#include <QThread>
#include <QDir>
#include <QFile>
#include <QtConcurrentRun>

static QxrdAcquisition *g_Acquisition = NULL;

QxrdAcquisition::QxrdAcquisition(QxrdApplication *app, QxrdAcquisitionThread *thread)
  : QObject(),
    m_Thread(thread),
    m_Mutex(QMutex::Recursive),
    m_Application(app),
    //m_Cancel(false),
    m_NRows(0),
    m_NCols(0),
    m_IntegMode(0),
    m_NSums(0),
    m_NFrames(0),
    m_NBufferFrames(0),
    m_NIntTimes(0)
{
  printf("Enter QxrdAcquisition::QxrdAcquisition\n");

  g_Acquisition = this;

  connect(this, SIGNAL(haltAcquire()), this, SLOT(_haltAcquire()), Qt::QueuedConnection);
}

QxrdAcquisition::~QxrdAcquisition()
{
}

static void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc);
static void CALLBACK OnEndAcqCallback(HACQDESC hAcqDesc);

static HACQDESC m_AcqDesc = NULL;

void QxrdAcquisition::initialize()
{
  printf("QxrdAcquisition::initialize()\n");

  int nRet = HIS_ALL_OK;
  UINT nSensors;
  BOOL bEnableIRQ = true;
  ACQDESCPOS Pos = 0;
  UINT nChannelType;
  int nChannelNr;
  UINT dwSortFlags, dwDataType, dwRows, dwColumns, dwFrames;
  DWORD 
    dwAcqType, dwSystemID, dwSyncMode, dwHwAccess,
    dwNumSensors;
  char strBuffer[1000];

  nRet = Acquisition_EnumSensors(&nSensors, bEnableIRQ, FALSE);

  if (nRet != HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

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
}

void QxrdAcquisition::acquire(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum, int nframes)
{
  {
//     QMutexLocker lock(&m_Mutex);

    if (nsum <= 0) nsum = 1;
    if (nframes <= 0) nframes = 1;

    emit printMessage(tr("QxrdAcquisition::acquire(\"%1\",\"%2\",%3,%4,%5,%6)\n")
                      .arg(outDir).arg(filePattern).arg(fileIndex).arg(integmode).arg(nsum).arg(nframes));
    emit statusMessage("Starting acquisition");

    int nRet = HIS_ALL_OK;

    m_OutputDir   = outDir;
    m_FilePattern = filePattern;
    m_FileIndex = fileIndex;
    m_IntegMode = integmode;
    m_NSums = nsum;
    m_NFrames = nframes;
    m_NBufferFrames = 10;
    m_BufferFrame = 0;

    m_AcquiredImage.resize(m_NRows*m_NCols*m_NFrames);
    m_AcquiredImage.fill(0);
    m_Buffer.resize(m_NRows*m_NCols*m_NBufferFrames);
    m_Buffer.fill(0);
    m_Saved.resize(m_NFrames);
    m_Saved.fill(QFuture<int>());

    if ((nRet=Acquisition_SetCallbacksAndMessages(m_AcqDesc, NULL, 0,
						  0, OnEndFrameCallback, OnEndAcqCallback))!=HIS_ALL_OK) {
      acquisitionError(nRet);
      return;
    }

    if ((nRet=Acquisition_SetCameraMode(m_AcqDesc, integmode)) != HIS_ALL_OK) {
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
  }
}

void QxrdAcquisition::onEndFrame()
{
  QString fileName = QDir(m_OutputDir).filePath(m_FilePattern+tr("%1").arg(m_FileIndex,5,10,QChar('0')));

  emit acquiredFrame(fileName, m_FileIndex, m_CurrentSum,m_NSums, m_CurrentFrame, m_NFrames);
  // sum current frame

  long npixels = m_NRows*m_NCols;
  double* current = m_AcquiredImage.data();
  unsigned short* frame = m_Buffer.data() + m_BufferFrame*npixels;
  unsigned short max=0;

  for (long i=0; i<npixels; i++) {
    *current += *frame;
    current++; frame++;
  }

  m_CurrentSum++;
  m_BufferFrame++;

  if (m_BufferFrame >= m_NBufferFrames) {
    m_BufferFrame = 0;
  }

  if (m_CurrentSum >= m_NSums) {
    m_CurrentSum = 0;

    m_Saved[m_CurrentFrame] =
        QtConcurrent::run(this, &QxrdAcquisition::saveAcquiredFrame, fileName, m_CurrentFrame);
    emit statusMessage("Saving "+fileName);
    emit printMessage("Saving """+fileName+"""");
    emit summedFrameCompleted(fileName, m_CurrentFrame);

    m_FileIndex++;
    emit fileIndexChanged(m_FileIndex);
    m_CurrentFrame++;

    if (m_CurrentFrame >= m_NFrames) {
      emit printMessage("Acquisition ended\n");
      emit haltAcquire();
      emit printMessage("Aborted acquisition\n");
    }
  }
}

void QxrdAcquisition::onEndAcquisition()
{
  emit printMessage("(CB) Acquisition ended\n");

  QFuture<int> f;

  emit statusMessage("Waiting for saves");

  foreach(f, m_Saved) {
    f.waitForFinished();
  }

  emit statusMessage("Acquire Complete");
  emit acquireComplete();
}

void QxrdAcquisition::resultsAvailable(int chan)
{
  emit resultsChanged();
}

void QxrdAcquisition::savingComplete(int chan)
{
}

void QxrdAcquisition::acquisitionError(int n)
{
  
}

void QxrdAcquisition::_haltAcquire()
{
    Acquisition_Abort(m_AcqDesc);

    emit acquireComplete();
}

void QxrdAcquisition::cancel()
{
  _haltAcquire();

  //m_Cancel = true;
}

static void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc)
{
  g_Acquisition -> onEndFrame();
}

static void CALLBACK OnEndAcqCallback(HACQDESC hAcqDesc)
{
  g_Acquisition -> onEndAcquisition();
}

QVector<double> QxrdAcquisition::integrationTimes()
{
  QVector<double> res;

  for (int i=0; i<m_NIntTimes; i++) {
    res.append(m_IntTimes[i]);
  }

  return res;
}

void QxrdAcquisition::saveData(QString name)
{
  double* current = m_AcquiredImage.data();
  long npixels = m_NRows*m_NCols;

  QFile outfile(name);
  outfile.open(QIODevice::ReadWrite);
  outfile.write((const char*) current, npixels*sizeof(double));
}

int QxrdAcquisition::saveAcquiredFrame(QString name, int frame)
{
  long npixels = m_NRows*m_NCols;
  double* current = m_AcquiredImage.data()+frame*npixels;

  QFile outfile(name);
  outfile.open(QIODevice::ReadWrite);
  outfile.write((const char*) current, npixels*sizeof(double));

  return 1;
}

QxrdRasterData QxrdAcquisition::imageRaster(int iframe)
{
  if (iframe >= 0 && iframe < m_CurrentFrame) {
    return QxrdRasterData(m_AcquiredImage, iframe*m_NRows*m_NCols, m_NRows, m_NCols);
  } else {
    return QxrdRasterData(m_AcquiredImage, 0, m_NRows, m_NCols);
  }
}
