/******************************************************************
*
*  $Id: qxrdxislinterface.h,v 1.2 2010/09/13 20:00:42 jennings Exp $
*
*******************************************************************/

#ifndef QXRDXISLINTERFACE_H
#define QXRDXISLINTERFACE_H

#include <QObject>
#include "qcepmacros.h"

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

#include "Acq.h"

#define DEFINE_XISL_ROUTINE(fname,args) \
public: UINT fname args; \
      private: typedef UINT (__stdcall *p##fname) args; \
        p##fname m_##fname;

class QxrdXISLInterface {
public:
  QxrdXISLInterface();

  DEFINE_XISL_ROUTINE(Acquisition_SetCallbacksAndMessages,(HACQDESC pAcqDesc, HWND hWnd, UINT dwErrorMsg, UINT dwLoosingFramesMsg, void (CALLBACK *lpfnEndFrameCallback)(HACQDESC), void (CALLBACK *lpfnEndAcqCallback)(HACQDESC)));
  DEFINE_XISL_ROUTINE(Acquisition_EnumSensors, (UINT *pdwNumSensors, BOOL bEnableIRQ, BOOL bAlwaysOpen));
  DEFINE_XISL_ROUTINE(Acquisition_GetNextSensor, (ACQDESCPOS *Pos, HACQDESC *phAcqDesc));
  DEFINE_XISL_ROUTINE(Acquisition_GetCommChannel, (HACQDESC pAcqDesc, UINT *pdwChannelType, int *pnChannelNr));
  DEFINE_XISL_ROUTINE(Acquisition_DefineDestBuffers, (HACQDESC pAcqDesc, unsigned short *pProcessedData, UINT nFrames, UINT nRows, UINT nColumns));
  DEFINE_XISL_ROUTINE(Acquisition_Acquire_Image, (HACQDESC pAcqDesc, UINT dwFrames, UINT dwSkipFrms, UINT dwOpt, unsigned short *pwOffsetData, DWORD *pdwGainData, DWORD *pdwPxlCorrList));
  DEFINE_XISL_ROUTINE(Acquisition_GetConfiguration, (HACQDESC hAcqDesc,
                                          UINT *dwFrames, UINT *dwRows, UINT *dwColumns, UINT *dwDataType,
                                          UINT *dwSortFlags, BOOL *bIRQEnabled, DWORD *dwAcqType, DWORD *dwSystemID,
                                          DWORD *dwSyncMode, DWORD *dwHwAccess));
  DEFINE_XISL_ROUTINE(Acquisition_GetIntTimes, (HACQDESC hAcqDesc, double *dblIntTime, int *nIntTimes));
  DEFINE_XISL_ROUTINE(Acquisition_GetActFrame, (HACQDESC hAcqDesc, DWORD *dwActAcqFrame, DWORD *dwActSecBuffFrame));
  DEFINE_XISL_ROUTINE(Acquisition_GetHwHeaderInfo, (HACQDESC hAcqDesc, CHwHeaderInfo *pInfo));
  DEFINE_XISL_ROUTINE(Acquisition_SetFrameSyncMode, (HACQDESC hAcqDesc, DWORD dwMode));
  DEFINE_XISL_ROUTINE(Acquisition_SetTimerSync, (HACQDESC hAcqDesc, DWORD *dwCycleTime));
  DEFINE_XISL_ROUTINE(Acquisition_SetCameraGain, (HACQDESC hAcqDesc, WORD wMode));
  DEFINE_XISL_ROUTINE(Acquisition_SetCameraBinningMode, (HACQDESC hAcqDesc, WORD wMode));
  DEFINE_XISL_ROUTINE(Acquisition_GetCameraBinningMode, (HACQDESC hAcqDesc, WORD* wMode));
  DEFINE_XISL_ROUTINE(Acquisition_GetHwHeaderInfoEx, (HACQDESC hAcqDesc, CHwHeaderInfo *pInfo ,CHwHeaderInfoEx *pInfoEx));

private:
  HEADER_IDENT("$Id: qxrdxislinterface.h,v 1.2 2010/09/13 20:00:42 jennings Exp $");
};

#endif // QXRDXISLINTERFACE_H

/******************************************************************
*
*  $Log: qxrdxislinterface.h,v $
*  Revision 1.2  2010/09/13 20:00:42  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:42  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.3  2010/06/07 20:17:42  jennings
*  Added more cvs log and ident stuff - removed qxrdspecserver.cpp & qxrdspecserver.h
*
*
*
*******************************************************************/
