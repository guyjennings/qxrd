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

static QxrdAcquisition *g_Acquisition = NULL;

QxrdAcquisition::QxrdAcquisition(QxrdApplication *app, QxrdAcquisitionThread *thread)
  : QObject(),
    m_Thread(thread),
    m_Mutex(QMutex::Recursive),
    m_Application(app),
    m_Cancel(false),
    m_NRows(0),
    m_NCols(0),
    m_Exposure(0),
    m_NSums(0),
    m_NFrames(0),
    m_NBufferFrames(0)
{
  printf("Enter QxrdAcquisition::QxrdAcquisition\n");

  g_Acquisition = this;
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

  emit printMessage(tr("channel type: %1, ChannelNr: %2\n").arg(nChannelType).arg(nChannelNr));
  emit printMessage(tr("frames: %1\n").arg(dwFrames));
  emit printMessage(tr("rows: %1\ncolumns: %2\n").arg(dwRows).arg(dwColumns));
  
  if ((nRet=Acquisition_SetCallbacksAndMessages(m_AcqDesc, NULL, 0,
						0, OnEndFrameCallback, OnEndAcqCallback))!=HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }
}

void QxrdAcquisition::acquire(double integ, int nsum, int nframes)
{
  {
    QMutexLocker lock(&m_Mutex);

    emit printMessage(tr("QxrdAcquisition::acquire(%1,%2,%3)\n").arg(integ).arg(nsum).arg(nframes));

    int nRet = HIS_ALL_OK;

    m_Exposure = integ;
    m_NSums = nsum;
    m_NFrames = nframes;
    m_NBufferFrames = 10;

    m_AcquiredImage.resize(m_NRows*m_NCols*m_NFrames);
    m_Buffer.resize(m_NRows*m_NCols*m_NBufferFrames);

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

//     for (int i=0; i<nframes; i++) {
//       emit printMessage(tr("Starting frame %1...\n").arg(i));

//       for (int j=0; j<nsum; j++) {
// 	emit printMessage(tr("Starting sub-exposure %1...\n").arg(j));

// 	m_Thread -> msleep(integ*1000);

// 	if (m_Cancel) {
// 	  return;
// 	}
//       }
//     }
    
//     emit acquireComplete();
//     m_Acquisition -> setNAverages((int)(integ*1000));
//     m_Acquisition -> setNRepetitions(nreps);
  }

//   configureAverager();

//   for (int i=0; i<nreps; i++) {
//     startAcquisition(i);

//     waitAcquisition(i);
//     readoutAcquisition(i);
//   }
}

void QxrdAcquisition::onEndFrame()
{
  emit printMessage("Frame ended\n");
}

void QxrdAcquisition::onEndAcquisition()
{
  emit printMessage("Acquisition ended\n");
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

void QxrdAcquisition::cancel()
{
  Acquisition_Abort(m_AcqDesc);

  m_Cancel = true;
}

static void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc)
{
  g_Acquisition -> onEndFrame();
}

static void CALLBACK OnEndAcqCallback(HACQDESC hAcqDesc)
{
  g_Acquisition -> onEndAcquisition();
}

