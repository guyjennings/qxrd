#include <QObject>


#ifdef Q_OS_UNIX
#include "AcqLinuxTypes.h"
#endif

#include "Acq.h"

/*
  Dummy version of the PE xisl library for development purposes.
*/

static void (*endFrameCallback)(HACQDESC hAcqDesc) = NULL;
static void (*endAcqCallback)(HACQDESC hAcqDesc) = NULL;
static int nFrames = 0;
static int options = 0;
static int continuous = 0;

HIS_RETURN Acquisition_EnumSensors(UINT *pdwNumSensors, BOOL bEnableIRQ, BOOL bAlwaysOpen)
{
  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_GetNextSensor(ACQDESCPOS *Pos,  HACQDESC *phAcqDesc)
{
  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_GetCommChannel(HACQDESC pAcqDesc, UINT *pdwChannelType, int *pnChannelNr)
{
  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_GetConfiguration(HACQDESC hAcqDesc, 
					UINT *dwFrames, UINT *dwRows, UINT *dwColumns, UINT *dwDataType,
					UINT *dwSortFlags, BOOL *bIRQEnabled, DWORD *dwAcqType, DWORD *dwSystemID,
					DWORD *dwSyncMode, DWORD *dwHwAccess)
{
  if (dwRows) *dwRows = 2048;
  if (dwColumns) *dwColumns = 2048;

  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_SetCallbacksAndMessages(HACQDESC pAcqDesc,
					       HWND hWnd,
					       UINT dwErrorMsg, UINT dwLoosingFramesMsg,
					       void (CALLBACK *lpfnEndFrameCallback)(HACQDESC), 
					       void (CALLBACK *lpfnEndAcqCallback)(HACQDESC)
					       )
{
  endFrameCallback = lpfnEndFrameCallback;
  endAcqCallback = lpfnEndAcqCallback;

  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_Abort(HACQDESC hAcqDesc)
{
  continuous = 0;

  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_DefineDestBuffers(HACQDESC pAcqDesc, unsigned short *pProcessedData, UINT nFrames, UINT nRows, UINT nColumns)
{
  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_Acquire_Image(HACQDESC pAcqDesc, UINT dwFrames, UINT dwSkipFrms, UINT dwOpt, unsigned short *pwOffsetData, DWORD *pdwGainData, DWORD *pdwPxlCorrList)
{
  nFrames = dwFrames;
  options = dwOpt;

  if (dwOpt == HIS_SEQ_CONTINUOUS) {
    continuous = 1;

    while(continuous) {
      sleep(1);
      if (endFrameCallback) {
	endFrameCallback(pAcqDesc);
      }
    }
  }

  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_SetCameraMode(HACQDESC hAcqDesc, UINT dwMode)
{
  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_GetIntTimes(HACQDESC hAcqDesc, double *dblIntTime, int *nIntTimes)
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

HIS_RETURN Acquisition_GetActFrame(HACQDESC hAcqDesc, DWORD *dwActAcqFrame, DWORD *dwActSecBuffFrame)
{
  if (dwActAcqFrame) *dwActAcqFrame = 42;
  if (dwActSecBuffFrame) *dwActSecBuffFrame=89;
}
