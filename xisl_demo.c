#include <windows.h>
#include <stdio.h>
#include <assert.h>
//#include "..\..\COMMON\XISL 4.0\Acq.h"
#include "Acq.h"
/*
this is a little demonstration for the use of the XISL
*/
HANDLE hOutput = NULL, hInput = NULL;
DWORD dwCharsWritten=0;
unsigned short *pAcqBuffer=NULL, *pOffsetBuffer=NULL,*pBrightocBuffer=NULL,*pGainSeqBuffer=NULL,*pGainSeqMedBuffer=NULL;
DWORD *pGainBuffer = NULL, *pPixelBuffer = NULL;
HANDLE hevEndAcq=NULL; //signaled at end of acquisition by corresponding callback
char strBuffer[1000]; //buffer for outputs

CHwHeaderInfo Info; 
CHwHeaderInfoEx InfoEx; // new Header 1621
WORD actframe;

#define ACQ_CONT			1
#define ACQ_OFFSET			2
#define ACQ_GAIN			4
#define ACQ_SNAP			8
#define	ACQ_Brightoc		16

//callback function that is called by XISL every frame at end of data transfer
void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc)
{
	DWORD dwActFrame, dwSecFrame, dwRow=128, dwCol=128;
	DWORD dwDataType, dwRows, dwColumns, dwFrames, dwSortFlags,
	dwIRQFlags, dwAcqType, dwSystemID, dwAcqData, dwSyncMode, dwHwAccess;
	
	Acquisition_GetConfiguration(hAcqDesc, &dwFrames, &dwRows,
		&dwColumns, &dwDataType, &dwSortFlags, &dwIRQFlags, &dwAcqType, 
		&dwSystemID, &dwSyncMode, &dwHwAccess);
	Acquisition_GetActFrame(hAcqDesc, &dwActFrame, &dwSecFrame);
	Acquisition_GetAcqData(hAcqDesc, &dwAcqData);

	// 1621 function demo
	Acquisition_GetLatestFrameHeader(hAcqDesc,&Info,&InfoEx);
	if (Info.dwHeaderID==14 && InfoEx.wCameratype==1) //1621 ?)
	{
		sprintf(strBuffer, "framecount: %d frametime %d,%d millisec\t",InfoEx.wFrameCnt,InfoEx.wRealInttime_milliSec,InfoEx.wRealInttime_microSec);
		WriteConsole(hOutput, strBuffer, strlen(strBuffer), &dwCharsWritten, NULL);
	}
	// 1621 function demo end

	if (dwAcqData & ACQ_SNAP) // 25.09.00
	{
		sprintf(strBuffer, "acq buffer frame: %d, dest frame %d, row: %d, col: %d, value: %d\n", 
			dwActFrame, dwSecFrame,
			dwRow, dwCol, pAcqBuffer[dwColumns*dwRow+dwCol]);
	}

	else if (dwAcqData & ACQ_CONT) 
	{
		sprintf(strBuffer, "acq buffer frame: %d, dest frame %d, row: %d, col: %d, value: %d\n", 
			dwActFrame, dwSecFrame,
			dwRow, dwCol, pAcqBuffer[dwColumns*dwRow+dwCol]);


	} else if (dwAcqData & ACQ_OFFSET)
	{
		sprintf(strBuffer, "offset buffer frame: %d, dest frame %d, row: %d, col: %d, value: %d\n", 
			dwActFrame, dwSecFrame,
			dwRow, dwCol, pOffsetBuffer[dwColumns*dwRow+dwCol]);

	}  else if (dwAcqData & ACQ_GAIN)
	{
		sprintf(strBuffer, "gain buffer frame: %d, dest frame %d, row: %d, col: %d, value: %d\n", 
			dwActFrame, dwSecFrame,
			dwRow, dwCol, pGainBuffer[dwColumns*dwRow+dwCol]);

	} else	if (dwAcqData & ACQ_Brightoc)
	{
		sprintf(strBuffer, "gain buffer frame: %d, dest frame %d, row: %d, col: %d, value: %d\n", 
			dwActFrame, dwSecFrame,
			dwRow, dwCol, pBrightocBuffer[dwColumns*dwRow+dwCol]);

	}
	else
	{
		//printf("endframe \n");
		sprintf(strBuffer, "endframe image size %d\n\n",dwRows);
	}
	WriteConsole(hOutput, strBuffer, strlen(strBuffer), &dwCharsWritten, NULL);
}

//callback function that is called by XISL at end of acquisition
void CALLBACK OnEndAcqCallback(HACQDESC hAcqDesc)
{

	DWORD dwActFrame, dwSecFrame, dwRow=128, dwCol=128;
	DWORD dwDataType, dwRows, dwColumns, dwFrames, dwSortFlags,
	dwIRQFlags, dwAcqType, dwSystemID, dwAcqData, dwSyncMode, dwHwAccess;
	
	Acquisition_GetConfiguration(hAcqDesc, &dwFrames, &dwRows,
		&dwColumns, &dwDataType, &dwSortFlags, &dwIRQFlags, &dwAcqType, 
		&dwSystemID, &dwSyncMode, &dwHwAccess);
	Acquisition_GetActFrame(hAcqDesc, &dwActFrame, &dwSecFrame);
	Acquisition_GetAcqData(hAcqDesc, &dwAcqData);

	
	printf("dwActTyp %d\n",dwAcqType);

 	printf("End of Acquisition\n");
	SetEvent(hevEndAcq);
}


int main()
{
	DWORD time;
	int nRet = HIS_ALL_OK;
	int test;
	BOOL bEnableIRQ;
	DWORD *pPixelPtr = NULL;
	DWORD dwDataType, dwRows, dwColumns, dwFrames, dwSortFlags,
		dwAcqType, dwSystemID, dwSyncMode, dwHwAccess,
		dwNumSensors;
	HACQDESC hAcqDesc=NULL;
	INPUT_RECORD ir;
	DWORD dwRead;
	char szFileName[300];
	FILE *pFile = NULL;
	ACQDESCPOS Pos = 0;
	CHwHeaderInfo pInfo;
	double *dblIntTime;
	int *inttimes;
	int nChannelNr;
	UINT nChannelType;

	//get an output handle to console
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOutput == INVALID_HANDLE_VALUE)
	{
		//error handling
		return 0;
	}

	hInput = GetStdHandle(STD_INPUT_HANDLE);
	if (hInput == INVALID_HANDLE_VALUE)
	{
		//error handling
		return 0;
	}
	
	//At first we tell the system to enumerate all available sensors
	//and initialize them in polling mode
	//to enable interrupt support set bEnableIRQ = TRUE;
	bEnableIRQ =TRUE;//TRUE;//FALSE;
	nRet = Acquisition_EnumSensors(&dwNumSensors, bEnableIRQ, FALSE);
	if (nRet!=HIS_ALL_OK)
	{
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
		goto Exit;
	}

	printf("%d sensors recognized!\n", dwNumSensors);

	//now we iterate through all this sensors and display sensor data
	do
	{
		if ((nRet = Acquisition_GetNextSensor(&Pos, &hAcqDesc))!=HIS_ALL_OK)
		{
			char szMsg[300];
			sprintf(szMsg, "Error nr.: %d", nRet);
			MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
			goto Exit;
		}
	
		//ask for communication device type and its number
		if ((nRet=Acquisition_GetCommChannel(hAcqDesc, &nChannelType, &nChannelNr))!=HIS_ALL_OK)
		{
			//error handling
			char szMsg[300];
			sprintf(szMsg, "Error nr.: %d", nRet);
			MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
			return 0;
		}

		//ask for data organization of all sensors
		if ((nRet=Acquisition_GetConfiguration(hAcqDesc, &dwFrames, &dwRows,
			&dwColumns, &dwDataType, &dwSortFlags, &bEnableIRQ
			
			, &dwAcqType, 
			&dwSystemID, &dwSyncMode, &dwHwAccess))!=HIS_ALL_OK)
		{
			//error handling
			char szMsg[300];
			sprintf(szMsg, "Error nr.: %d", nRet);
			MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
			return 0;
		}
		sprintf(strBuffer, "channel type: %d, ChannelNr: %d\n", nChannelType, nChannelNr);
		WriteConsole(hOutput, strBuffer, strlen(strBuffer), &dwCharsWritten, NULL);
		sprintf(strBuffer, "frames: %d\n", dwFrames);
		WriteConsole(hOutput, strBuffer, strlen(strBuffer), &dwCharsWritten, NULL);
		sprintf(strBuffer, "rows: %d\ncolumns: %d\n", dwRows, dwColumns);
		WriteConsole(hOutput, strBuffer, strlen(strBuffer), &dwCharsWritten, NULL);

		// now set callbacks and messages for every sensor
		if ((nRet=Acquisition_SetCallbacksAndMessages(hAcqDesc, NULL, 0,
			0, OnEndFrameCallback, OnEndAcqCallback))!=HIS_ALL_OK)
		{
			//error handling
			char szMsg[300];
			sprintf(szMsg, "Error nr.: %d", nRet);
			MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
			goto Exit;
		}

	} while (Pos!=0);


	//we continue with one sensor to show the other XISL features

	// short demo how to use the features of the 1621 if one is connected
		
	//ask for communication device type and its number
	if ((nRet=Acquisition_GetCommChannel(hAcqDesc, &nChannelType, &nChannelNr))!=HIS_ALL_OK)
	{
		//error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
		return 0;
	}				
	// check if optical framgrabber is used
	// 
	if (nChannelType==HIS_BOARD_TYPE_ELTEC_XRD_FGX) 
	{
		//create and receive FrameHeader
		CHwHeaderInfo Info; 
		CHwHeaderInfoEx InfoEx; // new Header 1621
		if ((nRet = Acquisition_GetHwHeaderInfoEx(hAcqDesc, &Info, &InfoEx))==HIS_ALL_OK)
		{
			// header could be retrieved
			
			//check if 1621 connected
			if (Info.dwHeaderID==14 && InfoEx.wCameratype==1)
			{
				unsigned short *pOffsetBufferBinning1=NULL,*pOffsetBufferBinning2=NULL;

				WORD wBinning=1;
				int timings = 8;
				//create lists to receive timings for different binning modes
				double* m_pTimingsListBinning1;
				double* m_pTimingsListBinning2;
				m_pTimingsListBinning1 = (double*) malloc(timings*sizeof(double));
				m_pTimingsListBinning2 = (double*) malloc(timings*sizeof(double));

				//  set detector timing and gain
				if ((nRet = Acquisition_SetCameraMode(hAcqDesc, 0))!=HIS_ALL_OK) goto exit;
				if ((nRet = Acquisition_SetCameraGain(hAcqDesc, 1))!=HIS_ALL_OK) goto exit;
				// set detector to default binning mode
				if ((nRet = Acquisition_SetCameraBinningMode(hAcqDesc,wBinning))!=HIS_ALL_OK) goto exit;
				// get int times for selected binning mode
				if ((nRet = Acquisition_GetIntTimes(hAcqDesc, m_pTimingsListBinning1, &timings))!=HIS_ALL_OK) goto exit;
				
				//ask for detector data organization to 
				if ((nRet = Acquisition_GetConfiguration(hAcqDesc, &dwFrames, &dwRows,
					&dwColumns, &dwDataType, &dwSortFlags, &bEnableIRQ, &dwAcqType, 
					&dwSystemID, &dwSyncMode, &dwHwAccess))!=HIS_ALL_OK) goto exit;
				
				// get offsetfile
				//allocate memory for offset data
				pOffsetBufferBinning1 = malloc(dwRows*dwColumns*sizeof(short));
				if ((nRet = Acquisition_SetAcqData(hAcqDesc, 0))!=HIS_ALL_OK) goto exit;
				
				printf("\nAcquire offset data now!\n"); //valtest

				hevEndAcq = CreateEvent(NULL, FALSE, FALSE, NULL);
				
				//acquire 13 dark frames and average them
				if ((nRet = Acquisition_Acquire_OffsetImage(hAcqDesc, pOffsetBufferBinning1, dwRows, dwColumns, 13))!=HIS_ALL_OK) goto exit;
					
				//wait for end of acquisition event
				if (WaitForSingleObject(hevEndAcq, INFINITE)!=WAIT_OBJECT_0) goto exit;
				
				// get gainimage is similar....

				//now change to other binning mode
				// set detector to default binning mode
				wBinning=2;
				// set detector to 2x2 binned mode
				if ((nRet =Acquisition_SetCameraBinningMode(hAcqDesc,wBinning))!=HIS_ALL_OK) goto exit;
				// get int times for selected binning mode
				if ((nRet =Acquisition_GetIntTimes(hAcqDesc, m_pTimingsListBinning2, &timings))!=HIS_ALL_OK) goto exit;
								
				//ask for changed detector data organization
				if ((nRet =Acquisition_GetConfiguration(hAcqDesc, &dwFrames, &dwRows,
					&dwColumns, &dwDataType, &dwSortFlags, &bEnableIRQ, &dwAcqType, 
					&dwSystemID, &dwSyncMode, &dwHwAccess))!=HIS_ALL_OK) goto exit;
				
				// get offsetfile
				//allocate memory for offset data
				pOffsetBufferBinning2 = malloc(dwRows*dwColumns*sizeof(short));
				if ((nRet =Acquisition_SetAcqData(hAcqDesc, 0))!=HIS_ALL_OK) goto exit;
				
				printf("\nAcquire offset data now!\n"); //valtest

				hevEndAcq = CreateEvent(NULL, FALSE, FALSE, NULL);
				
				//acquire 13 dark frames and average them
				if ((nRet=Acquisition_Acquire_OffsetImage(hAcqDesc, pOffsetBufferBinning2, dwRows, dwColumns, 13))!=HIS_ALL_OK) goto exit;
				
				//wait for end of acquisition event
				if (WaitForSingleObject(hevEndAcq, INFINITE)!=WAIT_OBJECT_0) goto exit;
				// get gainimage is similar....


				// now back to default binning
				wBinning=1;
				if ((nRet=Acquisition_SetCameraBinningMode(hAcqDesc,wBinning))!=HIS_ALL_OK) goto exit;
			
				// get offsetcorr averaged image binning1
				//ask for detector data organization
				if ((nRet=Acquisition_GetConfiguration(hAcqDesc, &dwFrames, &dwRows,
					&dwColumns, &dwDataType, &dwSortFlags, &bEnableIRQ, &dwAcqType, 
					&dwSystemID, &dwSyncMode, &dwHwAccess))!=HIS_ALL_OK) goto exit;
				
				//allocate acquisition buffer
				pAcqBuffer = malloc(1*dwRows*dwColumns*sizeof(short));
				if (!pAcqBuffer) goto Exit;

				if ((nRet=Acquisition_SetAcqData(hAcqDesc, ACQ_SNAP))!=HIS_ALL_OK) goto exit;

				if ((nRet=Acquisition_DefineDestBuffers(hAcqDesc, pAcqBuffer,
					1, dwRows, dwColumns))!=HIS_ALL_OK) goto exit;

				if ((Acquisition_Acquire_Image(hAcqDesc,15,0, 
					HIS_SEQ_AVERAGE, pOffsetBufferBinning1, NULL, NULL))!=HIS_ALL_OK) goto exit;

				//wait for end of acquisition event
				if ( WaitForSingleObject(hevEndAcq, INFINITE)!=WAIT_OBJECT_0) goto exit;
				free(pAcqBuffer);
				
				// get offsetcorr averaged image binning2
				// now back to binning 2
				wBinning=2;
				if ((nRet=Acquisition_SetCameraBinningMode(hAcqDesc,wBinning))!=HIS_ALL_OK) goto exit;
			
				// get offsetcorr averaged image binning2
				//ask for detector data organization
				if ((nRet=Acquisition_GetConfiguration(hAcqDesc, &dwFrames, &dwRows,
					&dwColumns, &dwDataType, &dwSortFlags, &bEnableIRQ, &dwAcqType, 
					&dwSystemID, &dwSyncMode, &dwHwAccess))!=HIS_ALL_OK) goto exit;
				
				//allocate acquisition buffer
				pAcqBuffer = malloc(1*dwRows*dwColumns*sizeof(short));
				if (!pAcqBuffer) goto Exit;

				if ((nRet=Acquisition_SetAcqData(hAcqDesc, ACQ_SNAP))!=HIS_ALL_OK) goto exit;
	
				if ((nRet=Acquisition_DefineDestBuffers(hAcqDesc, pAcqBuffer,
					1, dwRows, dwColumns))!=HIS_ALL_OK) goto exit;

				// Reset Framecounter to read out and display the framecnt in the endframecallback
				if ((nRet=Acquisition_ResetFrameCnt(hAcqDesc))!=HIS_ALL_OK) goto exit;
				
				if ((Acquisition_Acquire_Image(hAcqDesc,15,0, 
					HIS_SEQ_AVERAGE, pOffsetBufferBinning2, NULL, NULL))!=HIS_ALL_OK) goto exit;

				//wait for end of acquisition event
				if ( WaitForSingleObject(hevEndAcq, INFINITE)!=WAIT_OBJECT_0) goto exit;
				free(pAcqBuffer);
				free(pOffsetBufferBinning1);
				free(pOffsetBufferBinning2);

				// now back to default binning
				wBinning=1;
				if ((nRet=Acquisition_SetCameraBinningMode(hAcqDesc,wBinning))!=HIS_ALL_OK) goto exit;
				//ask for detector data organization
				if ((nRet=Acquisition_GetConfiguration(hAcqDesc, &dwFrames, &dwRows,
					&dwColumns, &dwDataType, &dwSortFlags, &bEnableIRQ, &dwAcqType, 
					&dwSystemID, &dwSyncMode, &dwHwAccess))!=HIS_ALL_OK) goto exit;

			}
		}

	}
	// board type is HIS_BOARD_TYPE_ELTEC

	//acquire 10 frames
	dwFrames = 10;	
	
	//allocate acquisition buffer
	pAcqBuffer = malloc(dwFrames*dwRows*dwColumns*sizeof(short));
	if (!pAcqBuffer)
	{
		//error handling
		goto Exit;
	}



	hevEndAcq = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!hevEndAcq)
	{
		//error handling
		goto Exit;
	}


	// Timing 0 Internal Timer 5000msec
	Acquisition_SetCameraMode(hAcqDesc,0);
	Acquisition_SetFrameSyncMode(hAcqDesc,HIS_SYNCMODE_INTERNAL_TIMER);
	time=1000*500;
	Acquisition_SetTimerSync(hAcqDesc,&time);

	Acquisition_SetAcqData(hAcqDesc, ACQ_SNAP);

	if ((nRet=Acquisition_DefineDestBuffers(hAcqDesc, pAcqBuffer,
		dwFrames, dwRows, dwColumns))!=HIS_ALL_OK)
	{
		//error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
		goto Exit;
	}


	if((Acquisition_Acquire_Image(hAcqDesc,dwFrames,0, 
	//		HIS_SEQ_DEST_ONE_FRAME, NULL, NULL, NULL))!=HIS_ALL_OK)
	//		HIS_SEQ_ONE_BUFFER, NULL, NULL, NULL))!=HIS_ALL_OK)
	//		HIS_SEQ_TWO_BUFFERS, NULL, NULL, NULL))!=HIS_ALL_OK)
	//		HIS_SEQ_CONTINUOUS, NULL, NULL, NULL))!=HIS_ALL_OK)
	//		HIS_SEQ_AVERAGE, NULL, NULL, NULL))!=HIS_ALL_OK)
	//		HIS_SEQ_COLLATE, NULL, NULL, NULL))!=HIS_ALL_OK)
			HIS_SEQ_ONE_BUFFER, NULL, NULL, NULL))!=HIS_ALL_OK)
		{
			//error handling
			char szMsg[300];
			sprintf(szMsg, "Error nr.: %d", nRet);
			MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
			goto Exit;
		}
		
		sprintf(strBuffer, "I'm waiting\n");
		WriteConsole(hOutput, strBuffer, strlen(strBuffer), &dwCharsWritten, NULL);

		//wait for end of acquisition event
		nRet = WaitForSingleObject(hevEndAcq, INFINITE);
		

	free(pAcqBuffer);
	switch(nRet)
	{
	case WAIT_FAILED:
		MessageBox(NULL, "wait failed", "debug message", MB_OK | MB_ICONSTOP);
		goto Exit;
	case WAIT_OBJECT_0:
		break;
	case WAIT_ABANDONED:
		MessageBox(NULL, "wait abandoned by mutex", "debug message", MB_OK | MB_ICONSTOP);
		break;
	case WAIT_TIMEOUT:
		MessageBox(NULL, "wait time out occured", "debug message", MB_OK | MB_ICONSTOP);
		break; 
	}

	// Free Running Timing 1
	Acquisition_SetCameraMode(hAcqDesc,1);
	Acquisition_SetFrameSyncMode(hAcqDesc,HIS_SYNCMODE_FREE_RUNNING);
	
	dwFrames=1;
	pAcqBuffer = malloc(dwFrames*dwRows*dwColumns*sizeof(short));
	if (!pAcqBuffer)
	{
		//error handling
		goto Exit;
	}

	//route acquisition buffer to XISL
	if ((nRet=Acquisition_DefineDestBuffers(hAcqDesc, pAcqBuffer,
 		dwFrames, dwRows, dwColumns))!=HIS_ALL_OK)
	{
		//error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
		goto Exit;
	}

	//create end of acquisition event
	hevEndAcq = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!hevEndAcq)
	{
		//error handling
		goto Exit;
	}

	
	Acquisition_SetAcqData(hAcqDesc, ACQ_SNAP);

	// Acquire 10 Frames Averaged 
	if ((nRet=Acquisition_Acquire_Image(hAcqDesc,10,0,
		HIS_SEQ_AVERAGE, NULL, NULL, NULL))!=HIS_ALL_OK)
		{
			//error handling
			char szMsg[300];
			sprintf(szMsg, "Error nr.: %d", nRet);
			MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
			goto Exit;
		}
		
		sprintf(strBuffer, "I'm waiting\n");
		WriteConsole(hOutput, strBuffer, strlen(strBuffer), &dwCharsWritten, NULL);

		//wait for end of acquisition event
		nRet = WaitForSingleObject(hevEndAcq, INFINITE);

	free(pAcqBuffer);
	switch(nRet)
	{
	case WAIT_FAILED:
		MessageBox(NULL, "wait failed", "debug message", MB_OK | MB_ICONSTOP);
		goto Exit;
	case WAIT_OBJECT_0:
		break;
	case WAIT_ABANDONED:
		MessageBox(NULL, "wait abandoned by mutex", "debug message", MB_OK | MB_ICONSTOP);
		break;
	case WAIT_TIMEOUT:
		MessageBox(NULL, "wait time out occured", "debug message", MB_OK | MB_ICONSTOP);
		break; 
	}


	hevEndAcq = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!hevEndAcq)
	{
		//error handling
		goto Exit;
	}

	dwFrames=1;
	sprintf(strBuffer,"\nPress a key to cancel acquisition!\n");
	WriteConsole(hOutput, strBuffer, strlen(strBuffer), &dwCharsWritten, NULL);
	pAcqBuffer = malloc(dwFrames*dwRows*dwColumns*sizeof(short));
	if (!pAcqBuffer)
	{
		//error handling
		goto Exit;
	}

	//route acquisition buffer to XISL
	if ((nRet=Acquisition_DefineDestBuffers(hAcqDesc, pAcqBuffer,
		dwFrames, dwRows, dwColumns))!=HIS_ALL_OK)
	{
		//error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
		goto Exit;
	}
	
	//set acquisition data to use it in callback
	Acquisition_SetAcqData(hAcqDesc, ACQ_CONT);

	//continuous acquisition
	if ((nRet=Acquisition_Acquire_Image(hAcqDesc, dwFrames, 0, HIS_SEQ_CONTINUOUS, NULL, NULL, NULL))!=HIS_ALL_OK)
	{
		//error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
		goto Exit;
	}
	
	FlushConsoleInputBuffer(hInput);

	do
	{
		WaitForSingleObject(hInput, INFINITE);
		ReadConsoleInput(hInput, &ir, 1, &dwRead);
	} while(!(ir.EventType==KEY_EVENT && ir.Event.KeyEvent.bKeyDown));


	Acquisition_Abort(hAcqDesc);
	nRet = WaitForSingleObject(hevEndAcq, INFINITE);

	switch(nRet)
	{
	case WAIT_FAILED:
		MessageBox(NULL, "wait failed", "debug message", MB_OK | MB_ICONSTOP);
		goto Exit;
	case WAIT_OBJECT_0:
		break;
	case WAIT_ABANDONED:
		MessageBox(NULL, "wait abandoned by mutex", "debug message", MB_OK | MB_ICONSTOP);
		break;
	case WAIT_TIMEOUT:
		MessageBox(NULL, "wait time out occured", "debug message", MB_OK | MB_ICONSTOP);
		break;
	}


	dwFrames=1;

	hevEndAcq = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!hevEndAcq)
	{
		//error handling
		goto Exit;
	}

	if (pAcqBuffer)
		free (pAcqBuffer);
	pAcqBuffer = malloc(dwFrames*dwRows*dwColumns*sizeof(short));
	//route acquisition buffer to XISL
	if ((nRet=Acquisition_DefineDestBuffers(hAcqDesc, pAcqBuffer,
		dwFrames, dwRows, dwColumns))!=HIS_ALL_OK)
	{
		//error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
		goto Exit;
	}
	
	//select another frame time (timing 4)
	printf("\nSet camera mode to 4!\n");
	if ((nRet=Acquisition_SetCameraMode(hAcqDesc, 4))!=HIS_ALL_OK)
	{
		//error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
		goto Exit;
	}
	Acquisition_SetFrameSyncMode(hAcqDesc,HIS_SYNCMODE_FREE_RUNNING);

	//set acquisition data to use it in callback
	Acquisition_SetAcqData(hAcqDesc, ACQ_SNAP);

	//continuous acquisition

	printf("\nAcquire Image!\n");
	if ((nRet=Acquisition_Acquire_Image(hAcqDesc, dwFrames, 0, HIS_SEQ_CONTINUOUS, NULL, NULL, NULL))!=HIS_ALL_OK)
	{
		//error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
		goto Exit;
	}

	sprintf(strBuffer, "Press a key to cancel acquisition!\n");
	WriteConsole(hOutput, strBuffer, strlen(strBuffer), &dwCharsWritten, NULL);
	
	Sleep(200);
	FlushConsoleInputBuffer(hInput);
	do
	{
		printf("warte..\n");
		WaitForSingleObject(hInput, INFINITE);
		ReadConsoleInput(hInput, &ir, 1, &dwRead);
	} while(!(ir.EventType==KEY_EVENT && ir.Event.KeyEvent.bKeyDown));


	printf("abort...\n");
	Acquisition_Abort(hAcqDesc);

	nRet = WaitForSingleObject(hevEndAcq, INFINITE);
	switch(nRet)
	{
	case WAIT_FAILED:
		MessageBox(NULL, "wait failed", "debug message", MB_OK | MB_ICONSTOP);
		goto Exit;
	case WAIT_OBJECT_0:
		break;
	case WAIT_ABANDONED:
		MessageBox(NULL, "wait abandoned by mutex", "debug message", MB_OK | MB_ICONSTOP);
		break;
	case WAIT_TIMEOUT:
		MessageBox(NULL, "wait time out occured", "debug message", MB_OK | MB_ICONSTOP);
		break;
	}
//*/

	hevEndAcq = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!hevEndAcq)
	{
		//error handling
		goto Exit;
	}
	//allocate memory for offset data
	pOffsetBuffer = malloc(dwRows*dwColumns*sizeof(short));
	Acquisition_SetAcqData(hAcqDesc, ACQ_OFFSET);
	printf("\nAcquire offset data now!\n"); //valtest

	Acquisition_SetCameraMode(hAcqDesc, 0);

	//acquire 13 dark frames and average them
	if ((nRet=Acquisition_Acquire_OffsetImage(hAcqDesc, pOffsetBuffer, dwRows, dwColumns, 13))!=HIS_ALL_OK)
	{
		//error handler
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
		goto Exit;
	}

	//wait for end of acquisition event
	nRet = WaitForSingleObject(hevEndAcq, INFINITE);
	switch(nRet)
	{
	case WAIT_FAILED:
		MessageBox(NULL, "wait failed", "debug message", MB_OK | MB_ICONSTOP);
		goto Exit;
	case WAIT_OBJECT_0:
		break;
	case WAIT_ABANDONED:
		MessageBox(NULL, "wait abandoned by mutex", "debug message", MB_OK | MB_ICONSTOP);
		break;
	case WAIT_TIMEOUT:
		MessageBox(NULL, "wait time out occured", "debug message", MB_OK | MB_ICONSTOP);
		break;
	}
	
	strcpy(szFileName, "Offset.his");
	printf("Save Offset Image under filename:\n%s\n", szFileName);
	pFile = fopen(szFileName, "wb");
	if (pFile)
	{
		WinHeaderType FileHeader;
		int nDummy = 0;
		memset(&FileHeader, 0, sizeof(WinHeaderType));
		FileHeader.FileType=0x7000;
		FileHeader.HeaderSize = sizeof(WinHeaderType);
		FileHeader.ImageHeaderSize = 32;
		FileHeader.FileSize = FileHeader.HeaderSize+FileHeader.ImageHeaderSize+dwRows*dwColumns*sizeof(WORD);
		FileHeader.ULX = FileHeader.ULY = 1;
		FileHeader.BRX = dwColumns;
		FileHeader.BRY = dwRows;
		FileHeader.NrOfFrames = 1;
		FileHeader.TypeOfNumbers = 4;//DATASHORT;
		FileHeader.IntegrationTime = 0;
		fwrite(&FileHeader, sizeof(WinHeaderType), 1, pFile);
		fwrite(&nDummy, sizeof(nDummy), 8, pFile);
		fwrite(pOffsetBuffer, sizeof(WORD)*dwRows*dwColumns, 1, pFile);
		fclose(pFile);
	} else printf("Error creating Offset file!\n");

	printf("\nOffset corrected acquisition!\n");
	//set acquisition data to use it in callback
	Acquisition_SetAcqData(hAcqDesc, ACQ_CONT | ACQ_OFFSET);

	//route acquisition buffer to XISL again
	if ((nRet=Acquisition_DefineDestBuffers(hAcqDesc, pAcqBuffer,
		dwFrames, dwRows, dwColumns))!=HIS_ALL_OK)
	{
		//error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
		goto Exit;
	}

	//continuous acquisition
	if ((nRet=Acquisition_Acquire_Image(hAcqDesc, dwFrames, 0, HIS_SEQ_CONTINUOUS, pOffsetBuffer, NULL, NULL))!=HIS_ALL_OK)
	{
		//error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
		goto Exit;
	}
	
	FlushConsoleInputBuffer(hInput);

	do
	{
		WaitForSingleObject(hInput, INFINITE);
		ReadConsoleInput(hInput, &ir, 1, &dwRead);
	} while(!(ir.EventType==KEY_EVENT  && ir.Event.KeyEvent.bKeyDown));

	Acquisition_Abort(hAcqDesc);
	nRet = WaitForSingleObject(hevEndAcq, INFINITE);
	switch(nRet)
	{
	case WAIT_FAILED:
		MessageBox(NULL, "wait failed", "debug message", MB_OK | MB_ICONSTOP);
		goto Exit;
	case WAIT_OBJECT_0:
		break;
	case WAIT_ABANDONED:
		MessageBox(NULL, "wait abandoned by mutex", "debug message", MB_OK | MB_ICONSTOP);
		break;
	case WAIT_TIMEOUT:
		MessageBox(NULL, "wait time out occured", "debug message", MB_OK | MB_ICONSTOP);
		break;
	}

	//implementation of gain correction is similar
	//allocate memory for gain data
	pGainBuffer = malloc(dwRows*dwColumns*sizeof(DWORD));
	Acquisition_SetAcqData(hAcqDesc, ACQ_GAIN);
	printf("\nAcquire gain data now!\n");

	//acquire 17 bright frames and average them
	if ((nRet=Acquisition_Acquire_GainImage(hAcqDesc, pOffsetBuffer, pGainBuffer, dwRows, dwColumns, 17))!=HIS_ALL_OK)
	{
		//error handler
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
		goto Exit;
	}

	//wait for end of acquisition event
	assert(hevEndAcq);
	nRet = WaitForSingleObject(hevEndAcq, INFINITE);
	switch(nRet)
	{
	case WAIT_FAILED:
		MessageBox(NULL, "wait failed", "debug message", MB_OK | MB_ICONSTOP);
		goto Exit;
	case WAIT_OBJECT_0:
		break;
	case WAIT_ABANDONED:
		MessageBox(NULL, "wait abandoned by mutex", "debug message", MB_OK | MB_ICONSTOP);
		break;
	case WAIT_TIMEOUT:
		MessageBox(NULL, "wait time out occured", "debug message", MB_OK | MB_ICONSTOP);
		break;
	}

	printf("\nGain/Offset corrected acquisition!\n");
	//set acquisition data to use it in callback
	Acquisition_SetAcqData(hAcqDesc, ACQ_CONT);

	//route acquisition buffer to XISL again
	if ((nRet=Acquisition_DefineDestBuffers(hAcqDesc, pAcqBuffer,
		dwFrames, dwRows, dwColumns))!=HIS_ALL_OK)
	{
		//error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
		goto Exit;
	}

	//continuous acquisition
	if ((nRet=Acquisition_Acquire_Image(hAcqDesc, dwFrames, 0, HIS_SEQ_CONTINUOUS, pOffsetBuffer, pGainBuffer, NULL))!=HIS_ALL_OK)
	{
		//error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
		goto Exit;
	}
	
	FlushConsoleInputBuffer(hInput);

	do
	{
		WaitForSingleObject(hInput, INFINITE);
		ReadConsoleInput(hInput, &ir, 1, &dwRead);
	} while(ir.EventType!=KEY_EVENT);

	Acquisition_Abort(hAcqDesc);
	WaitForSingleObject(hevEndAcq, INFINITE);
//*/

	// build an use of the multi gain correction
	// acquire offset corrected bright images for each ROI
	// for example 2
	hevEndAcq = CreateEvent(NULL, FALSE, FALSE, NULL);
	Acquisition_SetCameraMode(hAcqDesc,0);
	Acquisition_SetFrameSyncMode(hAcqDesc,HIS_SYNCMODE_FREE_RUNNING);

	// 2 point multi gain
	// first aquire 2 offsetcorrected bright images. one for each intensity
	// and build a sequence of the data
	dwFrames=2;
	pGainSeqBuffer = malloc(dwFrames*dwRows*dwColumns*sizeof(short));

	// buffer for offsetcorrected bright image
	pBrightocBuffer=malloc(dwRows*dwColumns*sizeof(short));

	Acquisition_SetAcqData(hAcqDesc, ACQ_Brightoc);

	if ((nRet=Acquisition_DefineDestBuffers(hAcqDesc, pBrightocBuffer,
			1, dwRows, dwColumns))!=HIS_ALL_OK)
	{
		//error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
		goto Exit;
	}

	// acquire offsetcorrected-avg bright image with low intensity
	if((Acquisition_Acquire_Image(hAcqDesc,15,0, 
			HIS_SEQ_AVERAGE, NULL, NULL, NULL))!=HIS_ALL_OK)
	{
		//error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
		goto Exit;
	}
	//wait for end of acquisition event
	nRet = WaitForSingleObject(hevEndAcq, INFINITE);
	
	// copy acquired data to sequence buffer
	memcpy(pGainSeqBuffer,pBrightocBuffer,dwRows*dwColumns*sizeof(short)); 
	
	// acquire offsetcorrected-avg bright image with higher intensity
	if((Acquisition_Acquire_Image(hAcqDesc,15,0, 
			HIS_SEQ_AVERAGE, NULL, NULL, NULL))!=HIS_ALL_OK)
	{
		//error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
		goto Exit;
	}
	//wait for end of acquisition event
	nRet = WaitForSingleObject(hevEndAcq, INFINITE);

	// copy acquired data to sequence buffer
	memcpy(pGainSeqBuffer+dwRows*dwColumns,pBrightocBuffer,dwRows*dwColumns*sizeof(short));

	// build buffer for median data to be used with the multi-gain correction
	pGainSeqMedBuffer= malloc(2*sizeof(short));
	//create gain map
	Acquisition_CreateGainMap(pGainSeqBuffer,pGainSeqMedBuffer,dwRows*dwColumns,2);
	
	// now acquire image with offset and multigain correction
	dwFrames=1;
	if (pAcqBuffer)
		free (pAcqBuffer);
	pAcqBuffer = malloc(dwRows*dwColumns*sizeof(short));
	//route acquisition buffer to XISL
	if ((nRet=Acquisition_DefineDestBuffers(hAcqDesc, pAcqBuffer,
		dwFrames, dwRows, dwColumns))!=HIS_ALL_OK)
	{
		//error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
		goto Exit;
	}
	Acquisition_SetAcqData(hAcqDesc, ACQ_SNAP);
	if((Acquisition_Acquire_Image_Ex(hAcqDesc,30,0, 
			HIS_SEQ_AVERAGE,pOffsetBuffer,2,pGainSeqBuffer,pGainSeqMedBuffer,NULL,NULL))!=HIS_ALL_OK)
	{
		//error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
		goto Exit;
	}
	
	//wait for end of acquisition event
	nRet = WaitForSingleObject(hevEndAcq, INFINITE);


	printf("\nPixel corrected acquisition!\n");
	//now we want to perform a pixel correction of pixel 
	//(100, 22) and pixel (34, 56) by its eight neighbors
	pPixelBuffer = (DWORD *) malloc((10*2+1)*sizeof(DWORD));
	pPixelPtr = pPixelBuffer;
	*pPixelPtr = (100*dwRows+22);
	pPixelPtr++;
	*pPixelPtr = (99*dwRows+21);
	pPixelPtr++;
	*pPixelPtr = (99*dwRows+22);
	pPixelPtr++;
	*pPixelPtr = (99*dwRows+23);
	pPixelPtr++;
	*pPixelPtr = (100*dwRows+21);
	pPixelPtr++;
	*pPixelPtr = (100*dwRows+23);
	pPixelPtr++;
	*pPixelPtr = (101*dwRows+21);
	pPixelPtr++;
	*pPixelPtr = (101*dwRows+22)*sizeof(short);
	pPixelPtr++;
	*pPixelPtr = (101*dwRows+23);
	pPixelPtr++;
	*pPixelPtr = -1; //indicates end of list of 
						//correction pixels for pixel 
						//(100, 22)
	*pPixelPtr = (34*dwRows+56);
	pPixelPtr++;
	*pPixelPtr = (33*dwRows+55);
	pPixelPtr++;
	*pPixelPtr = (33*dwRows+56);
	pPixelPtr++;
	*pPixelPtr = (33*dwRows+57);
	pPixelPtr++;
	*pPixelPtr = (34*dwRows+55);
	pPixelPtr++;
	*pPixelPtr = (34*dwRows+57);
	pPixelPtr++;
	*pPixelPtr = (35*dwRows+55);
	pPixelPtr++;
	*pPixelPtr = (35*dwRows+56);
	pPixelPtr++;
	*pPixelPtr = (35*dwRows+57);
	pPixelPtr++;
	*pPixelPtr = -1; //indicates end of list of 
						//correction pixels for pixel 
						//(34, 56)
	pPixelPtr++;
	*pPixelPtr = -1; //indicates end of list of pixels
						//to correct

	if ((nRet=Acquisition_Acquire_Image(hAcqDesc, dwFrames, 0, HIS_SEQ_CONTINUOUS, pOffsetBuffer, pGainBuffer, pPixelBuffer))!=HIS_ALL_OK)
	{
		//error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
		goto Exit;
	}
	Sleep(200);
	if (!FlushConsoleInputBuffer(hInput)) printf("Console flush didn't work!");

	do
	{
		WaitForSingleObject(hInput, INFINITE);
		ReadConsoleInput(hInput, &ir, 1, &dwRead);
	} while(!(ir.EventType==KEY_EVENT && ir.Event.KeyEvent.bKeyDown));

	Acquisition_Abort(hAcqDesc);
	WaitForSingleObject(hevEndAcq, INFINITE);
//*/
Exit:
exit:
	//close acquisition and clean up
	//free event object
	if (hevEndAcq) CloseHandle(hevEndAcq);
	hevEndAcq = NULL;
	if (pAcqBuffer) free(pAcqBuffer);
	if (pOffsetBuffer) free(pOffsetBuffer);
	if (pGainBuffer) free(pGainBuffer);
	if (pPixelBuffer) free(pPixelBuffer);
	
	if ((nRet=Acquisition_CloseAll())!=HIS_ALL_OK)
	{
		//error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
		return 0;
	}

	return 0;
}