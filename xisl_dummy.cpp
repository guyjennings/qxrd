/******************************************************************
*
*  $Id: xisl_dummy.cpp,v 1.11 2009/06/28 16:33:20 jennings Exp $
*
*******************************************************************/

#include <QObject>


#ifdef Q_OS_UNIX
#include "AcqLinuxTypes.h"
#endif

#include "Acq.h"

#include <QTimer>
#include "xisl_dummy.h"

/*
  Dummy version of the PE xisl library for development purposes.
*/

static HACQDESC acqDesc;
static void (*endFrameCallback)(HACQDESC hAcqDesc) = NULL;
static void (*endAcqCallback)(HACQDESC hAcqDesc) = NULL;
static int nFrames = 0;
static int options = 0;
static int continuous = 0;

static AcquisitionTimer timer;

AcquisitionTimer::AcquisitionTimer()
  : QObject(NULL),
    m_Mode(0),
    SOURCE_IDENT("$Id: xisl_dummy.cpp,v 1.11 2009/06/28 16:33:20 jennings Exp $")
{
  connect(&m_Timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

void AcquisitionTimer::start()
{
//   printf("AcquisitionTimer::start(), m_Mode = %d\n", m_Mode);

  switch (m_Mode) {
  case 0:
    m_Timer.start(67);
    break;
  case 1:
    m_Timer.start(83);
    break;
  case 2:
    m_Timer.start(100);
    break;
  case 3:
    m_Timer.start(125);
    break;
  case 4:
    m_Timer.start(167);
    break;
  case 5:
    m_Timer.start(250);
    break;
  case 6:
    m_Timer.start(500);
    break;
  case 7:
    m_Timer.start(1000);
    break;
  default:
    break;
  };
}

void AcquisitionTimer::stop()
{
//   printf("AcquisitionTimer::stop()\n");

  m_Timer.stop();
}

void AcquisitionTimer::setmode(int mode)
{
//   printf("AcquisitionTimer::setmode(%d)\n",mode);

  m_Mode = mode;
}

HIS_RETURN Acquisition_EnumSensors(UINT */*pdwNumSensors*/, BOOL /*bEnableIRQ*/, BOOL /*bAlwaysOpen*/)
{
  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_GetNextSensor(ACQDESCPOS */*Pos*/,  HACQDESC */*phAcqDesc*/)
{
  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_GetCommChannel(HACQDESC /*pAcqDesc*/, UINT */*pdwChannelType*/, int */*pnChannelNr*/)
{
  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_SetFrameSync(HACQDESC /*hAcqDesc*/)
{
  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_SetFrameSyncMode(HACQDESC /*hAcqDesc*/, DWORD /*dwMode*/)
{
  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_SetTimerSync(HACQDESC /*hAcqDesc*/, DWORD */*dwCycleTime*/)
{
  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_SetCameraGain(HACQDESC /*hAcqDesc*/, WORD /*wMode*/)
{
  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_GetConfiguration(HACQDESC /*hAcqDesc*/,
                                        UINT */*dwFrames*/, UINT *dwRows, UINT *dwColumns, UINT */*dwDataType*/,
                                        UINT */*dwSortFlags*/, BOOL */*bIRQEnabled*/, DWORD */*dwAcqType*/, DWORD */*dwSystemID*/,
                                        DWORD */*dwSyncMode*/, DWORD */*dwHwAccess*/)
{
  if (dwRows) *dwRows = 2048;
  if (dwColumns) *dwColumns = 2048;

  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_SetCallbacksAndMessages(HACQDESC pAcqDesc,
                                               HWND /*hWnd*/,
                                               UINT /*dwErrorMsg*/, UINT /*dwLoosingFramesMsg*/,
					       void (CALLBACK *lpfnEndFrameCallback)(HACQDESC), 
					       void (CALLBACK *lpfnEndAcqCallback)(HACQDESC)
					       )
{
  endFrameCallback = lpfnEndFrameCallback;
  endAcqCallback = lpfnEndAcqCallback;
  acqDesc = pAcqDesc;

  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_Abort(HACQDESC /*hAcqDesc*/)
{
  continuous = 0;

  timer.stop();

  if (endAcqCallback) {
    endAcqCallback(acqDesc);
  }

  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_DefineDestBuffers(HACQDESC /*pAcqDesc*/, unsigned short */*pProcessedData*/, UINT /*nFrames*/, UINT /*nRows*/, UINT /*nColumns*/)
{
  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_Acquire_Image(HACQDESC /*pAcqDesc*/, UINT dwFrames, UINT /*dwSkipFrms*/, UINT dwOpt, unsigned short */*pwOffsetData*/, DWORD */*pdwGainData*/, DWORD */*pdwPxlCorrList*/)
{
//   printf("Acquisition_Acquire_Image dwOpt = %d\n", dwOpt);

  nFrames = dwFrames;
  options = dwOpt;

  if (dwOpt == HIS_SEQ_CONTINUOUS) {
    continuous = 1;

    timer.start();
  }

  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_SetCameraMode(HACQDESC /*hAcqDesc*/, UINT dwMode)
{
  timer.setmode(dwMode);

  return HIS_ALL_OK;
}

void AcquisitionTimer::timeout()
{
  if (endFrameCallback) {
    endFrameCallback(acqDesc);
  }

  if (continuous) {
    timer.start();
  } else {
    if (endAcqCallback) {
      endAcqCallback(acqDesc);
    }
  }
}

HIS_RETURN Acquisition_GetIntTimes(HACQDESC /*hAcqDesc*/, double *dblIntTime, int *nIntTimes)
{
  if (nIntTimes) *nIntTimes = 8;

  dblIntTime[0] = 66577.8;
  dblIntTime[1] = 82755.6;
  dblIntTime[2] = 99928.9;
  dblIntTime[3] = 124942;
  dblIntTime[4] = 166631;
  dblIntTime[5] = 249884;
  dblIntTime[6] = 499893;
  dblIntTime[7] = 999911;

  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_GetActFrame(HACQDESC /*hAcqDesc*/, DWORD *dwActAcqFrame, DWORD *dwActSecBuffFrame)
{
  if (dwActAcqFrame) *dwActAcqFrame = 42;
  if (dwActSecBuffFrame) *dwActSecBuffFrame=89;

  return HIS_ALL_OK;
}

/******************************************************************
*
*  $Log: xisl_dummy.cpp,v $
*  Revision 1.11  2009/06/28 16:33:20  jennings
*  Eliminated compiler warnings
*
*  Revision 1.10  2009/06/27 22:50:33  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

