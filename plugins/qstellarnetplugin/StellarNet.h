// StellarNet.h : Include for Stellarnet Spectrometer Functions (interface to swdll.dll)
// Copyright (c) 2011 All Right Reserved, http://stellarnet-inc.com/
//
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Author:Mark Wood, mswood@pobox.com
// Date:2011-02-11
//
// Functions in StellarNet.cpp

#include <QObject>

#ifdef Q_OS_WIN
#include <windows.h>

int OpenStellarNetLibrary(void);
void CloseStellarNetLibrary(void);
void InitializeSpectrometer(int IntegrationPeriod, int ScansToAvg, int xsmooth, int xtrate, bool TemperatureCompensation);
bool ReadSpectrometer( HWND hWnd, int chan, float *DataBuffer );
int getDeviceCount(void);
int getBWee(int chan, double *c1, double *c2, double *c3, double *c4);
#endif
