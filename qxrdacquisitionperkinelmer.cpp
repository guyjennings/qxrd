/******************************************************************
*
*  $Id: qxrdacquisitionperkinelmer.cpp,v 1.22 2009/09/03 21:16:24 jennings Exp $
*
*******************************************************************/

#include "qxrdacquisitionperkinelmer.h"
//#include "qxrdacquisitionthread.h"
#include "qxrddataprocessor.h"
#include "qxrdapplication.h"
#include "qxrdimagedata.h"
#include "qxrdwindow.h"

#ifdef Q_OS_UNIX
#include "AcqLinuxTypes.h"
#endif

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

#include <math.h>

#include "Acq.h"
#include <QThread>
#include <QDir>
#include <QFile>
#include <QTime>
#include <QMutexLocker>
#include <QMetaObject>
#include <QMetaProperty>

static QxrdAcquisitionPerkinElmer * g_Acquisition = NULL;

QxrdAcquisitionPerkinElmer::QxrdAcquisitionPerkinElmer(QxrdDataProcessor *proc)
  : QxrdAcquisitionSimulated(proc),
    m_Mutex(QMutex::Recursive),
//    m_Cancelling(0),
//    m_AcquireDark(0),
//    m_NRows(0),
//    m_NCols(0),
//    m_ExposuresToSum(0),
    m_CurrentFile(0),
    m_BufferSize(0),
//    m_AcquiredData(NULL),
    m_AcquiredInt16Data(NULL),
    m_AcquiredInt32Data(NULL),
    SOURCE_IDENT("$Id: qxrdacquisitionperkinelmer.cpp,v 1.22 2009/09/03 21:16:24 jennings Exp $")
{
  ::g_Acquisition = this;
}

QxrdAcquisitionPerkinElmer::~QxrdAcquisitionPerkinElmer()
{
  if (m_AcquiredInt16Data) {
    delete m_AcquiredInt16Data;
  }

  if (m_AcquiredInt32Data) {
    delete m_AcquiredInt32Data;
  }


}

void QxrdAcquisitionPerkinElmer::acquire()
{
  THREAD_CHECK;

//  printf("QxrdAcquisitionPerkinElmer::acquire (thread()=%p, currentThread()=%p)\n",
//         thread(), QThread::currentThread());
  m_Acquiring.lock();

  emit printMessage(tr("QxrdAcquisitionPerkinElmer::acquire()"));
  emit statusMessage("Starting acquisition");
  emit acquireStarted(0);

  acquisition(0);
}

void QxrdAcquisitionPerkinElmer::acquireDark()
{
  THREAD_CHECK;

//  printf("QxrdAcquisitionPerkinElmer::acquireDark (thread()=%p, currentThread()=%p)\n",
//         thread(), QThread::currentThread());
  m_Acquiring.lock();

  emit printMessage(tr("QxrdAcquisitionPerkinElmer::acquireDark()\n"));
  emit statusMessage("Starting dark acquisition");
  emit acquireStarted(1);

  acquisition(1);
}

void QxrdAcquisitionPerkinElmer::cancel()
{
  haltAcquire();
}

void QxrdAcquisitionPerkinElmer::cancelDark()
{
  haltAcquire();
}

void QxrdAcquisitionPerkinElmer::trigger()
{
  set_Trigger(true);
}

static void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc);
static void CALLBACK OnEndAcqCallback(HACQDESC hAcqDesc);

static HACQDESC m_AcqDesc = NULL;

void QxrdAcquisitionPerkinElmer::initialize()
{
  THREAD_CHECK;

  emit printMessage("QxrdAcquisitionPerkinElmer::initialize()\n");

  int nRet = HIS_ALL_OK;
  UINT nSensors;
  BOOL bEnableIRQ = true;
  ACQDESCPOS Pos = 0;
  UINT nChannelType;
  int nChannelNr;
  UINT dwSortFlags, dwDataType, dwRows, dwColumns, dwFrames;
  DWORD dwAcqType, dwSystemID, dwSyncMode, dwHwAccess;

  nRet = Acquisition_EnumSensors(&nSensors, bEnableIRQ, FALSE);

  emit printMessage(tr("Acquisition_EnumSensors = %1\n").arg(nRet));

  if (nRet != HIS_ALL_OK) {
    acquisitionInitError(nRet);
    set_SimulationMode(true);
    simulatedInitialize();
    return;
  }

  emit printMessage(tr("Number of sensors = %1\n").arg(nSensors));

  if (nSensors != 1) {
    acquisitionNSensorsError(nRet);
    set_SimulationMode(true);
    simulatedInitialize();
    return;
  }

  if ((nRet = Acquisition_GetNextSensor(&Pos, &m_AcqDesc))!=HIS_ALL_OK) {
    acquisitionNSensorsError(nRet);
    set_SimulationMode(true);
    simulatedInitialize();
    return;
  }

  if ((nRet=Acquisition_GetCommChannel(m_AcqDesc, &nChannelType, &nChannelNr))!=HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  if ((nRet=Acquisition_GetConfiguration(m_AcqDesc, &dwFrames, &dwRows, &dwColumns, &dwDataType,
                                         &dwSortFlags, &bEnableIRQ, &dwAcqType,
                                         &dwSystemID, &dwSyncMode, &dwHwAccess))!=HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  set_NRows(dwRows);
  set_NCols(dwColumns);

  CHwHeaderInfo hwHeaderInfo;

  if ((nRet=Acquisition_GetHwHeaderInfo(m_AcqDesc, &hwHeaderInfo)) != HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  emit printMessage(tr("Prom ID %1, Header ID %2")
                    .arg(hwHeaderInfo.dwPROMID)
                    .arg(hwHeaderInfo.dwHeaderID));

  if (hwHeaderInfo.dwHeaderID >= 14) {
    CHwHeaderInfoEx hdrx;

    if ((nRet = Acquisition_GetHwHeaderInfoEx(m_AcqDesc, &hwHeaderInfo, &hdrx)) != HIS_ALL_OK) {
      acquisitionError(nRet);
      return;
    }

    emit printMessage(tr("Camera Type %1").arg(hdrx.wCameratype));
  }

  int nReadoutTimes = 8;
  double readoutTimes[8];

  if ((nRet=Acquisition_GetIntTimes(m_AcqDesc, readoutTimes, &nReadoutTimes)) != HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  for (int i=0; i<nReadoutTimes; i++) {
    m_ReadoutTimes.append(readoutTimes[i]);

    emit oneReadoutModeChanged(i, readoutTimes[i]);
  }

  emit printMessage(tr("channel type: %1, ChannelNr: %2\n").arg(nChannelType).arg(nChannelNr));
  emit printMessage(tr("frames: %1\n").arg(dwFrames));
  emit printMessage(tr("rows: %1\ncolumns: %2\n").arg(dwRows).arg(dwColumns));

  if ((nRet=Acquisition_SetCallbacksAndMessages(m_AcqDesc, NULL, 0,
                                                0, OnEndFrameCallback, OnEndAcqCallback))!=HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  for (int i=0; i<10; i++) {
    m_DataProcessor -> returnImageToPool(new QxrdDoubleImageData(get_NCols(), get_NRows()));
  }
}

void QxrdAcquisitionPerkinElmer::returnImageToPool(QxrdInt16ImageData *img)
{
  m_FreeInt16Images.enqueue(img);
}

void QxrdAcquisitionPerkinElmer::returnImageToPool(QxrdInt32ImageData *img)
{
  m_FreeInt32Images.enqueue(img);
}

void QxrdAcquisitionPerkinElmer::allocateMemoryForAcquisition()
{
  THREAD_CHECK;

  if (get_ExposuresToSum() == 1) {
    int nMaxFrames = get_TotalBufferSize()/(get_NCols()*get_NRows()*sizeof(quint16));
    int nFrames = qMin(get_FilesInAcquiredSequence(), nMaxFrames);

    printf("Preallocating %d %d x %d 16 bit images\n", nFrames, get_NCols(), get_NRows());

    m_FreeInt32Images.deallocate();
    m_PreTriggerInt32Images.deallocate();
    m_PreTriggerInt16Images.deallocate();
    delete m_AcquiredInt32Data; m_AcquiredInt32Data = NULL;
    delete m_AcquiredInt16Data; m_AcquiredInt16Data = NULL;


    m_FreeInt16Images.preallocate(nFrames, get_NCols(), get_NRows());

    printf("Preallocated %d %d x %d 16 bit images\n", nFrames, get_NCols(), get_NRows());
  } else {
    int nMaxFrames = get_TotalBufferSize()/(get_NCols()*get_NRows()*sizeof(qint32));
    int nFrames = qMin(get_FilesInAcquiredSequence(), nMaxFrames);

    printf("Preallocating %d %d x %d 16 bit images\n", nFrames, get_NCols(), get_NRows());

    m_FreeInt16Images.deallocate();
    m_PreTriggerInt32Images.deallocate();
    m_PreTriggerInt16Images.deallocate();
    delete m_AcquiredInt32Data; m_AcquiredInt32Data = NULL;
    delete m_AcquiredInt16Data; m_AcquiredInt16Data = NULL;


    m_FreeInt32Images.preallocate(nFrames, get_NCols(), get_NRows());

    printf("Preallocated %d %d x %d 16 bit images\n", nFrames, get_NCols(), get_NRows());
  }
}

void QxrdAcquisitionPerkinElmer::acquisition(int isDark)
{
  THREAD_CHECK;

  if (get_SimulationMode()) {
    simulatedAcquisition(isDark);
  } else {
    QMutexLocker lock(&m_Mutex);

    int nRet = HIS_ALL_OK;

    set_AcquireDark(isDark);
    set_Trigger(0);

    m_BufferSize = 4;
    m_BufferIndex = 0;

    set_Cancelling(0);

    if (get_AcquireDark()) {
      set_ExposuresToSum(get_DarkSummedExposures());
      set_FilesInAcquiredSequence(1);
    } else {
      set_ExposuresToSum(get_SummedExposures());
      set_FilesInAcquiredSequence(get_PostTriggerFiles());
    }

    if (get_FilesInAcquiredSequence ()<= 0) {
      set_FilesInAcquiredSequence(1);
    }

    allocateMemoryForAcquisition();

    m_Buffer.resize(get_NRows()*get_NCols()*m_BufferSize);
    m_Buffer.fill(0);

    if ((nRet=Acquisition_SetCallbacksAndMessages(m_AcqDesc, NULL, 0,
                                                  0, OnEndFrameCallback, OnEndAcqCallback))!=HIS_ALL_OK) {
      acquisitionError(nRet);
      return;
    }

    if ((nRet=Acquisition_SetCameraMode(m_AcqDesc, get_ReadoutMode())) != HIS_ALL_OK) {
      acquisitionError(nRet);
      return;
    }

    emit printMessage(tr("Readout time = %1, Exposure Time = %2")
                      .arg(readoutTime()).arg(get_ExposureTime()));

    if (readoutTime() >= get_ExposureTime()) {
      emit printMessage("SetFrameSyncMode HIS_SYNCMODE_FREE_RUNNING");
      if ((nRet=Acquisition_SetFrameSyncMode(m_AcqDesc, HIS_SYNCMODE_FREE_RUNNING)) != HIS_ALL_OK) {
        acquisitionError(nRet);
        return;
      }
    } else {
      emit printMessage("SetFrameSyncMode HIS_SYNCMODE_INTERNAL_TIMER");
      if ((nRet=Acquisition_SetFrameSyncMode(m_AcqDesc, HIS_SYNCMODE_INTERNAL_TIMER)) != HIS_ALL_OK) {
        acquisitionError(nRet);
        return;
      }

      DWORD tmp = (int)(get_ExposureTime()*1e6);
      emit printMessage(tr("SetTimerSync %1").arg(tmp));

      if ((nRet=Acquisition_SetTimerSync(m_AcqDesc, &tmp)) != HIS_ALL_OK) {
        acquisitionError(nRet);
        return;
      }

      emit printMessage(tr("TimerSync = %1").arg(tmp));

      set_ExposureTime(tmp/1.0e6);
    }

    if ((nRet=Acquisition_SetCameraGain(m_AcqDesc, get_CameraGain())) != HIS_ALL_OK) {
      acquisitionError(nRet);
      return;
    }

    if ((nRet=Acquisition_DefineDestBuffers(m_AcqDesc, m_Buffer.data(), m_BufferSize,
                                            get_NRows(), get_NCols())) != HIS_ALL_OK) {
      acquisitionError(nRet);
      return;
    }

    m_CurrentExposure = 0;
    m_CurrentFile = 0;

    if ((nRet=Acquisition_Acquire_Image(m_AcqDesc, m_BufferSize,
                                        0, HIS_SEQ_CONTINUOUS, NULL, NULL, NULL)) != HIS_ALL_OK) {
      acquisitionError(nRet);
      return;
    }
  }
}

void QxrdAcquisitionPerkinElmer::onEndFrame()
{
//  emit printMessage("QxrdAcquisitionPerkinElmer::onEndFrame()");
//
  QMutexLocker lock(&m_Mutex);

  if (get_Cancelling()) {
    set_Cancelling(false);
    return /*true*/;
  }

  QString fileName;
  QString fileBase;

  if (get_AcquireDark()) {
    fileBase = get_FilePattern()+tr("-%1.dark.tif").arg(get_FileIndex(),5,10,QChar('0'));
    fileName = QDir(m_DataProcessor -> get_OutputDirectory())
               .filePath(get_FilePattern()+tr("-%1.dark.tif")
                         .arg(get_FileIndex(),5,10,QChar('0')));
  } else {
    fileBase = get_FilePattern()+tr("-%1.tif").arg(get_FileIndex(),5,10,QChar('0'));
    fileName = QDir(m_DataProcessor -> get_OutputDirectory())
               .filePath(get_FilePattern()+tr("-%1.tif")
                         .arg(get_FileIndex(),5,10,QChar('0')));
  }

//  emit printMessage(tr("Fn: %1, Fi: %2, Exp: %3, Nexp %4, Fil: %5, NFil: %6")
//                    .arg(fileName).arg(fileIndex())
//                    .arg(m_CurrentExposure).arg(m_ExposuresToSum)
//                    .arg(m_CurrentFile).arg(m_FilesInSequence));

  set_FileBase(fileBase);
  m_DataProcessor -> set_FileName(fileName);

  emit acquiredFrame(fileName, get_FileIndex(),
                     m_CurrentExposure,get_ExposuresToSum(),
                     m_CurrentFile, get_FilesInAcquiredSequence());
  // sum current frame

  long npixels = get_NRows()*get_NCols();

  unsigned short* frame = m_Buffer.data() + m_BufferIndex*npixels;

  DWORD actualFrame, actSecFrame;

  Acquisition_GetActFrame(m_AcqDesc, &actualFrame, &actSecFrame);

  if (((actSecFrame-1)%m_BufferSize) != m_BufferIndex) {
    printf("actSecFrame %d, m_BufferIndex %d\n", actSecFrame, m_BufferIndex);
  }

//   printf("m_AcquiredImage.data() = %p\n", current);

//   QTime tic;
//   tic.start();

  if (get_ExposuresToSum() == 1) {
    if (m_AcquiredInt16Data == NULL) {
      m_AcquiredInt16Data = m_FreeInt16Images.dequeue();
    }

    if (m_AcquiredInt16Data != NULL) {
      quint16* current = m_AcquiredInt16Data->data();
      for (long i=0; i<npixels; i++) {
        *current = *frame;
        current++; frame++;
      }

      m_CurrentExposure++;
    } else {
      printf("Frame dropped\n");
    }
  } else {
    if (m_AcquiredInt32Data == NULL) {
      m_AcquiredInt32Data = m_FreeInt32Images.dequeue();
    }

    if (m_AcquiredInt32Data != NULL) {
      quint32* current = m_AcquiredInt32Data->data();
      for (long i=0; i<npixels; i++) {
        *current += *frame;
        current++; frame++;
      }

      m_CurrentExposure++;
    } else {
      printf("Frame dropped\n");
    }
  }

  m_BufferIndex++;

//    printf("Frame sum took %d msec\n", tic.elapsed());

  if (m_BufferIndex >= m_BufferSize) {
    m_BufferIndex = 0;
  }

//  Acquisition_SetReady(m_AcqDesc, true);

  if (m_CurrentExposure >= get_ExposuresToSum()) {
    m_CurrentExposure = 0;

    QFileInfo finfo(fileName);

    if (get_ExposuresToSum() == 1) {
      m_AcquiredInt16Data -> set_FileName(fileName);
      m_AcquiredInt16Data -> set_Title(finfo.fileName());
      m_AcquiredInt16Data -> set_ReadoutMode(get_ReadoutMode());
      m_AcquiredInt16Data -> set_ExposureTime(get_ExposureTime());
      m_AcquiredInt16Data -> set_SummedExposures(get_ExposuresToSum());

      if (get_AcquireDark()) {
        m_AcquiredInt16Data -> set_ImageNumber(-1);
      } else {
        m_AcquiredInt16Data -> set_ImageNumber(m_CurrentFile);
      }

      m_DataProcessor -> incrementAcquiredCount();

      emit acquiredInt16ImageAvailable(m_AcquiredInt16Data);

      m_AcquiredInt16Data = NULL;
    } else {
      m_AcquiredInt32Data -> set_FileName(fileName);
      m_AcquiredInt32Data -> set_Title(finfo.fileName());
      m_AcquiredInt32Data -> set_ReadoutMode(get_ReadoutMode());
      m_AcquiredInt32Data -> set_ExposureTime(get_ExposureTime());
      m_AcquiredInt32Data -> set_SummedExposures(get_ExposuresToSum());

      if (get_AcquireDark()) {
        m_AcquiredInt32Data -> set_ImageNumber(-1);
      } else {
        m_AcquiredInt32Data -> set_ImageNumber(m_CurrentFile);
      }

      m_DataProcessor -> incrementAcquiredCount();

      emit acquiredInt32ImageAvailable(m_AcquiredInt32Data);

      m_AcquiredInt32Data = NULL;
    }

    emit statusMessage("Saving "+fileName);
    emit printMessage("Saving """+fileName+"""");

    set_FileIndex(get_FileIndex()+1);
    m_CurrentFile++;

    if (m_CurrentFile >= get_FilesInAcquiredSequence()) {
      emit printMessage("Acquisition ended\n");
      emit printMessage("Aborted acquisition\n");

      haltAcquire();

//      return true;
    }
  }

//  return false;
}

void QxrdAcquisitionPerkinElmer::haltAcquire()
{
  if (QThread::currentThread() != thread()) {
    printf("Ooops...\n");
  }

  set_Cancelling(true);

  Acquisition_Abort(m_AcqDesc);

  emit statusMessage("Acquire Complete");
  emit acquireComplete(get_AcquireDark());

  m_Acquiring.tryLock();
  m_Acquiring.unlock();

  m_StatusWaiting.wakeAll();
}

int QxrdAcquisitionPerkinElmer::acquisitionStatus(double time)
{
  if (m_Acquiring.tryLock()) {
    m_Acquiring.unlock();

//    printf("m_Acquiring.tryLock() succeeded\n");

    return 1;
  }

  QMutex mutex;
  QMutexLocker lock(&mutex);

  if (m_StatusWaiting.wait(&mutex, (int)(time*1000))) {
//    printf("m_StatusWaiting.wait succeeded\n");
    return 1;
  } else {
//    printf("m_StatusWaiting.wait failed\n");
    return 0;
  }
}

void QxrdAcquisitionPerkinElmer::acquisitionError(int n)
{
  haltAcquire();

  emit printMessage(tr("Acquisition Error %1\n").arg(n));
  emit statusMessage(tr("Acquisition Error %1\n").arg(n));
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

void QxrdAcquisitionPerkinElmer::onEndFrameCallback()
{
//  m_AcquisitionWaiting.wakeAll();
}

QVector<double> QxrdAcquisitionPerkinElmer::readoutTimes()
{
  QMutexLocker lock(&m_Mutex);

  return m_ReadoutTimes;
}

double QxrdAcquisitionPerkinElmer::readoutTime() const
{
  QMutexLocker lock(&m_Mutex);

  int n = get_ReadoutMode();

  return m_ReadoutTimes.value(n)/1e6;
}

static void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc)
{
//  DWORD actualFrame, actSecFrame;
//
//  Acquisition_GetActFrame(hAcqDesc, &actualFrame, &actSecFrame);
//
//  printf("Actual frame %d, sec frame %d\n", actualFrame, actSecFrame);
//
//  printf("OnEndFrameCallback\n");

//  g_Acquisition -> onEndFrameCallback();

  QMetaObject::invokeMethod(g_Acquisition, "onEndFrame", Qt::QueuedConnection);
//  QTimer::singleShot(0, , SLOT(onEndFrame()));
}

static void CALLBACK OnEndAcqCallback(HACQDESC /*hAcqDesc*/)
{
}


/******************************************************************
*
*  $Log: qxrdacquisitionperkinelmer.cpp,v $
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

