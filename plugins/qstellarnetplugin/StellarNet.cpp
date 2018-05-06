// StellarNet.cpp : Stellarnet Spectrometer Functions (interface to swdll.dll)
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
//#include "stdafx.h"
#include "StellarNet.h"

#ifdef Q_OS_WIN
//#include "resource.h"
#include <windows.h>

// Declare function pointers to fuctions inside Stellarnet Library
typedef void (_stdcall* FncPtrVoid)(void);
typedef void (_stdcall* FncPtrInt)(int);
typedef int  (_stdcall* FncPtrIntFloatP)(int,float*);
typedef int	 (_stdcall* FncPtrIntFloatPLV)(int,float*);
typedef int	 (_stdcall* FncPtrIntIntInt)(int,int,int);
typedef int	 (_stdcall* FncPtrIntVoid)(void);
typedef BOOL (_stdcall* FncPtrWordWordCharP)(_int16,_int16,char*);

FncPtrVoid SWDinit;    // Function pointer
FncPtrInt SWDrate;
FncPtrIntFloatP SWDscan;
FncPtrIntFloatPLV SWDscanLV;
FncPtrVoid SWDclose;
FncPtrIntIntInt SWDupdate;
FncPtrInt SWDxtrate;
FncPtrIntVoid SWDdevcnt;
FncPtrWordWordCharP SWDeeRead;

static BOOL StellarNetLibraryOpen;
HINSTANCE hDLL;				// Handle to SWDLL (Stellarnet DLL)

bool SpectrometerBusy = 0;

// *******************************************************
// ************  Open StellarNet Library  ****************
// *******************************************************
int OpenStellarNetLibrary(void)
{
  SetDllDirectory(L"C:\\Program Files\\StellarNet\\SpectraWiz\\");
        hDLL = LoadLibrary(L"swdll64.dll");
	if (hDLL == NULL) {
//                MessageBox(GetActiveWindow(), L"SWDLL not loaded.", L"Error", MB_OK );
		return(FALSE);
	}

	// Put adresses of SWDLL functions into function pointers.
	SWDinit = (FncPtrVoid)GetProcAddress(hDLL, "SWDinit");
	SWDrate = (FncPtrInt)GetProcAddress(hDLL, "SWDrate");
	SWDscan = (FncPtrIntFloatP)GetProcAddress(hDLL, "SWDscan");
	SWDscanLV = (FncPtrIntFloatPLV)GetProcAddress(hDLL, "SWDscanLV");
	SWDclose = (FncPtrVoid)GetProcAddress(hDLL, "SWDclose");
	SWDupdate = (FncPtrIntIntInt)GetProcAddress(hDLL, "SWDupdate");
	SWDxtrate = (FncPtrInt)GetProcAddress(hDLL, "SWDxtrate");
	SWDdevcnt = (FncPtrIntVoid)GetProcAddress(hDLL, "SWDdevcnt");
	SWDeeRead = (FncPtrWordWordCharP)GetProcAddress(hDLL, "SWDeeRead");

	if (!SWDinit) {
		FreeLibrary(hDLL);       
		return(-10);
	}
	if (!SWDrate) {
		FreeLibrary(hDLL);       
		return(-11);
	}
	if (!SWDscan) {
		FreeLibrary(hDLL);       
		return(-12);
	}
	if (!SWDscanLV) {
		FreeLibrary(hDLL);       
		return(-13);
	}
	if (!SWDupdate) {
		FreeLibrary(hDLL);
		return(-14);
	}
	if (!SWDxtrate) {
		FreeLibrary(hDLL);
		return(-15);
	}
	if (!SWDclose) {
		FreeLibrary(hDLL);
		return(-16);
	}
	if (!SWDdevcnt) {
//                MessageBox(GetActiveWindow(), L"Upgrade Stellarnet DLL to use new SWD functions.", L"Error", MB_OK );
		SWDinit();	//Call only once!!!!
		StellarNetLibraryOpen=TRUE;
		return(-17);
	}
	if (!SWDeeRead) {
//                MessageBox(GetActiveWindow(), L"Upgrade Stellarnet DLL to use new SWD functions.", L"Error", MB_OK );
		SWDinit();	//Call only once!!!!
		StellarNetLibraryOpen=TRUE;
		return(-18);
	}

	// Initialize SWDLL
        SWDinit();	//Call only once!!!!
	StellarNetLibraryOpen=TRUE;

	return(TRUE);
}
// *******************************************************
// **********  End of Open StellarNet Library ************
// *******************************************************




// *******************************************************
// ************  Close StellarNet Library  ***************
// *******************************************************
void CloseStellarNetLibrary(void)
{
	if( StellarNetLibraryOpen!=TRUE ) return;

	SWDclose();		   // allow SWDLL to release its resources
	FreeLibrary(hDLL);      // release dynamic link library SWDLL.DLL
}
// *******************************************************
// **********  End of Close StellarNet Library ***********
// *******************************************************



// *******************************************************
// **************  Initialize Spectrometer  **************
// *******************************************************
void InitializeSpectrometer(int IntegrationPeriod, int ScansToAvg, int xsmooth, int xtrate, bool TemperatureCompensation)
{
	if( StellarNetLibraryOpen!=TRUE ) return;

	// change detector integration rate in Milli-seconds (4-65500)
	SWDrate(IntegrationPeriod);

	// dynamic update for data processing configuration
	// ScansToAvg = 1..99 
	// >>scan averaging accumulates scans then divides by ScansToAverage
	// xsmooth = (0=none, 1=5_pixel, 2=9_pixel, 3=17_pixel, 4=33_pixel)
	// >>smoothing uses pixel boxcar averaging across spectral scan
	// TempComp = 0=none 1=on 
	// >>temperature compensation uses detector optical black region pixels 1-12
	SWDupdate(ScansToAvg, xsmooth, TemperatureCompensation);

	// set xtiming resolution clock rate 0-2 where 0=fastest & lowest resolution
	SWDxtrate(xtrate);

	return;
}
// *******************************************************
// ***********  End of Initialize Spectrometer  **********
// *******************************************************




// *******************************************************
// *********  Read Spectrum from Spectrometer  ***********
// *******************************************************
bool ReadSpectrometer( HWND hWnd, int chan, float *DataBuffer )
{
	MSG msg;
	long timeout = 0;
	int scode = 0;

	if (SpectrometerBusy == true) {
		return(false);
	}

	SpectrometerBusy = true;
	do {
		if(scode!=0) Sleep(1);
		scode = SWDscanLV( chan, DataBuffer);  // get spectral scan

		timeout++;	// exit if there is a problem
		if( timeout>3000 ) {
			SpectrometerBusy = false;
			return(false);
		}

	} while(scode!=0);
	SpectrometerBusy = false;

	return(true);
}
// *******************************************************
// ******  End of Read Spectrum from Spectrometer  *******
// *******************************************************




// *******************************************************
// ******  Get the number of spectrometers attached. *****
// *******************************************************
int getDeviceCount(void)
{
	int NumberOfSpectrometers;

	NumberOfSpectrometers = SWDdevcnt();
	return(NumberOfSpectrometers);
}
// *******************************************************
// **  End of get the number of spectrometers attached. **
// *******************************************************




// *******************************************************
// get coefficients c1,c2,c3,c4 stored in spectrometer eerom
// *******************************************************
int getBWee(int chan, double *CalCoeff1, double *CalCoeff2, double *CalCoeff3, double *CalCoeff4)
{
	int i;
	_int16 ec, ea;
	char buffer[32];

	//---------------------
	ec=chan;
	ea=0x80;	// C1
	if( !SWDeeRead(ec, ea, buffer) ) { // check if BW has existing data
		return(1);	// Error reading C1
	}

	for( i=0 ; i<=16 ; i++ ) {
		if( buffer[i]<0x20 || buffer[i]>127 ) {
			return(10);	// invalid data here - vamoos
		}
	}

	buffer[14] = 0;
	*CalCoeff1 = atof(buffer);

	//-------------------------
	ea=0xA0;	// C2
	if( !SWDeeRead(ec, ea, buffer) ) {
		return(2);	// Error reading C2
	}
	buffer[14] = 0;
	*CalCoeff2 = atof(buffer);

	//-----------------------------                                                   
	ea=0xC0;	// C3
	if( !SWDeeRead(ec, ea, buffer) ) {
		return(3);	// Error reading C3
	}
	buffer[14] = 0;
	*CalCoeff3 = atof(buffer);

	//---------------------------------
	ea=0xE0;	// C4
	if( !SWDeeRead(ec, ea, buffer) ) {
		return(4);	// Error reading C4
	}
	buffer[14] = 0;
	*CalCoeff4 =atof(buffer);

	return(0);	// No error
}
// ****************************************************************
// End of get coefficients c1,c2,c3,c4 stored in spectrometer eerom
// ****************************************************************

#endif
