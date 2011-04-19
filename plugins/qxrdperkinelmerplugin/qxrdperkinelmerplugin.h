#ifndef QXRDPERKINELMERPLUGIN_H
#define QXRDPERKINELMERPLUGIN_H

#ifdef HAVE_PERKIN_ELMER

#include <QObject>
#include "qxrdperkinelmerplugininterface.h"

class QxrdPerkinElmerPlugin : public QObject, public QxrdPerkinElmerPluginInterface
{
  Q_OBJECT;
  Q_INTERFACES(QxrdPerkinElmerPluginInterface);

public:
  QxrdPerkinElmerPlugin();

  QString name() const;

  virtual UINT Acquisition_SetCallbacksAndMessages(
      HACQDESC pAcqDesc, HWND hWnd, UINT dwErrorMsg,
      UINT dwLoosingFramesMsg, void (CALLBACK *lpfnEndFrameCallback)(HACQDESC),
      void (CALLBACK *lpfnEndAcqCallback)(HACQDESC));

  virtual UINT Acquisition_EnumSensors(
      UINT *pdwNumSensors, BOOL bEnableIRQ, BOOL bAlwaysOpen);

  virtual UINT Acquisition_GetNextSensor(
      ACQDESCPOS *Pos, HACQDESC *phAcqDesc);

  virtual UINT Acquisition_GetCommChannel(
      HACQDESC pAcqDesc, UINT *pdwChannelType, int *pnChannelNr);

  virtual UINT Acquisition_Close(HACQDESC hAcqDesc);

  virtual UINT Acquisition_CloseAll();

  virtual UINT Acquisition_DefineDestBuffers(
      HACQDESC pAcqDesc, unsigned short *pProcessedData,
      UINT nFrames, UINT nRows, UINT nColumns);

  virtual UINT Acquisition_Abort(HACQDESC hAcqDesc);

  virtual UINT Acquisition_Acquire_Image(
      HACQDESC pAcqDesc, UINT dwFrames, UINT dwSkipFrms,
      UINT dwOpt, unsigned short *pwOffsetData,
      DWORD *pdwGainData, DWORD *pdwPxlCorrList);

  virtual UINT Acquisition_GetConfiguration(
      HACQDESC hAcqDesc, UINT *dwFrames, UINT *dwRows, UINT *dwColumns, UINT *dwDataType,
      UINT *dwSortFlags, BOOL *bIRQEnabled, DWORD *dwAcqType, DWORD *dwSystemID,
      DWORD *dwSyncMode, DWORD *dwHwAccess);

  virtual UINT Acquisition_GetIntTimes(
      HACQDESC hAcqDesc, double *dblIntTime, int *nIntTimes);

  virtual UINT Acquisition_GetActFrame(
      HACQDESC hAcqDesc, DWORD *dwActAcqFrame, DWORD *dwActSecBuffFrame);

  virtual UINT Acquisition_ResetFrameCnt(HACQDESC hAcqDesc);

  virtual UINT Acquisition_GetHwHeaderInfo(
      HACQDESC hAcqDesc, CHwHeaderInfo *pInfo);

  virtual UINT Acquisition_SetFrameSyncMode(
      HACQDESC hAcqDesc, DWORD dwMode);

  virtual UINT Acquisition_SetTimerSync(
      HACQDESC hAcqDesc, DWORD *dwCycleTime);

  virtual UINT Acquisition_SetCameraGain(
      HACQDESC hAcqDesc, WORD wMode);

  virtual UINT Acquisition_SetCameraBinningMode(
      HACQDESC hAcqDesc, WORD wMode);

  virtual UINT Acquisition_GetCameraBinningMode(
      HACQDESC hAcqDesc, WORD* wMode);

  virtual UINT Acquisition_GetHwHeaderInfoEx(
      HACQDESC hAcqDesc, CHwHeaderInfo *pInfo, CHwHeaderInfoEx *pInfoEx);
};

#endif // HAVE_PERKIN_ELMER

#endif // QXRDPERKINELMERPLUGIN_H
