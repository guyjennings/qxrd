#include <QObject>


#ifdef Q_OS_UNIX
#include "AcqLinuxTypes.h"
#endif

#include "Acq.h"

/*
  Dummy version of the PE xisl library for development purposes.
*/

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
  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_Abort(HACQDESC hAcqDesc)
{
  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_DefineDestBuffers(HACQDESC pAcqDesc, unsigned short *pProcessedData, UINT nFrames, UINT nRows, UINT nColumns)
{
  return HIS_ALL_OK;
}

HIS_RETURN Acquisition_Acquire_Image(HACQDESC pAcqDesc, UINT dwFrames, UINT dwSkipFrms, UINT dwOpt, unsigned short *pwOffsetData, DWORD *pdwGainData, DWORD *pdwPxlCorrList)
{
  return HIS_ALL_OK;
}
