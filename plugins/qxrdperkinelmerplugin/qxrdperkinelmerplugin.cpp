/******************************************************************
*
*  $Id: qxrdperkinelmerplugin.cpp,v 1.3 2010/09/23 19:57:32 jennings Exp $
*
*******************************************************************/

#include "qxrdperkinelmerplugin.h"

QxrdPerkinElmerPlugin::QxrdPerkinElmerPlugin()
{
}

QString QxrdPerkinElmerPlugin::name() const
{
  return "Perkin Elmer XRD Detector";
}

UINT QxrdPerkinElmerPlugin::Acquisition_SetCallbacksAndMessages(
    HACQDESC pAcqDesc, HWND hWnd, UINT dwErrorMsg,
    UINT dwLoosingFramesMsg, void (CALLBACK *lpfnEndFrameCallback)(HACQDESC),
    void (CALLBACK *lpfnEndAcqCallback)(HACQDESC))
{
  return ::Acquisition_SetCallbacksAndMessages(pAcqDesc, hWnd, dwErrorMsg,
                                              dwLoosingFramesMsg, lpfnEndFrameCallback,
                                              lpfnEndAcqCallback);
}

UINT QxrdPerkinElmerPlugin::Acquisition_EnumSensors(
    UINT *pdwNumSensors, BOOL bEnableIRQ, BOOL bAlwaysOpen)
{
  return ::Acquisition_EnumSensors(pdwNumSensors, bEnableIRQ, bAlwaysOpen);
}

UINT QxrdPerkinElmerPlugin::Acquisition_GetNextSensor(
    ACQDESCPOS *Pos, HACQDESC *phAcqDesc)
{
  return ::Acquisition_GetNextSensor(Pos, phAcqDesc);
}

UINT QxrdPerkinElmerPlugin::Acquisition_GetCommChannel(
    HACQDESC pAcqDesc, UINT *pdwChannelType, int *pnChannelNr)
{
  return ::Acquisition_GetCommChannel(pAcqDesc, pdwChannelType, pnChannelNr);
}

UINT QxrdPerkinElmerPlugin::Acquisition_DefineDestBuffers(
    HACQDESC pAcqDesc, unsigned short *pProcessedData,
    UINT nFrames, UINT nRows, UINT nColumns)
{
  return ::Acquisition_DefineDestBuffers(pAcqDesc, pProcessedData,
                                         nFrames, nRows, nColumns);
}

UINT QxrdPerkinElmerPlugin::Acquisition_Acquire_Image(
    HACQDESC pAcqDesc, UINT dwFrames, UINT dwSkipFrms,
    UINT dwOpt, unsigned short *pwOffsetData,
    DWORD *pdwGainData, DWORD *pdwPxlCorrList)
{
  return ::Acquisition_Acquire_Image(pAcqDesc, dwFrames, dwSkipFrms,
                                     dwOpt, pwOffsetData,
                                     pdwGainData, pdwPxlCorrList);
}

UINT QxrdPerkinElmerPlugin::Acquisition_GetConfiguration(
    HACQDESC hAcqDesc, UINT *dwFrames, UINT *dwRows, UINT *dwColumns, UINT *dwDataType,
    UINT *dwSortFlags, BOOL *bIRQEnabled, DWORD *dwAcqType, DWORD *dwSystemID,
    DWORD *dwSyncMode, DWORD *dwHwAccess)
{
  return ::Acquisition_GetConfiguration(hAcqDesc, dwFrames, dwRows, dwColumns, dwDataType,
                                        dwSortFlags, bIRQEnabled, dwAcqType, dwSystemID,
                                        dwSyncMode, dwHwAccess);
}

UINT QxrdPerkinElmerPlugin::Acquisition_GetIntTimes(
    HACQDESC hAcqDesc, double *dblIntTime, int *nIntTimes)
{
  return ::Acquisition_GetIntTimes(hAcqDesc, dblIntTime, nIntTimes);
}

UINT QxrdPerkinElmerPlugin::Acquisition_GetActFrame(
    HACQDESC hAcqDesc, DWORD *dwActAcqFrame, DWORD *dwActSecBuffFrame)
{
  return ::Acquisition_GetActFrame(hAcqDesc, dwActAcqFrame, dwActSecBuffFrame);
}

UINT QxrdPerkinElmerPlugin::Acquisition_GetHwHeaderInfo(
    HACQDESC hAcqDesc, CHwHeaderInfo *pInfo)
{
  return ::Acquisition_GetHwHeaderInfo(hAcqDesc, pInfo);
}

UINT QxrdPerkinElmerPlugin::Acquisition_SetFrameSyncMode(
    HACQDESC hAcqDesc, DWORD dwMode)
{
  return ::Acquisition_SetFrameSyncMode(hAcqDesc, dwMode);
}

UINT QxrdPerkinElmerPlugin::Acquisition_SetTimerSync(
    HACQDESC hAcqDesc, DWORD *dwCycleTime)
{
  return ::Acquisition_SetTimerSync(hAcqDesc, dwCycleTime);
}

UINT QxrdPerkinElmerPlugin::Acquisition_SetCameraGain(
    HACQDESC hAcqDesc, WORD wMode)
{
  return ::Acquisition_SetCameraGain(hAcqDesc, wMode);
}

UINT QxrdPerkinElmerPlugin::Acquisition_SetCameraBinningMode(
    HACQDESC hAcqDesc, WORD wMode)
{
  return ::Acquisition_SetCameraBinningMode(hAcqDesc, wMode);
}

UINT QxrdPerkinElmerPlugin::Acquisition_GetCameraBinningMode(
    HACQDESC hAcqDesc, WORD* wMode)
{
  return ::Acquisition_GetCameraBinningMode(hAcqDesc, wMode);
}

UINT QxrdPerkinElmerPlugin::Acquisition_GetHwHeaderInfoEx(
    HACQDESC hAcqDesc, CHwHeaderInfo *pInfo, CHwHeaderInfoEx *pInfoEx)
{
  return ::Acquisition_GetHwHeaderInfoEx(hAcqDesc, pInfo, pInfoEx);
}

Q_EXPORT_PLUGIN2(qxrdperkinelmerplugin, QxrdPerkinElmerPlugin);

/******************************************************************
*
*  $Log: qxrdperkinelmerplugin.cpp,v $
*  Revision 1.3  2010/09/23 19:57:32  jennings
*  Modified plugins for perkin elmer - now works in 64 bit mode
*
*  Revision 1.2  2010/09/13 20:00:17  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/07/23 19:20:24  jennings
*  Plugins now working
*
*
*******************************************************************/
