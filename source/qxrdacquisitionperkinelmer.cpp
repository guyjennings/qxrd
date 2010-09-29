/******************************************************************
*
*  $Id: qxrdacquisitionperkinelmer.cpp,v 1.4 2010/09/29 21:02:47 jennings Exp $
*
*******************************************************************/

//#ifdef HAVE_PERKIN_ELMER

#include "qxrdacquisitionperkinelmer.h"
//#include "qxrdacquisitionthread.h"
#include "qxrddataprocessor.h"
#include "qxrdapplication.h"
#include "qxrdimagedata.h"
#include "qxrdwindow.h"

//#ifdef Q_OS_UNIX
//#include "AcqLinuxTypes.h"
//#endif

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

#include <math.h>

#include "Acq.h"
#include <QThread>
#include <QDir>
#include <QFile>
#include <QTime>
//#include <QMutexLocker>
#include "qxrdmutexlocker.h"
#include <QMetaObject>
#include <QMetaProperty>
//#include "qxrdxislinterface.h"

static QxrdAcquisitionPerkinElmer * g_Acquisition = NULL;
static void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc);
static void CALLBACK OnEndAcqCallback(HACQDESC hAcqDesc);
static HACQDESC m_AcqDesc = NULL;

QxrdAcquisitionPerkinElmer::QxrdAcquisitionPerkinElmer(QxrdDataProcessorPtr proc, QxrdAllocatorPtr allocator)
  : QxrdAcquisition(proc, allocator),
    m_Mutex(QMutex::Recursive),
    m_BufferSize(0),
    m_PROMID(-1),
    m_HeaderID(-1),
    m_CameraType(-1),
    m_CameraModel(""),
    m_CurrentGain(-1),
    m_Counter(0),
    m_PerkinElmer(0),
    SOURCE_IDENT("$Id: qxrdacquisitionperkinelmer.cpp,v 1.4 2010/09/29 21:02:47 jennings Exp $")
{
  QCEP_DEBUG(DEBUG_PERKINELMER,
             printf("QxrdAcquisitionPerkinElmer::QxrdAcquisitionPerkinElmer\n");
  );

  ::g_Acquisition = this;
}

//QxrdAcquisitionPerkinElmer::~QxrdAcquisitionPerkinElmer()
//{
//}

bool QxrdAcquisitionPerkinElmer::checkPluginAvailable()
{
  if (m_PerkinElmer == NULL) {
    m_PerkinElmer = g_Application->perkinElmerPlugin();
  }

  if (m_PerkinElmer == NULL) {
    emit criticalMessage("No perkin elmer plugin available\n");
  }

  return m_PerkinElmer;
}

void QxrdAcquisitionPerkinElmer::onExposureTimeChanged(double newTime)
{
  if (checkPluginAvailable()) {

    QCEP_DEBUG(DEBUG_PERKINELMER,
               printf("Exposure time changed to %g\n", newTime);
    );

    emit printMessage(tr("Exposure time changed to %1").arg(newTime));

    DWORD tmp = (int)(get_ExposureTime()*1e6);

    emit printMessage(tr("SetTimerSync %1").arg(tmp));

    int nRet;

    if ((nRet=m_PerkinElmer->Acquisition_SetTimerSync(m_AcqDesc, &tmp)) != HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    emit printMessage(tr("TimerSync = %1").arg(tmp));

    set_ExposureTime(tmp/1.0e6);
  }
}

void QxrdAcquisitionPerkinElmer::onBinningModeChanged(int newMode)
{
  if (checkPluginAvailable()) {

    QCEP_DEBUG(DEBUG_PERKINELMER,
               printf("Binning mode changed to %d\n", newMode);
    );

    emit printMessage(tr("Binning mode changed to %1").arg(newMode));

    //    WORD binningMode = get_BinningMode();
    //    emit printMessage(tr("Setting binning mode = %1").arg(binningMode));
    //
    //    if ((nRet=Acquisition_SetCameraBinningMode(m_AcqDesc, binningMode)) != HIS_ALL_OK) {
    //      acquisitionError(__LINE__, nRet);
    //      return;
    //    }
    //
    //    if ((nRet=Acquisition_GetCameraBinningMode(m_AcqDesc, &binningMode)) != HIS_ALL_OK) {
    //      acquisitionError(__LINE__, nRet);
    //      return;
    //    }
    //
    //    emit printMessage(tr("Binning mode was set to %1").arg(binningMode));
  }
}

void QxrdAcquisitionPerkinElmer::onCameraGainChanged(int newGain)
{
  if (checkPluginAvailable()) {

    QCEP_DEBUG(DEBUG_PERKINELMER,
               printf("Camera gain changed to %d\n", newGain);
    );

    emit printMessage(tr("Camera Gain Changed to %1").arg(newGain));

    emit printMessage("Setting camera gain");

    int nRet;

    if (m_CurrentGain != get_CameraGain()) {
      if ((nRet=m_PerkinElmer->Acquisition_SetCameraGain(m_AcqDesc, get_CameraGain())) != HIS_ALL_OK) {
        acquisitionError(__LINE__, nRet);
        return;
      }
      m_CurrentGain = get_CameraGain();
    }

    emit printMessage("Set camera gain");
  }
}

void QxrdAcquisitionPerkinElmer::initialize()
{
  if (checkPluginAvailable()) {

    QCEP_DEBUG(DEBUG_PERKINELMER,
               printf("QxrdAcquisitionPerkinElmer::initialize\n");
    );

    THREAD_CHECK;

    int nRet = HIS_ALL_OK;
    UINT nSensors;
    BOOL bEnableIRQ = true;
    ACQDESCPOS Pos = 0;
    UINT nChannelType;
    int nChannelNr;
    UINT dwSortFlags, dwDataType, dwRows, dwColumns, dwFrames;
    DWORD dwAcqType, dwSystemID, dwSyncMode, dwHwAccess;

    nRet = m_PerkinElmer->Acquisition_EnumSensors(&nSensors, bEnableIRQ, FALSE);

    QCEP_DEBUG(DEBUG_PERKINELMER,
               printf("Acquisition_EnumSensors = %d\n",nRet);
    );

    if (nRet != HIS_ALL_OK) {
      acquisitionInitError(nRet);
      return;
    }

    QCEP_DEBUG(DEBUG_PERKINELMER,
               printf("Number of sensors = %d\n",nSensors);
    );

    if (nSensors != 1) {
      acquisitionNSensorsError(nRet);
      return;
    }

    if ((nRet = m_PerkinElmer->Acquisition_GetNextSensor(&Pos, &m_AcqDesc))!=HIS_ALL_OK) {
      acquisitionNSensorsError(nRet);
      return;
    }

    if ((nRet=m_PerkinElmer->Acquisition_GetCommChannel(m_AcqDesc, &nChannelType, &nChannelNr))!=HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    if ((nRet=m_PerkinElmer->Acquisition_GetConfiguration(m_AcqDesc, &dwFrames, &dwRows, &dwColumns, &dwDataType,
                                           &dwSortFlags, &bEnableIRQ, &dwAcqType,
                                           &dwSystemID, &dwSyncMode, &dwHwAccess))!=HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    set_NRows(dwRows);
    set_NCols(dwColumns);

    QxrdAcquisition::initialize();

    CHwHeaderInfo hwHeaderInfo;

    if ((nRet=m_PerkinElmer->Acquisition_GetHwHeaderInfo(m_AcqDesc, &hwHeaderInfo)) != HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    QCEP_DEBUG(DEBUG_PERKINELMER,
               printf("Prom ID %d, Header ID %d\n",
                      (int) hwHeaderInfo.dwPROMID,
                      (int) hwHeaderInfo.dwHeaderID);
    );

    m_PROMID = hwHeaderInfo.dwPROMID;
    m_HeaderID = hwHeaderInfo.dwHeaderID;

    if (hwHeaderInfo.dwHeaderID >= 14) {
      CHwHeaderInfoEx hdrx;

      if ((nRet = m_PerkinElmer->Acquisition_GetHwHeaderInfoEx(m_AcqDesc, &hwHeaderInfo, &hdrx)) != HIS_ALL_OK) {
        acquisitionError(__LINE__, nRet);
        return;
      }

      QCEP_DEBUG(DEBUG_PERKINELMER,
                 printf("Camera Type %d\n", hdrx.wCameratype);
      );

      m_CameraType = hdrx.wCameratype;
    }

    int nReadoutTimes = 8;
    double readoutTimes[8];

    if ((nRet=m_PerkinElmer->Acquisition_GetIntTimes(m_AcqDesc, readoutTimes, &nReadoutTimes)) != HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    for (int i=0; i<nReadoutTimes; i++) {
      m_ReadoutTimes.append(readoutTimes[i]);
    }

    QCEP_DEBUG(DEBUG_PERKINELMER,
               printf("channel type: %d, ChannelNr: %d\n",nChannelType,nChannelNr);
               printf("frames: %d rows: %d, columns: %d\n",dwFrames,dwRows,dwColumns);
    );

    if ((nRet=m_PerkinElmer->Acquisition_SetCallbacksAndMessages(m_AcqDesc, NULL, 0,
                                                  0, OnEndFrameCallback, OnEndAcqCallback))!=HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    m_BufferSize = 4;
    m_BufferIndex = 0;

    m_Buffer.resize(get_NRows()*get_NCols()*m_BufferSize);
    m_Buffer.fill(0);

    QCEP_DEBUG(DEBUG_PERKINELMER,
               printf("Exposure Time = %g\n", get_ExposureTime());
               printf("SetFrameSyncMode HIS_SYNCMODE_INTERNAL_TIMER\n");
    );

    if ((nRet=m_PerkinElmer->Acquisition_SetFrameSyncMode(m_AcqDesc, HIS_SYNCMODE_INTERNAL_TIMER)) != HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    set_BinningMode(0);

    if ((nRet=m_PerkinElmer->Acquisition_DefineDestBuffers(m_AcqDesc, m_Buffer.data(), m_BufferSize,
                                            get_NRows(), get_NCols())) != HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    QCEP_DEBUG(DEBUG_PERKINELMER,
               printf("Define Dest Buffers");
    );

    m_CurrentExposure = 0;
    m_CurrentFile = 0;

    if ((nRet=m_PerkinElmer->Acquisition_Acquire_Image(m_AcqDesc, m_BufferSize,
                                        0, HIS_SEQ_CONTINUOUS, NULL, NULL, NULL)) != HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    QCEP_DEBUG(DEBUG_PERKINELMER,
               printf("Acquire");
    );
  }
}

void QxrdAcquisitionPerkinElmer::onEndFrame(int counter, unsigned int n1, unsigned int n2)
{
  if (checkPluginAvailable()) {

    QTime tic;
    tic.start();

    QCEP_DEBUG(DEBUG_PERKINELMER,
               emit printMessage("QxrdAcquisitionPerkinElmer::onEndFrame()");
    );

    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  //  if (get_Cancelling()) {
  //    set_Cancelling(false);
  //    return /*true*/;
  //  }

    long npixels = get_NRows()*get_NCols();

    unsigned short* frame = m_Buffer.data() + m_BufferIndex*npixels;

    DWORD actualFrame = n1, actSecFrame = n2;

  //  this->Acquisition_GetActFrame(m_AcqDesc, &actualFrame, &actSecFrame);
  //
  //  if (n1 != actualFrame || n2 != actSecFrame) {
  //    printf("Lost frame, %d[%d], %d[%d]\n", n1, (int) actualFrame, n2, (int) actSecFrame);
  //    return;
  //  }

    int counter1 = m_Counter;

    if ((counter1 - counter) > 5) {
      printf("%d frames behind [%d,%d], skipping\n", counter1-counter, counter, counter1);
      return;
    }

    if (((actSecFrame-1)%m_BufferSize) != m_BufferIndex) {
      QCEP_DEBUG(DEBUG_PERKINELMER,
                 emit printMessage(tr("actSecFrame %1, m_BufferIndex %2").arg(actSecFrame).arg(m_BufferIndex));
      )
    }

    quint16* current = (m_AcquiredInt16Data ? m_AcquiredInt16Data->data() : NULL);
    quint32  cksum = 0;
    double   avg = 0;

    for (long i=0; i<npixels; i++) {
      unsigned short val = *frame++;
      cksum += val;
      avg += val;
      if (current) {
        *current++ = val;
      }
    }

  //  set_Average(avg/npixels);

    QCEP_DEBUG(DEBUG_PERKINELMER,
               emit printMessage(tr("Frame checksum 0x%1, avg %2\n").arg(cksum,8,16,QChar('0')).arg(avg/npixels));
    );

    m_BufferIndex = (m_BufferIndex+1)%m_BufferSize;

    acquiredFrameAvailable();
  }
}

void QxrdAcquisitionPerkinElmer::acquisitionInitError(int n)
{
  acquisitionError(n);

  emit criticalMessage("Detector Initialization Failed");
}

void QxrdAcquisitionPerkinElmer::acquisitionNSensorsError(int n)
{
  acquisitionError(n);

  emit criticalMessage("Detector Initialization Failed");
}

void QxrdAcquisitionPerkinElmer::setupExposureMenu(QDoubleSpinBox *cb)
{
//  foreach(double t, m_ReadoutTimes) {
//    cb -> addItem(tr("%1").arg(t/1e6,5,'f'));
//  }
}

void QxrdAcquisitionPerkinElmer::setupCameraGainMenu(QComboBox *cb)
{
  cb -> addItem(tr("High: 0.25 pF"));
  cb -> addItem(tr("0.5 pF"));
  cb -> addItem(tr("1 pF"));
  cb -> addItem(tr("2 pF"));
  cb -> addItem(tr("4 pF"));
  cb -> addItem(tr("Low: 8 pF"));
}

void QxrdAcquisitionPerkinElmer::setupCameraBinningModeMenu(QComboBox *cb)
{
  cb -> addItem(tr("1x1 - 2048x2048 pixels"));
  cb -> addItem(tr("2x2 - 1024x1024 pixels"));
}

void QxrdAcquisitionPerkinElmer::onEndFrameCallback()
{
  if (checkPluginAvailable()) {

    DWORD actualFrame, actSecFrame;

    m_PerkinElmer->Acquisition_GetActFrame(m_AcqDesc, &actualFrame, &actSecFrame);

    int counter = m_Counter.fetchAndAddOrdered(1);

    INVOKE_CHECK(QMetaObject::invokeMethod(g_Acquisition, "onEndFrame", Qt::QueuedConnection,
                                           Q_ARG(int, counter),
                                           Q_ARG(unsigned int, actualFrame), Q_ARG(unsigned int, actSecFrame)));
  }
}

static void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc)
{
//  INVOKE_CHECK(QMetaObject::invokeMethod(g_Acquisition, "onEndFrame", Qt::QueuedConnection));
  g_Acquisition->onEndFrameCallback();
}

static void CALLBACK OnEndAcqCallback(HACQDESC /*hAcqDesc*/)
{
}

//#endif // HAVE_PERKIN_ELMER

/******************************************************************
*
*  $Log: qxrdacquisitionperkinelmer.cpp,v $
*  Revision 1.4  2010/09/29 21:02:47  jennings
*  More tweaks to build system to try to get 'app' and 'nsis' targets to build
*
*  Revision 1.3  2010/09/23 19:57:32  jennings
*  Modified plugins for perkin elmer - now works in 64 bit mode
*
*  Revision 1.2  2010/09/13 20:00:38  jennings
*  Merged
*
*  Revision 1.1.2.3  2010/09/09 21:28:41  jennings
*  *** empty log message ***
*
*  Revision 1.1.2.2  2010/08/17 19:20:50  jennings
*  Added INVOKE_CHECK macro to check returned result QMetaObject::invokeMethod calls
*
*  Revision 1.1.2.1  2010/07/22 18:39:37  jennings
*  Moving files into source subdirectory
*
*  Revision 1.48.2.24  2010/06/04 21:27:17  jennings
*  Added __stdcall to function typedef in QxrdXISLInterface
*
*  Revision 1.48.2.23  2010/05/28 21:46:04  jennings
*  Converted acquisition startup messages to QCEP_DEBUG controlled printfs
*
*  Revision 1.48.2.22  2010/05/27 21:13:14  jennings
*  Added more debug flags
*  Tried to detect when processing is too slow for acquisition
*
*  Revision 1.48.2.21  2010/05/26 19:20:43  jennings
*  Modified interface to XISL.dll so that it is accessed via 'LoadLibrary' and
*  'GetProcAddress' rather than being explicitly linked.  Should allow
*  qxrd to run if the PE libraries are not present.
*
*  Revision 1.48.2.20  2010/05/26 15:41:38  jennings
*  Adding support for weak linking of XISL dll
*
*  Revision 1.48.2.19  2010/05/25 22:17:48  jennings
*  *** empty log message ***
*
*  Revision 1.48.2.18  2010/05/24 21:02:37  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.48.2.17  2010/05/21 20:04:41  jennings
*  Detector and Processor type names and numbers are intialized by the corresponding thread object
*
*  Revision 1.48.2.16  2010/05/21 18:51:22  jennings
*  *** empty log message ***
*
*  Revision 1.48.2.15  2010/05/19 21:15:34  jennings
*  PE acquisition debugging
*
*  Revision 1.48.2.14  2010/05/19 01:12:14  jennings
*  Tidied up PE acquisition code
*
*  Revision 1.48.2.13  2010/05/18 22:47:01  jennings
*  More perkin elmer readout fixes
*
*  Revision 1.48.2.12  2010/05/17 21:06:32  jennings
*  Various PE related mods
*
*  Revision 1.48.2.11  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.48.2.10  2010/04/24 19:38:40  jennings
*  Added HAVE_PERKIN_ELMER config symbol to conditionalise support for
*  Perkin Elmer detector.
*
*  Revision 1.48.2.9  2010/04/22 19:40:33  jennings
*  Further rearrangement of acquisition code
*
*  Revision 1.48.2.8  2010/04/21 20:51:21  jennings
*  Added detectorType and detectorTypeName properties to each detector type
*
*  Revision 1.48.2.7  2010/04/20 21:19:29  jennings
*  Added 'onExposureTimeChanged' etc. to PE acquisition
*
*  Revision 1.48.2.6  2010/04/19 21:48:26  jennings
*  More rearrangement
*
*  Revision 1.48.2.5  2010/04/19 19:20:16  jennings
*  More reorganization of acquisition code
*
*  Revision 1.48.2.4  2010/04/18 17:20:48  jennings
*  Further updates
*
*  Revision 1.48.2.3  2010/04/17 05:13:58  jennings
*  Partially working
*
*  Revision 1.48.2.2  2010/04/15 19:34:24  jennings
*  Reorganization of acquisition object classes
*
*  Revision 1.48.2.1  2010/04/12 21:21:46  jennings
*  Version 0.4.0, some rearrangement of the Acquisition classes
*
*  Revision 1.48  2010/04/09 22:29:38  jennings
*  Removed file browser, added CMake support, build under VC
*
*  Revision 1.47  2010/01/26 21:26:19  jennings
*  Don't increment file index when taking dark image
*
*  Revision 1.46  2009/12/11 17:49:04  jennings
*  Added 'ImageSaved' property to image data and used this to avoid double-saving raw data when
*  processing data off-line
*
*  Revision 1.45  2009/12/03 21:36:16  jennings
*  More output messages during acquisition
*
*  Revision 1.44  2009/11/17 20:42:59  jennings
*  Added instrumented QxrdMutexLocker which tracks how long locks are held, and prints
*  info about any held for more than 100 msec
*
*  Revision 1.43  2009/10/16 21:54:17  jennings
*  Implemented various processDataSequence commands
*
*  Revision 1.42  2009/09/28 22:06:09  jennings
*  Removed QxrdAcquisitionPerkinElmer::status
*
*  Revision 1.41  2009/09/25 19:57:48  jennings
*  Corrected a problem with acquired image summation caused by forgetting to clear
*  images before starting summation.
*
*  Revision 1.40  2009/09/22 21:42:12  jennings
*  Set image title as well a s filename property during acquisition
*
*  Revision 1.39  2009/09/22 20:37:23  jennings
*  Dynamic properties attached to the acquisition object are propagated through to saved images
*
*  Revision 1.38  2009/09/21 19:51:11  jennings
*  Added call to statusMessage to criticalMessage and call printMessage from statusMessage
*
*  Revision 1.37  2009/09/21 18:49:24  jennings
*  Accelerate readout by only calling 'SetCameraMode' and 'SetCameraGain' if they change
*
*  Revision 1.36  2009/09/21 18:12:55  jennings
*  Added 'triggered', 'usercomment{1-4}' properties to data
*
*  Revision 1.35  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
*
*  Revision 1.34  2009/09/18 20:44:49  jennings
*  Add separate status functions for acquisition and processing, as well as an aggregated function
*  combining the status of the two.
*
*  Revision 1.33  2009/09/15 20:18:39  jennings
*  Added acquireCancel scripting command
*
*  Revision 1.32  2009/09/14 19:08:57  jennings
*  Added more checks for appropriate data type / exposure etc. before subtracting
*  backgrounds
*
*  Revision 1.31  2009/09/11 19:39:27  jennings
*  Fixed missing line number substitution in acquisition error dialog
*
*  Revision 1.30  2009/09/10 21:33:02  jennings
*  Made acquisition errors show error dialog
*
*  Revision 1.29  2009/09/09 22:32:12  jennings
*  Started to add TIFF metadata support
*
*  Revision 1.28  2009/09/08 21:42:34  jennings
*  Added line numbers to acquisition error messages
*
*  Revision 1.27  2009/09/04 21:11:41  jennings
*  Support for file write timing tests
*
*  Revision 1.26  2009/09/04 20:04:31  jennings
*  Debugging pre-trigger acquisition
*
*  Revision 1.25  2009/09/04 15:15:42  jennings
*  Added log file routines
*  Removed newlines from any printMessage calls.
*
*  Revision 1.24  2009/09/04 12:46:35  jennings
*  Added binning mode parameter
*  Added camera gain and binning mode user interfaces
*
*  Revision 1.23  2009/09/04 02:44:15  jennings
*  Implement pre-trigger acquisition
*
*  Revision 1.22  2009/09/03 21:16:24  jennings
*  Added properties and user interface elements for pre- and post- trigger counts
*  Added properties and user interface elements for fine-grained control of processing chain
*
*  Revision 1.21  2009/08/27 21:02:17  jennings
*  Partial implementation of lazy plotting
*
*  Revision 1.20  2009/08/27 17:05:11  jennings
*  Added some model number detection code
*
*  Revision 1.19  2009/08/26 20:56:15  jennings
*  More Int16 and Int32 implementation
*
*  Revision 1.18  2009/08/26 16:58:53  jennings
*  Partial implementation of the separate Int16 and Int32 acquisition paths
*
*  Revision 1.17  2009/08/25 21:01:29  jennings
*  Added routine to check frame numbers to ensure that frames are not lost
*
*  Revision 1.16  2009/08/25 20:07:00  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.15  2009/08/25 18:43:03  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.14  2009/07/20 00:34:49  jennings
*  Send data between acquisition and data processor via signal/slot args, rather
*  than image queues
*
*  Revision 1.13  2009/07/17 14:00:59  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.12  2009/07/14 20:07:00  jennings
*  Implemented simple simulated acquisition
*
*  Revision 1.11  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.10  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.9  2009/07/09 01:15:09  jennings
*  Added some locks
*
*  Revision 1.8  2009/06/28 16:33:20  jennings
*  Eliminated compiler warnings
*
*  Revision 1.7  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

