#ifndef QXRDPERKINELMERPLUGININTERFACE_H
#define QXRDPERKINELMERPLUGININTERFACE_H

#include <QtPlugin>
#include "qxrdnamedplugininterface.h"

#include <Windows.h>
#include <QSharedPointer>

#ifdef WIN64
#if WIN64
#define __X64
#endif
#endif

#ifdef __X64
#define ACQDATATYPE void*
#else
#define ACQDATATYPE DWORD
#endif

#include "Acq.h"

class QxrdPerkinElmerPluginInterface : public QxrdNamedPluginInterface {
public:
//  virtual ~QxrdPerkinElmerPluginInterface();

  virtual UINT Acquisition_SetCallbacksAndMessages(
      HACQDESC pAcqDesc, HWND hWnd, UINT dwErrorMsg,
      UINT dwLoosingFramesMsg, void (CALLBACK *lpfnEndFrameCallback)(HACQDESC),
      void (CALLBACK *lpfnEndAcqCallback)(HACQDESC)) = 0;

  virtual UINT Acquisition_EnumSensors(
      UINT *pdwNumSensors, BOOL bEnableIRQ, BOOL bAlwaysOpen) = 0;

  virtual UINT Acquisition_GetNextSensor(
      ACQDESCPOS *Pos, HACQDESC *phAcqDesc) = 0;

  virtual UINT Acquisition_SetAcqData(
      HACQDESC hAcqDesc, ACQDATATYPE AcqData) = 0;

  virtual UINT Acquisition_GetAcqData(
      HACQDESC hAcqDesc, ACQDATATYPE *AcqData) = 0;

  virtual UINT Acquisition_GetCommChannel(
      HACQDESC pAcqDesc, UINT *pdwChannelType, int *pnChannelNr) = 0;

  virtual UINT Acquisition_Close(HACQDESC hAcqDesc) = 0;

  virtual UINT Acquisition_CloseAll() = 0;

  virtual UINT Acquisition_DefineDestBuffers(
      HACQDESC pAcqDesc, unsigned short *pProcessedData,
      UINT nFrames, UINT nRows, UINT nColumns) = 0;

  virtual UINT Acquisition_Abort(HACQDESC hAcqDesc) = 0;

  virtual UINT Acquisition_Acquire_Image(
      HACQDESC pAcqDesc, UINT dwFrames, UINT dwSkipFrms,
      UINT dwOpt, unsigned short *pwOffsetData,
      DWORD *pdwGainData, DWORD *pdwPxlCorrList) = 0;

  virtual UINT Acquisition_GetConfiguration(
      HACQDESC hAcqDesc, UINT *dwFrames, UINT *dwRows, UINT *dwColumns, UINT *dwDataType,
      UINT *dwSortFlags, BOOL *bIRQEnabled, DWORD *dwAcqType, DWORD *dwSystemID,
      DWORD *dwSyncMode, DWORD *dwHwAccess) = 0;

  virtual UINT Acquisition_GetIntTimes(
      HACQDESC hAcqDesc, double *dblIntTime, int *nIntTimes) = 0;

  virtual UINT Acquisition_GetActFrame(
      HACQDESC hAcqDesc, DWORD *dwActAcqFrame, DWORD *dwActSecBuffFrame) = 0;

  virtual UINT Acquisition_ResetFrameCnt(HACQDESC hAcqDesc) = 0;

  virtual UINT Acquisition_GetHwHeaderInfo(
      HACQDESC hAcqDesc, CHwHeaderInfo *pInfo) = 0;

  virtual UINT Acquisition_SetFrameSyncMode(
      HACQDESC hAcqDesc, DWORD dwMode) = 0;

  virtual UINT Acquisition_SetTimerSync(
      HACQDESC hAcqDesc, DWORD *dwCycleTime) = 0;

  virtual UINT Acquisition_SetCameraGain(
      HACQDESC hAcqDesc, WORD wMode) = 0;

  virtual UINT Acquisition_SetCameraBinningMode(
      HACQDESC hAcqDesc, WORD wMode) = 0;

  virtual UINT Acquisition_GetCameraBinningMode(
      HACQDESC hAcqDesc, WORD* wMode) = 0;

  virtual UINT Acquisition_GetLatestFrameHeader(
      HACQDESC hAcqDesc, CHwHeaderInfo *pInfo, CHwHeaderInfoEx *pInfoEx) = 0;

  virtual UINT Acquisition_GetHwHeaderInfoEx(
      HACQDESC hAcqDesc, CHwHeaderInfo *pInfo, CHwHeaderInfoEx *pInfoEx) = 0;
};

Q_DECLARE_INTERFACE(QxrdPerkinElmerPluginInterface, "gov.anl.aps.cep.Qxrd.PerkinElmerInterface/1.0")

#endif // QXRDPERKINELMERPLUGININTERFACE_H
