#include "qxrdacquisition.h"
#include "qxrdacquisitionthread.h"
#include "qxrdapplication.h"

#ifdef Q_OS_UNIX
#include "AcqLinuxTypes.h"
#endif

#include "Acq.h"
#include <QThread>

QxrdAcquisition::QxrdAcquisition(QxrdApplication *app, QxrdAcquisitionThread *thread)
  : QObject(),
    m_Thread(thread),
    m_Mutex(QMutex::Recursive),
    m_Application(app),
    m_Cancel(false)
{
  printf("Enter QxrdAcquisition::QxrdAcquisition\n");
}

QxrdAcquisition::~QxrdAcquisition()
{
}

static void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc);
static void CALLBACK OnEndAcqCallback(HACQDESC hAcqDesc);

void QxrdAcquisition::initialize()
{
  printf("QxrdAcquisition::initialize()\n");

  HIS_RETURN nRet;
  UINT nSensors;
  BOOL bEnableIRQ = true;
  ACQDESCPOS Pos = 0;
  HACQDESC hAcqDesc = NULL;
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

  if ((nRet = Acquisition_GetNextSensor(&Pos, &hAcqDesc))!=HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  if ((nRet=Acquisition_GetCommChannel(hAcqDesc, &nChannelType, &nChannelNr))!=HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  if ((nRet=Acquisition_GetConfiguration(hAcqDesc, &dwFrames, &dwRows, &dwColumns, &dwDataType, 
					 &dwSortFlags, &bEnableIRQ, &dwAcqType, 
					 &dwSystemID, &dwSyncMode, &dwHwAccess))!=HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  emit printMessage(tr("channel type: %1, ChannelNr: %2\n").arg(nChannelType).arg(nChannelNr));
  emit printMessage(tr("frames: %1\n").arg(dwFrames));
  emit printMessage(tr("rows: %1\ncolumns: %2\n").arg(dwRows).arg(dwColumns));
  
  if ((nRet=Acquisition_SetCallbacksAndMessages(hAcqDesc, NULL, 0,
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

    for (int i=0; i<nframes; i++) {
      emit printMessage(tr("Starting frame %1...\n").arg(i));

      for (int j=0; j<nsum; j++) {
	emit printMessage(tr("Starting sub-exposure %1...\n").arg(j));

	m_Thread -> msleep(integ*1000);

	if (m_Cancel) {
	  return;
	}
      }
    }
    
    emit acquireComplete();
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
  m_Cancel = true;
}

static void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc)
{
}

static void CALLBACK OnEndAcqCallback(HACQDESC hAcqDesc)
{
}

