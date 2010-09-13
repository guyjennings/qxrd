/******************************************************************
*
*  $Id: qxrdxislinterface.cpp,v 1.2 2010/09/13 20:00:42 jennings Exp $
*
*******************************************************************/

#include "qxrdxislinterface.h"

//_Acquisition_Acquire_Image@28(m_AcqDesc, m_BufferSize,
//_Acquisition_SetTimerSync@8 (m_AcqDesc, &tmp)) != HIS_ALL_OK) {
//_Acquisition_SetCameraBinningMode@8(m_AcqDesc, binningMode)) != HIS_ALL_OK) {
//_Acquisition_GetCameraBinningMode@8(m_AcqDesc, &binningMode)) != HIS_ALL_OK) {
//_Acquisition_SetCameraGain@8(m_AcqDesc, get_CameraGain())) != HIS_ALL_OK) {
//_Acquisition_EnumSensors@12(&nSensors, bEnableIRQ, FALSE);
//_Acquisition_GetNextSensor@8(&Pos, &m_AcqDesc))!=HIS_ALL_OK) {
//_Acquisition_GetCommChannel@12(m_AcqDesc, &nChannelType, &nChannelNr))!=HIS_ALL_OK) {
//_Acquisition_GetConfiguration@44(m_AcqDesc, &dwFrames, &dwRows, &dwColumns, &dwDataType,
//_Acquisition_GetHwHeaderInfo@8(m_AcqDesc, &hwHeaderInfo)) != HIS_ALL_OK) {
//_Acquisition_GetHwHeaderInfoEx@12(m_AcqDesc, &hwHeaderInfo, &hdrx)) != HIS_ALL_OK) {
//_Acquisition_GetIntTimes@12(m_AcqDesc, readoutTimes, &nReadoutTimes)) != HIS_ALL_OK) {
//_Acquisition_SetCallbacksAndMessages@24(m_AcqDesc, NULL, 0,
//_Acquisition_SetFrameSyncMode@8(m_AcqDesc, HIS_SYNCMODE_INTERNAL_TIMER)) != HIS_ALL_OK) {
//_Acquisition_DefineDestBuffers@20(m_AcqDesc, m_Buffer.data(), m_BufferSize,
//_Acquisition_GetActFrame@12(m_AcqDesc, &actualFrame, &actSecFrame);

#define INITIALIZE_XISL_ROUTINE(fname,suffix) \
m_##fname = (xisldll ? ((p##fname) GetProcAddress(xisldll,"_" #fname suffix)) : NULL);

#define IMPLEMENT_XISL_ROUTINE(fname,args,call) \
UINT QxrdXISLInterface::fname args { if (m_##fname) { return (*m_##fname)call; } else { return NULL; } }

QxrdXISLInterface::QxrdXISLInterface() :
    SOURCE_IDENT("$id$")
{
  HINSTANCE xisldll;
  xisldll = LoadLibrary(TEXT("xisl.dll"));

  INITIALIZE_XISL_ROUTINE(Acquisition_SetCallbacksAndMessages,"@24");
  INITIALIZE_XISL_ROUTINE(Acquisition_EnumSensors, "@12");
  INITIALIZE_XISL_ROUTINE(Acquisition_GetNextSensor, "@8");
  INITIALIZE_XISL_ROUTINE(Acquisition_GetCommChannel, "@12");
  INITIALIZE_XISL_ROUTINE(Acquisition_DefineDestBuffers, "@20");
  INITIALIZE_XISL_ROUTINE(Acquisition_Acquire_Image, "@28");
  INITIALIZE_XISL_ROUTINE(Acquisition_GetConfiguration, "@44");
  INITIALIZE_XISL_ROUTINE(Acquisition_GetIntTimes, "@12");
  INITIALIZE_XISL_ROUTINE(Acquisition_GetActFrame, "@12");
  INITIALIZE_XISL_ROUTINE(Acquisition_GetHwHeaderInfo, "@8");
  INITIALIZE_XISL_ROUTINE(Acquisition_SetFrameSyncMode, "@8");
  INITIALIZE_XISL_ROUTINE(Acquisition_SetTimerSync, "@8");
  INITIALIZE_XISL_ROUTINE(Acquisition_SetCameraGain, "@8");
  INITIALIZE_XISL_ROUTINE(Acquisition_SetCameraBinningMode, "@8");
  INITIALIZE_XISL_ROUTINE(Acquisition_GetCameraBinningMode, "@8");
  INITIALIZE_XISL_ROUTINE(Acquisition_GetHwHeaderInfoEx, "@12");
}

IMPLEMENT_XISL_ROUTINE(Acquisition_SetCallbacksAndMessages,
                       (HACQDESC pAcqDesc, HWND hWnd,
                        UINT dwErrorMsg, UINT dwLoosingFramesMsg,
                        void (CALLBACK *lpfnEndFrameCallback)(HACQDESC),
                        void (CALLBACK *lpfnEndAcqCallback)(HACQDESC)
                        ),
                       (pAcqDesc, hWnd, dwErrorMsg, dwLoosingFramesMsg,
                        lpfnEndFrameCallback, lpfnEndAcqCallback)
                       );

IMPLEMENT_XISL_ROUTINE(Acquisition_EnumSensors,
                       (UINT *pdwNumSensors, BOOL bEnableIRQ, BOOL bAlwaysOpen),
                       (pdwNumSensors, bEnableIRQ, bAlwaysOpen)
                       );

IMPLEMENT_XISL_ROUTINE(Acquisition_GetNextSensor,
                       (ACQDESCPOS *Pos, HACQDESC *phAcqDesc),
                       (Pos, phAcqDesc)
                       );

IMPLEMENT_XISL_ROUTINE(Acquisition_GetCommChannel,
                       (HACQDESC pAcqDesc, UINT *pdwChannelType, int *pnChannelNr),
                       (pAcqDesc, pdwChannelType, pnChannelNr)
                       );

IMPLEMENT_XISL_ROUTINE(Acquisition_DefineDestBuffers,
                       (HACQDESC pAcqDesc, unsigned short *pProcessedData, UINT nFrames, UINT nRows, UINT nColumns),
                       (pAcqDesc, pProcessedData, nFrames, nRows, nColumns)
                       );

IMPLEMENT_XISL_ROUTINE(Acquisition_Acquire_Image,
                       (HACQDESC pAcqDesc, UINT dwFrames, UINT dwSkipFrms,
                        UINT dwOpt, unsigned short *pwOffsetData,
                        DWORD *pdwGainData, DWORD *pdwPxlCorrList),
                       (pAcqDesc, dwFrames, dwSkipFrms,
                        dwOpt, pwOffsetData, pdwGainData, pdwPxlCorrList)
                       );

IMPLEMENT_XISL_ROUTINE(Acquisition_GetConfiguration,
                       (HACQDESC hAcqDesc,
                        UINT *dwFrames, UINT *dwRows, UINT *dwColumns, UINT *dwDataType,
                        UINT *dwSortFlags, BOOL *bIRQEnabled, DWORD *dwAcqType, DWORD *dwSystemID,
                        DWORD *dwSyncMode, DWORD *dwHwAccess),
                       (hAcqDesc, dwFrames, dwRows, dwColumns, dwDataType,
                        dwSortFlags, bIRQEnabled, dwAcqType, dwSystemID,
                        dwSyncMode, dwHwAccess)
                       );

IMPLEMENT_XISL_ROUTINE(Acquisition_GetIntTimes,
                       (HACQDESC hAcqDesc, double *dblIntTime, int *nIntTimes),
                       (hAcqDesc, dblIntTime, nIntTimes)
                       );

IMPLEMENT_XISL_ROUTINE(Acquisition_GetActFrame,
                       (HACQDESC hAcqDesc, DWORD *dwActAcqFrame, DWORD *dwActSecBuffFrame),
                       (hAcqDesc, dwActAcqFrame, dwActSecBuffFrame)
                       );

IMPLEMENT_XISL_ROUTINE(Acquisition_GetHwHeaderInfo,
                       (HACQDESC hAcqDesc, CHwHeaderInfo *pInfo),
                       (hAcqDesc, pInfo)
                       );

IMPLEMENT_XISL_ROUTINE(Acquisition_SetFrameSyncMode,
                       (HACQDESC hAcqDesc, DWORD dwMode),
                       (hAcqDesc, dwMode)
                       );

IMPLEMENT_XISL_ROUTINE(Acquisition_SetTimerSync,
                       (HACQDESC hAcqDesc, DWORD *dwCycleTime),
                       (hAcqDesc, dwCycleTime)
                       );

IMPLEMENT_XISL_ROUTINE(Acquisition_SetCameraGain,
                       (HACQDESC hAcqDesc, WORD wMode),
                       (hAcqDesc, wMode)
                       );

IMPLEMENT_XISL_ROUTINE(Acquisition_SetCameraBinningMode,
                       (HACQDESC hAcqDesc, WORD wMode),
                       (hAcqDesc, wMode)
                       );

IMPLEMENT_XISL_ROUTINE(Acquisition_GetCameraBinningMode,
                       (HACQDESC hAcqDesc, WORD* wMode),
                       (hAcqDesc, wMode)
                       );

IMPLEMENT_XISL_ROUTINE(Acquisition_GetHwHeaderInfoEx,
                       (HACQDESC hAcqDesc, CHwHeaderInfo *pInfo ,CHwHeaderInfoEx *pInfoEx),
                       (hAcqDesc, pInfo, pInfoEx)
                       );

/******************************************************************
*
*  $Log: qxrdxislinterface.cpp,v $
*  Revision 1.2  2010/09/13 20:00:42  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:42  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.2  2010/06/07 20:17:42  jennings
*  Added more cvs log and ident stuff - removed qxrdspecserver.cpp & qxrdspecserver.h
*
*  Revision 1.1.2.1  2010/05/26 19:20:43  jennings
*  Modified interface to XISL.dll so that it is accessed via 'LoadLibrary' and
*  'GetProcAddress' rather than being explicitly linked.  Should allow
*  qxrd to run if the PE libraries are not present.
*
*
*******************************************************************/
