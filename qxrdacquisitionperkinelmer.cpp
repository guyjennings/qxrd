 /******************************************************************
*
*  $Id: qxrdacquisitionperkinelmer.cpp,v 1.48 2010/04/09 22:29:38 jennings Exp $
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
//#include <QMutexLocker>
#include "qxrdmutexlocker.h"
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
    m_FreeInt16Images("freeInt16Images"),
    m_FreeInt32Images("freeInt32Images"),
    m_PreTriggerInt16Images("preTriggerInt16Images"),
    m_PreTriggerInt32Images("preTriggerInt32Images"),
    m_PROMID(-1),
    m_HeaderID(-1),
    m_CameraType(-1),
    m_CameraModel(""),
    m_CurrentMode(-1),
    m_CurrentGain(-1),
    SOURCE_IDENT("$Id: qxrdacquisitionperkinelmer.cpp,v 1.48 2010/04/09 22:29:38 jennings Exp $")
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

//  emit printMessage(tr("QxrdAcquisitionPerkinElmer::acquire()"));
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

//  emit printMessage(tr("QxrdAcquisitionPerkinElmer::acquireDark()"));
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

//  prop_BinningMode() -> setDebug(1);

  set_BinningMode(0);

  emit printMessage("QxrdAcquisitionPerkinElmer::initialize()");

  int nRet = HIS_ALL_OK;
  UINT nSensors;
  BOOL bEnableIRQ = true;
  ACQDESCPOS Pos = 0;
  UINT nChannelType;
  int nChannelNr;
  UINT dwSortFlags, dwDataType, dwRows, dwColumns, dwFrames;
  DWORD dwAcqType, dwSystemID, dwSyncMode, dwHwAccess;

  nRet = Acquisition_EnumSensors(&nSensors, bEnableIRQ, FALSE);

  emit printMessage(tr("Acquisition_EnumSensors = %1").arg(nRet));

  if (nRet != HIS_ALL_OK) {
    acquisitionInitError(nRet);
    set_SimulationMode(true);
    simulatedInitialize();
    return;
  }

  emit printMessage(tr("Number of sensors = %1").arg(nSensors));

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
    acquisitionError(__LINE__, nRet);
    return;
  }

  if ((nRet=Acquisition_GetConfiguration(m_AcqDesc, &dwFrames, &dwRows, &dwColumns, &dwDataType,
                                         &dwSortFlags, &bEnableIRQ, &dwAcqType,
                                         &dwSystemID, &dwSyncMode, &dwHwAccess))!=HIS_ALL_OK) {
    acquisitionError(__LINE__, nRet);
    return;
  }

  set_NRows(dwRows);
  set_NCols(dwColumns);

  CHwHeaderInfo hwHeaderInfo;

  if ((nRet=Acquisition_GetHwHeaderInfo(m_AcqDesc, &hwHeaderInfo)) != HIS_ALL_OK) {
    acquisitionError(__LINE__, nRet);
    return;
  }

  emit printMessage(tr("Prom ID %1, Header ID %2")
                    .arg(hwHeaderInfo.dwPROMID)
                    .arg(hwHeaderInfo.dwHeaderID));

  m_PROMID = hwHeaderInfo.dwPROMID;
  m_HeaderID = hwHeaderInfo.dwHeaderID;

  if (hwHeaderInfo.dwHeaderID >= 14) {
    CHwHeaderInfoEx hdrx;

    if ((nRet = Acquisition_GetHwHeaderInfoEx(m_AcqDesc, &hwHeaderInfo, &hdrx)) != HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    emit printMessage(tr("Camera Type %1").arg(hdrx.wCameratype));

    m_CameraType = hdrx.wCameratype;
  }

  int nReadoutTimes = 8;
  double readoutTimes[8];

  if ((nRet=Acquisition_GetIntTimes(m_AcqDesc, readoutTimes, &nReadoutTimes)) != HIS_ALL_OK) {
    acquisitionError(__LINE__, nRet);
    return;
  }

  for (int i=0; i<nReadoutTimes; i++) {
    m_ReadoutTimes.append(readoutTimes[i]);

    emit oneReadoutModeChanged(i, readoutTimes[i]);
  }

  emit printMessage(tr("channel type: %1, ChannelNr: %2").arg(nChannelType).arg(nChannelNr));
  emit printMessage(tr("frames: %1").arg(dwFrames));
  emit printMessage(tr("rows: %1, columns: %2").arg(dwRows).arg(dwColumns));

  if ((nRet=Acquisition_SetCallbacksAndMessages(m_AcqDesc, NULL, 0,
                                                0, OnEndFrameCallback, OnEndAcqCallback))!=HIS_ALL_OK) {
    acquisitionError(__LINE__, nRet);
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
    int nFrames = qMin(get_FilesInAcquiredSequence()+get_PreTriggerFiles(), nMaxFrames);

    emit printMessage(tr("Preallocating %1 %2 x %3 16 bit images").arg(nFrames).arg(get_NCols()).arg(get_NRows()));

    m_FreeInt32Images.deallocate();
    m_PreTriggerInt32Images.deallocate();
    m_PreTriggerInt16Images.deallocate();
    delete m_AcquiredInt32Data; m_AcquiredInt32Data = NULL;
    delete m_AcquiredInt16Data; m_AcquiredInt16Data = NULL;


    m_FreeInt16Images.preallocate(nFrames, get_NCols(), get_NRows());

    emit printMessage(tr("Preallocated %1 %2 x %3 16 bit images").arg(nFrames).arg(get_NCols()).arg(get_NRows()));
  } else {
    int nMaxFrames = get_TotalBufferSize()/(get_NCols()*get_NRows()*sizeof(qint32));
    int nFrames = qMin(get_FilesInAcquiredSequence()+get_PreTriggerFiles(), nMaxFrames);

    emit printMessage(tr("Preallocating %1 %2 x %3 32 bit images").arg(nFrames).arg(get_NCols()).arg(get_NRows()));

    m_FreeInt16Images.deallocate();
    m_PreTriggerInt32Images.deallocate();
    m_PreTriggerInt16Images.deallocate();
    delete m_AcquiredInt32Data; m_AcquiredInt32Data = NULL;
    delete m_AcquiredInt16Data; m_AcquiredInt16Data = NULL;


    m_FreeInt32Images.preallocate(nFrames, get_NCols(), get_NRows());

    emit printMessage(tr("Preallocated %1 %2 x %3 32 bit images").arg(nFrames).arg(get_NCols()).arg(get_NRows()));
  }
}

void QxrdAcquisitionPerkinElmer::acquisition(int isDark)
{
  THREAD_CHECK;

  if (get_SimulationMode()) {
    simulatedAcquisition(isDark);
  } else {
    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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

    if ((nRet=Acquisition_SetCallbacksAndMessages(m_AcqDesc, NULL, 0,
                                                  0, OnEndFrameCallback, OnEndAcqCallback))!=HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

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

    allocateMemoryForAcquisition();

    m_Buffer.resize(get_NRows()*get_NCols()*m_BufferSize);
    m_Buffer.fill(0);

    if (m_CurrentMode != get_ReadoutMode()) {
      if ((nRet=Acquisition_SetCameraMode(m_AcqDesc, get_ReadoutMode())) != HIS_ALL_OK) {
        acquisitionError(__LINE__, nRet);
        return;
      }

      m_CurrentMode = get_ReadoutMode();
    }

    emit printMessage(tr("Readout time = %1, Exposure Time = %2")
                      .arg(readoutTime()).arg(get_ExposureTime()));

    if (readoutTime() >= get_ExposureTime()) {
      emit printMessage("SetFrameSyncMode HIS_SYNCMODE_FREE_RUNNING");
      if ((nRet=Acquisition_SetFrameSyncMode(m_AcqDesc, HIS_SYNCMODE_FREE_RUNNING)) != HIS_ALL_OK) {
        acquisitionError(__LINE__, nRet);
        return;
      }
    } else {
      emit printMessage("SetFrameSyncMode HIS_SYNCMODE_INTERNAL_TIMER");
      if ((nRet=Acquisition_SetFrameSyncMode(m_AcqDesc, HIS_SYNCMODE_INTERNAL_TIMER)) != HIS_ALL_OK) {
        acquisitionError(__LINE__, nRet);
        return;
      }

      DWORD tmp = (int)(get_ExposureTime()*1e6);
      emit printMessage(tr("SetTimerSync %1").arg(tmp));

      if ((nRet=Acquisition_SetTimerSync(m_AcqDesc, &tmp)) != HIS_ALL_OK) {
        acquisitionError(__LINE__, nRet);
        return;
      }

      emit printMessage(tr("TimerSync = %1").arg(tmp));

      set_ExposureTime(tmp/1.0e6);
    }

    emit printMessage("Setting camera gain");

    if (m_CurrentGain != get_CameraGain()) {
      if ((nRet=Acquisition_SetCameraGain(m_AcqDesc, get_CameraGain())) != HIS_ALL_OK) {
        acquisitionError(__LINE__, nRet);
        return;
      }
      m_CurrentGain = get_CameraGain();
    }

    emit printMessage("Set camera gain");

    if ((nRet=Acquisition_DefineDestBuffers(m_AcqDesc, m_Buffer.data(), m_BufferSize,
                                            get_NRows(), get_NCols())) != HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    emit printMessage("Define Dest Buffers");

    m_CurrentExposure = 0;
    m_CurrentFile = 0;

    if ((nRet=Acquisition_Acquire_Image(m_AcqDesc, m_BufferSize,
                                        0, HIS_SEQ_CONTINUOUS, NULL, NULL, NULL)) != HIS_ALL_OK) {
      acquisitionError(__LINE__, nRet);
      return;
    }

    emit printMessage("Acquire");
  }
}

void QxrdAcquisitionPerkinElmer::onEndFrame()
{
  QTime tic;
  tic.start();

  emit printMessage("QxrdAcquisitionPerkinElmer::onEndFrame()");

  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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
    emit printMessage(tr("actSecFrame %1, m_BufferIndex %2").arg(actSecFrame).arg(m_BufferIndex));
  }

//   printf("m_AcquiredImage.data() = %p\n", current);

  if (get_ExposuresToSum() == 1) {
    if (m_AcquiredInt16Data == NULL) {
      if (m_PreTriggerInt16Images.size() > get_PreTriggerFiles()) {
        m_AcquiredInt16Data = m_PreTriggerInt16Images.dequeue();
      }

      if (m_AcquiredInt16Data == NULL) {
        m_AcquiredInt16Data = m_FreeInt16Images.dequeue();
      }
    }

    if (m_AcquiredInt16Data != NULL) {
      quint16* current = m_AcquiredInt16Data->data();
      for (long i=0; i<npixels; i++) {
        *current = *frame;
        current++; frame++;
      }

      m_CurrentExposure++;
    } else {
      emit statusMessage("Frame dropped");
    }
  } else {
    if (m_AcquiredInt32Data == NULL) {
      if (m_PreTriggerInt32Images.size() > get_PreTriggerFiles()) {
        m_AcquiredInt32Data = m_PreTriggerInt32Images.dequeue();
      }

      if (m_AcquiredInt32Data == NULL) {
        m_AcquiredInt32Data = m_FreeInt32Images.dequeue();
      }

      m_AcquiredInt32Data -> clear();
    }

    if (m_AcquiredInt32Data != NULL) {
      quint32* current = m_AcquiredInt32Data->data();
      for (long i=0; i<npixels; i++) {
        *current += *frame;
        current++; frame++;
      }

      m_CurrentExposure++;
    } else {
      emit statusMessage("Frame dropped");
   }
  }

  m_BufferIndex++;


  if (m_BufferIndex >= m_BufferSize) {
    m_BufferIndex = 0;
  }

//  Acquisition_SetReady(m_AcqDesc, true);

  emit printMessage("Frame summed");

  if (m_CurrentExposure >= get_ExposuresToSum()) {
    m_CurrentExposure = 0;

    QFileInfo finfo(fileName);

    if (get_ExposuresToSum() == 1) {
      m_AcquiredInt16Data -> set_FileName(fileName);
      m_AcquiredInt16Data -> set_Title(finfo.fileName());
      m_AcquiredInt16Data -> set_ReadoutMode(get_ReadoutMode());
      m_AcquiredInt16Data -> set_ExposureTime(get_ExposureTime());
      m_AcquiredInt16Data -> set_SummedExposures(get_ExposuresToSum());
      m_AcquiredInt16Data -> set_DateTime(QDateTime::currentDateTime());
      m_AcquiredInt16Data -> set_HBinning(1);
      m_AcquiredInt16Data -> set_VBinning(1);
      m_AcquiredInt16Data -> set_CameraGain(get_CameraGain());
      m_AcquiredInt16Data -> set_DataType(QxrdInt16ImageData::Raw16Data);
      m_AcquiredInt16Data -> set_Triggered(get_Trigger());
      m_AcquiredInt16Data -> set_UserComment1(get_UserComment1());
      m_AcquiredInt16Data -> set_UserComment2(get_UserComment2());
      m_AcquiredInt16Data -> set_UserComment3(get_UserComment3());
      m_AcquiredInt16Data -> set_UserComment4(get_UserComment4());
      m_AcquiredInt16Data -> set_ImageSaved(false);

      copyDynamicProperties(m_AcquiredInt16Data);

      if (get_AcquireDark()) {
        m_AcquiredInt16Data -> set_ImageNumber(-1);
        acquiredInt16Image();
        haltAcquire();
      } else {        
        if (get_PreTriggerFiles() > 0) {
          if (get_Trigger()) {
            while (m_PreTriggerInt16Images.size() > 0) {
              QxrdInt16ImageData *img = m_PreTriggerInt16Images.dequeue();

              img -> set_ImageNumber(m_CurrentFile++);
              fileName = QDir(m_DataProcessor -> get_OutputDirectory())
                         .filePath(get_FilePattern()+tr("-%1.tif")
                         .arg(get_FileIndex(),5,10,QChar('0')));
              img -> set_FileName(fileName);
              img -> set_Title(QFileInfo(fileName).fileName());

              m_DataProcessor -> incrementAcquiredCount();
              emit acquiredInt16ImageAvailable(img);
              set_FileIndex(get_FileIndex()+1);
            }

            fileName = QDir(m_DataProcessor -> get_OutputDirectory())
                       .filePath(get_FilePattern()+tr("-%1.tif")
                       .arg(get_FileIndex(),5,10,QChar('0')));
            m_AcquiredInt16Data -> set_FileName(fileName);
            m_AcquiredInt16Data -> set_Title(QFileInfo(fileName).fileName());
            m_AcquiredInt16Data -> set_ImageNumber(m_CurrentFile++);
            acquiredInt16Image();
          } else {
            m_PreTriggerInt16Images.enqueue(m_AcquiredInt16Data);
            emit statusMessage(tr("%1 pre trigger 16 bit images queued").arg(m_PreTriggerInt16Images.size()));
            m_AcquiredInt16Data = NULL;
          }
        } else {
          m_AcquiredInt16Data -> set_ImageNumber(m_CurrentFile++);
          acquiredInt16Image();
        }
      }
    } else {
      m_AcquiredInt32Data -> set_FileName(fileName);
      m_AcquiredInt32Data -> set_Title(finfo.fileName());
      m_AcquiredInt32Data -> set_ReadoutMode(get_ReadoutMode());
      m_AcquiredInt32Data -> set_ExposureTime(get_ExposureTime());
      m_AcquiredInt32Data -> set_SummedExposures(get_ExposuresToSum());
      m_AcquiredInt32Data -> set_DateTime(QDateTime::currentDateTime());
      m_AcquiredInt32Data -> set_HBinning(1);
      m_AcquiredInt32Data -> set_VBinning(1);
      m_AcquiredInt32Data -> set_CameraGain(get_CameraGain());
      m_AcquiredInt32Data -> set_DataType(QxrdInt32ImageData::Raw32Data);
      m_AcquiredInt32Data -> set_Triggered(get_Trigger());
      m_AcquiredInt32Data -> set_UserComment1(get_UserComment1());
      m_AcquiredInt32Data -> set_UserComment2(get_UserComment2());
      m_AcquiredInt32Data -> set_UserComment3(get_UserComment3());
      m_AcquiredInt32Data -> set_UserComment4(get_UserComment4());
      m_AcquiredInt32Data -> set_ImageSaved(false);

      copyDynamicProperties(m_AcquiredInt32Data);

      if (get_AcquireDark()) {
        m_AcquiredInt32Data -> set_ImageNumber(-1);
        acquiredInt32Image();
        haltAcquire();
      } else {
        if (get_PreTriggerFiles() > 0) {
          if (get_Trigger()) {
            while (m_PreTriggerInt32Images.size() > 0) {
              QxrdInt32ImageData *img = m_PreTriggerInt32Images.dequeue();

              img -> set_ImageNumber(m_CurrentFile++);
              fileName = QDir(m_DataProcessor -> get_OutputDirectory())
                         .filePath(get_FilePattern()+tr("-%1.tif")
                         .arg(get_FileIndex(),5,10,QChar('0')));
              img -> set_FileName(fileName);
              img -> set_Title(QFileInfo(fileName).fileName());

              m_DataProcessor -> incrementAcquiredCount();
              emit acquiredInt32ImageAvailable(img);
              set_FileIndex(get_FileIndex()+1);
            }

            fileName = QDir(m_DataProcessor -> get_OutputDirectory())
                       .filePath(get_FilePattern()+tr("-%1.tif")
                       .arg(get_FileIndex(),5,10,QChar('0')));
            m_AcquiredInt32Data -> set_FileName(fileName);
            m_AcquiredInt32Data -> set_Title(QFileInfo(fileName).fileName());
            m_AcquiredInt32Data -> set_ImageNumber(m_CurrentFile++);
            acquiredInt32Image();
          } else {
            m_PreTriggerInt32Images.enqueue(m_AcquiredInt32Data);
            emit statusMessage(tr("%1 pre trigger 32 bit images queued").arg(m_PreTriggerInt32Images.size()));
            m_AcquiredInt32Data = NULL;
          }
        } else {
          m_AcquiredInt32Data -> set_ImageNumber(m_CurrentFile++);
          acquiredInt32Image();
        }
      }
    }

    emit statusMessage("Acquiring """+fileName+"""");

    if (m_CurrentFile >= (get_FilesInAcquiredSequence() + get_PreTriggerFiles())) {
      emit printMessage("Acquisition ended");
      emit printMessage("Halted acquisition");

      haltAcquire();

//      return true;
    }
  }

  emit printMessage(tr("QxrdAcquisitionPerkinElmer::onEndFrame %1 msec").arg(tic.elapsed()));
//  return false;
}

void QxrdAcquisitionPerkinElmer::acquiredInt16Image()
{
  m_DataProcessor -> incrementAcquiredCount();

  emit acquiredInt16ImageAvailable(m_AcquiredInt16Data);

  m_AcquiredInt16Data = NULL;

  if (!get_AcquireDark()) {
    set_FileIndex(get_FileIndex()+1);
  }
}

void QxrdAcquisitionPerkinElmer::acquiredInt32Image()
{
  m_DataProcessor -> incrementAcquiredCount();

  emit acquiredInt32ImageAvailable(m_AcquiredInt32Data);

  m_AcquiredInt32Data = NULL;

  if (!get_AcquireDark()) {
    set_FileIndex(get_FileIndex()+1);
  }
}

void QxrdAcquisitionPerkinElmer::haltAcquire()
{
//  if (QThread::currentThread() != thread()) {
//    printf("Ooops...\n");
//  }

  set_Cancelling(true);

  Acquisition_Abort(m_AcqDesc);

  emit statusMessage("Acquire Complete");
  emit acquireComplete(get_AcquireDark());

  m_Acquiring.tryLock();
  m_Acquiring.unlock();

  m_StatusWaiting.wakeAll();
}

//int QxrdAcquisitionPerkinElmer::status(double time)
//{
//  return acquisitionStatus(time);
//}
//
int QxrdAcquisitionPerkinElmer::acquisitionStatus(double time)
{
  if (m_Acquiring.tryLock()) {
    m_Acquiring.unlock();

//    printf("m_Acquiring.tryLock() succeeded\n");

    return 1;
  }

  QMutex mutex;
  QxrdMutexLocker lock(__FILE__, __LINE__, &mutex);

  if (m_StatusWaiting.wait(&mutex, (int)(time*1000))) {
//    printf("m_StatusWaiting.wait succeeded\n");
    return 1;
  } else {
//    printf("m_StatusWaiting.wait failed\n");
    return 0;
  }
}

int QxrdAcquisitionPerkinElmer::acquisitionCancel()
{
  haltAcquire();

  return 1;
}

void QxrdAcquisitionPerkinElmer::acquisitionError(int n)
{
  haltAcquire();

  emit criticalMessage(tr("Acquisition Error %1").arg(n));
}

void QxrdAcquisitionPerkinElmer::acquisitionError(int ln, int n)
{
  haltAcquire();

  emit criticalMessage(tr("Acquisition Error %1 at line %2").arg(n).arg(ln));
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
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_ReadoutTimes;
}

double QxrdAcquisitionPerkinElmer::readoutTime() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  int n = get_ReadoutMode();

  return m_ReadoutTimes.value(n)/1e6;
}

void QxrdAcquisitionPerkinElmer::setupCameraGainMenu(QComboBox *cb)
{
  cb -> addItem(tr("0.25 pF - Highest Gain"));
  cb -> addItem(tr("0.5 pF"));
  cb -> addItem(tr("1 pF"));
  cb -> addItem(tr("2 pF"));
  cb -> addItem(tr("4 pF"));
  cb -> addItem(tr("8 pF - Lowest Gain"));
}

void QxrdAcquisitionPerkinElmer::setupCameraBinningModeMenu(QComboBox *cb)
{
  cb -> addItem(tr("1x1 - 2048x2048 pixels"));
  cb -> addItem(tr("2x2 - 1024x1024 pixels"));
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

void QxrdAcquisitionPerkinElmer::doRawSaveBenchmark()
{
    THREAD_CHECK;

    int nImages = get_PostTriggerFiles();

    m_FreeInt16Images.preallocate(10, get_NCols(), get_NRows());
    m_AcquiredInt16Data = m_FreeInt16Images.dequeue();

    m_Acquiring.lock();

    emit statusMessage("Raw Save Benchmark starting");
    emit acquireStarted(0);

    for (int i=0; i<nImages; i++) {
        QString fileBase = get_FilePattern()+tr("-%1.raw.tif").arg(get_FileIndex(),5,10,QChar('0'));
        QString fileName = QDir(m_DataProcessor -> get_OutputDirectory())
                           .filePath(get_FilePattern()+tr("-%1.raw.tif")
                                     .arg(get_FileIndex(),5,10,QChar('0')));

        emit printMessage(tr("Image %1").arg(fileBase));

        m_AcquiredInt16Data -> set_FileName(fileName);
        m_AcquiredInt16Data -> set_Title(fileBase);
        m_AcquiredInt16Data -> set_ReadoutMode(get_ReadoutMode());
        m_AcquiredInt16Data -> set_ExposureTime(get_ExposureTime());
        m_AcquiredInt16Data -> set_SummedExposures(get_ExposuresToSum());
        m_AcquiredInt16Data -> set_DateTime(QDateTime::currentDateTime());
        m_AcquiredInt16Data -> set_HBinning(1);
        m_AcquiredInt16Data -> set_VBinning(1);
        m_AcquiredInt16Data -> set_CameraGain(get_CameraGain());
        m_AcquiredInt16Data -> set_DataType(QxrdInt16ImageData::Raw16Data);
        m_AcquiredInt16Data -> set_Triggered(get_Trigger());
        m_AcquiredInt16Data -> set_UserComment1(get_UserComment1());
        m_AcquiredInt16Data -> set_UserComment2(get_UserComment2());
        m_AcquiredInt16Data -> set_UserComment3(get_UserComment3());
        m_AcquiredInt16Data -> set_UserComment4(get_UserComment4());
        m_AcquiredInt16Data -> set_ImageSaved(false);

        m_DataProcessor -> saveNamedImageData(fileName, m_AcquiredInt16Data, false);

        set_FileIndex(get_FileIndex()+1);
    }

    m_FreeInt16Images.enqueue(m_AcquiredInt16Data);

    haltAcquire();
}

void QxrdAcquisitionPerkinElmer::doSubtractedSaveBenchmark()
{
}

void QxrdAcquisitionPerkinElmer::doRawSubtractedSaveBenchmark()
{
}

/******************************************************************
*
*  $Log: qxrdacquisitionperkinelmer.cpp,v $
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

