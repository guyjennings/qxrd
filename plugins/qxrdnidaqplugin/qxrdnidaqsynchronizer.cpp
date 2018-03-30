#include "qxrdnidaqsynchronizer.h"
#include "NIDAQmx.h"
#include <QVector>
#include <QElapsedTimer>
#include <stdio.h>
#include <QMutexLocker>
#include <QStringList>
#include <QThread>
#include "qxrdsynchronizedacquisition.h"
#include "qxrdsynchronizeddetectorchannel.h"
#include "qxrdsynchronizedoutputchannel.h"
#include "qxrdsynchronizedinputchannel.h"
#include "qxrdacqcommon.h"
#include "qxrdacquisitionparameterpack.h"
#include "qxrddetectorsettings.h"
#include "qxrddebug.h"
#include <QMetaObject>

#define DAQmxErrChk(functionCall) do { int error; \
                                       if( DAQmxFailed(error=(functionCall)) ) { \
                                           QxrdNIDAQSynchronizer::errorCheck(__FILE__,__LINE__,error); \
                                           goto Error; \
                                       } } while(0)

QxrdNIDAQSynchronizer::QxrdNIDAQSynchronizer(QString name)
  : inherited(name),
    m_SyncTask(0),
    m_SyncAOTask(0),
    m_SyncAITask(0),
    m_PrimaryCounterName(""),
    m_PrimaryTriggerName(""),
    m_ExposureTime(0),
    m_SyncCounter(0),
    m_SyncLongTime(0),
    m_SyncNPhases(1),
    m_OutputNSamples(0),
    m_InputNSamples(0),
    m_DeviceCount(0),
    m_DetectorDeviceCount(0),
    m_OutputDeviceCount(0),
    m_InputDeviceCount(0)
{
}

QxrdNIDAQSynchronizer::~QxrdNIDAQSynchronizer()
{
  THREAD_CHECK;

  closeTaskHandles();
}

void QxrdNIDAQSynchronizer::initialize(QcepObjectWPtr parent)
{
  THREAD_CHECK;

  inherited::initialize(parent);

  printMessage("QxrdNIDAQPlugin::initialize");

  m_DeviceNames = deviceNames();

  m_DeviceCount = m_DeviceNames.count();

  for (int i=0; i<m_DeviceCount; i++) {
    QString deviceName = m_DeviceNames.value(i);

//    if (!deviceIsSimulated(deviceName)) {
      QStringList counterChans = deviceCOChannels(deviceName);
      QStringList outputChans  = deviceAOChannels(deviceName);
      QStringList inputChans   = deviceAIChannels(deviceName);

      m_DetectorDeviceNames.append(counterChans);
      m_OutputDeviceNames.append(outputChans);
      m_InputDeviceNames.append(inputChans);
//    }
  }

  m_DetectorDeviceCount = m_DetectorDeviceNames.count();
  m_OutputDeviceCount   = m_OutputDeviceNames.count();
  m_InputDeviceCount    = m_InputDeviceNames.count();

  m_Acquisition =
      QxrdAcqCommon::findAcquisition(parent);

  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq) {
    m_SynchronizedAcquisition = acq->synchronizedAcquisition();
  } else {
    printMessage(tr("QxrdNIDAQSynchronizer::initialize acquisition == NULL"));
  }

  QxrdSynchronizedAcquisitionPtr sync(m_SynchronizedAcquisition);

  if (sync == NULL) {
    printMessage(tr("QxrdNIDAQSynchronizer::initialize syncAcquisition == NULL"));
  }
}

QString QxrdNIDAQSynchronizer::name() const
{
  return "NIDAQ Synchronizer";
}

void QxrdNIDAQSynchronizer::stopSynchronizer()
{
  THREAD_CHECK;

  closeTaskHandles();
}

void QxrdNIDAQSynchronizer::startSynchronizer()
{
  THREAD_CHECK;

  closeTaskHandles();

  changeExposureTime(m_ExposureTime, m_SyncNPhases);

  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq) {
    QxrdAcquisitionParameterPackPtr parms(acq->acquisitionParameterPack());

    if (parms) {
      updateSyncWaveforms(m_SynchronizedAcquisition, parms);
    }
  }
}

void QxrdNIDAQSynchronizer::errorCheck(const char* file, int line, int err)
{
  if (DAQmxFailed(err)) {
    int szx = DAQmxGetExtendedErrorInfo(NULL, 0);

    if (szx > 0) {
      char *buff = (char*) malloc(szx);

      if (DAQmxGetExtendedErrorInfo(buff, szx) == 0) {
        printMessage(tr("%1:%2 NI-DAQ Error %3 : %4").arg(file).arg(line).arg(err).arg(buff));
      }

      free(buff);
    }
  }
}

void QxrdNIDAQSynchronizer::closeTaskHandles()
{
  THREAD_CHECK;

  clearSync();
  clearDetectorSync();
  clearOutputChannels();
  clearInputChannels();
}

//double QxrdNIDAQSynchronizer::aiGet(QString chan)
//{
//  initTaskHandles();

//  int error;
//  float64 res = 0;

//  DAQmxErrChk(DAQmxReadAnalogScalarF64(m_AITaskHandle, 10.0, &res, NULL))

//Error:
//  return res;
//  return 0;
//}

//double QxrdNIDAQSynchronizer::getAnalogInput(int chan)
//{
//  QMutexLocker lock(&m_Mutex);

//  float64 res = 0;

//  if (m_AITaskHandle) {
//    DAQmxStopTask(m_AITaskHandle);
//    DAQmxClearTask(m_AITaskHandle);
//    m_AITaskHandle = 0;
//  }

//  if (chan >= 0) {
//    DAQmxErrChk(DAQmxCreateTask("qxrd-input", &m_AITaskHandle));
//    DAQmxErrChk(DAQmxCreateAIVoltageChan (m_AITaskHandle,
//                                          qPrintable(tr("Dev1/ai%1").arg(chan)), NULL, DAQmx_Val_Cfg_Default, -10.0, 10.0, DAQmx_Val_Volts, NULL));

//    if (m_AITaskHandle) {
//      DAQmxErrChk(DAQmxReadAnalogScalarF64(m_AITaskHandle, 10.0, &res, NULL));
//    }
//  }

//Error:
//  return res;
//}

//int QxrdNIDAQSynchronizer::prepareContinuousInput(double sampleRate,
//                                            double acquireDelay,
//                                            double exposureTime,
//                                            QStringList chans,
//                                            QVector<double> minVals,
//                                            QVector<double> maxVals)
//{
//  int bufferSize = 0;

//  m_SampleRate = sampleRate;
//  m_AcquireDelay = acquireDelay;
//  m_ExposureTime = exposureTime;

//  finishContinuousInput();

//  QRegExp re_ai("^(.*)/ai\\d+$");
//  QRegExp re_ci("^(.*)/ctr\\d+$");
//  QString aiDevice;

//  if (m_ContinuousAITask == NULL) {
//    DAQmxErrChk(DAQmxCreateTask("continuousAI", &m_ContinuousAITask));
//  }

//  for(int i=0; i<chans.count(); i++) {
//    QString chan = chans.value(i);

//    if (re_ai.exactMatch(chan)) {

//      DAQmxErrChk(DAQmxCreateAIVoltageChan(m_ContinuousAITask, qPrintable(chan), "",
//                                           DAQmx_Val_Cfg_Default,
//                                           minVals.value(i),
//                                           maxVals.value(i),
//                                           DAQmx_Val_Volts, NULL ));

//      m_ContinuousFlags.append(1);
//      m_ContinuousChans.append(m_NAIChannels++);

//      if (re_ai.capturedTexts().count() >= 2) {
//        if (aiDevice.count() == 0) {
//          aiDevice = re_ai.capturedTexts().value(1);
//        }
//      }

//      printMessage(tr("Analog input channel : %1").arg(chan));
//    } else if (re_ci.exactMatch(chan)) {
//      TaskHandle thdl;

//      DAQmxErrChk(DAQmxCreateTask(qPrintable(tr("continuousCI_%1").arg(m_ContinuousCITasks.count())), &thdl));

//      m_ContinuousCITasks.append(thdl);

//      DAQmxErrChk(DAQmxCreateCICountEdgesChan(thdl, qPrintable(chan), "", DAQmx_Val_Rising, 0, DAQmx_Val_CountUp));

//      m_ContinuousFlags.append(-1);
//      m_ContinuousChans.append(m_NCIChannels++);

//      if (re_ci.capturedTexts().count() >= 2) {
//        if (aiDevice.count() == 0) {
//          aiDevice = re_ci.capturedTexts().value(1);
//        }
//      }

//      printMessage(tr("Counter input channel : %1").arg(chan));
//    } else {
//      m_ContinuousFlags.append(0);
//      m_ContinuousChans.append(0);

//      printMessage(tr("Skipped channel : %1").arg(chan));
//    }
//  }

//  if (m_NAIChannels == 0) {
//    DAQmxErrChk(DAQmxCreateAIVoltageChan(m_ContinuousAITask,
//                                         qPrintable(tr("%1/ai0").arg(aiDevice)), "",
//                                         DAQmx_Val_Cfg_Default, -10.0, 10.0, DAQmx_Val_Volts, NULL ));

//  }

//  m_NContinuousSamples = m_SampleRate*(m_ExposureTime + m_AcquireDelay + 0.1);

//  bufferSize = m_NContinuousSamples+(10.0*m_SampleRate);

//  printMessage(tr("Buffer size %1").arg(bufferSize));

//  if (m_ContinuousAITask) {
//    DAQmxErrChk(DAQmxCfgSampClkTiming(m_ContinuousAITask, NULL,
//                                      sampleRate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, bufferSize));
//    DAQmxErrChk(DAQmxSetBufInputBufSize(m_ContinuousAITask, bufferSize));
//    DAQmxErrChk(DAQmxSetReadOverWrite(m_ContinuousAITask, DAQmx_Val_OverwriteUnreadSamps));
//  }

//  foreach (TaskHandle tsk, m_ContinuousCITasks) {
//    DAQmxErrChk(DAQmxCfgSampClkTiming(tsk, qPrintable(tr("/%1/ai/SampleClock").arg(aiDevice)),
//                                      sampleRate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, bufferSize));
//    DAQmxErrChk(DAQmxSetBufInputBufSize(tsk, bufferSize));
//    DAQmxErrChk(DAQmxSetReadOverWrite(tsk, DAQmx_Val_OverwriteUnreadSamps));
//    //    DAQmxErrChk(DAQmxCfgDigEdgeStartTrig(tsk, qPrintable(tr("/%1/ai/StartTrigger").arg(aiDevice)),
//    //                                         DAQmx_Val_Rising));
//  }

//  foreach (TaskHandle tsk, m_ContinuousCITasks) {
//    DAQmxErrChk(DAQmxStartTask(tsk));
//  }

//  if (m_ContinuousAITask) {
//    DAQmxErrChk(DAQmxStartTask(m_ContinuousAITask));
//  }

//  m_NContinuousInputs = chans.count();

//  m_ContinuousInputData.resize(m_NContinuousInputs);

//  for (int i=0; i<m_NContinuousInputs; i++) {
//    m_ContinuousInputData[i].resize(m_NContinuousSamples);
//  }
//Error:

//  return 0;
//}

//int QxrdNIDAQPlugin::readContinuousInput()
//{
//  uInt32 avail;

//  QVector<double> aiBuff(m_NAIChannels*m_NContinuousSamples);
//  QVector< QVector<double> > ciBuff(m_NCIChannels);

//  for (int i=0; i<m_NCIChannels; i++) {
//    ciBuff[i].resize(m_NContinuousSamples + 1);
//  }

//  if (m_ContinuousAITask) {
//    DAQmxErrChk(DAQmxGetReadAvailSampPerChan(m_ContinuousAITask, &avail));

////    printMessage(tr("%1 samples available").arg(avail));

//    if (avail < m_NContinuousSamples) {
//      return -1;
//    }

//    DAQmxErrChk(DAQmxSetReadRelativeTo(m_ContinuousAITask, DAQmx_Val_MostRecentSamp));
//    DAQmxErrChk(DAQmxSetReadOffset(m_ContinuousAITask, -m_NContinuousSamples));
//  }

////  foreach(TaskHandle tsk, m_ContinuousCITasks) {
////    DAQmxErrChk(DAQmxSetReadRelativeTo(tsk, DAQmx_Val_MostRecentSamp));
////    DAQmxErrChk(DAQmxSetReadOffset(tsk, -m_NContinuousSamples));
////  }

//  int32 actuallyRead;
//  uInt64 lastSample[30];

//  if (m_ContinuousAITask && m_NAIChannels) {
//    DAQmxErrChk(DAQmxReadAnalogF64(m_ContinuousAITask, m_NContinuousSamples, -1,
//                                   DAQmx_Val_GroupByChannel, aiBuff.data(), aiBuff.count(),
//                                   &actuallyRead, NULL));

//    DAQmxErrChk(DAQmxGetReadCurrReadPos(m_ContinuousAITask, &lastSample[0]));
//  }

//  for(int i=0; i<m_NCIChannels; i++) {
//    TaskHandle tsk = m_ContinuousCITasks.value(i);

//    if (tsk) {
//      uInt64 currentPos;

//      DAQmxErrChk(DAQmxGetReadCurrReadPos(tsk, &currentPos));
//      DAQmxErrChk(DAQmxSetReadRelativeTo(tsk, DAQmx_Val_CurrReadPos));
//      int32 offset = ((int64)lastSample[0]) - ((int64)currentPos) - (m_NContinuousSamples+1);
//      DAQmxErrChk(DAQmxSetReadOffset(tsk, offset));
//      DAQmxErrChk(DAQmxReadCounterF64(tsk, m_NContinuousSamples+1, -1,
//                                      ciBuff[i].data(), ciBuff[i].count(),
//                                      &actuallyRead, NULL));

//      if (i<29) {
//        DAQmxErrChk(DAQmxGetReadCurrReadPos(tsk, &lastSample[i+1]));

//        if (lastSample[i+1] != lastSample[i]) {
//          printf("Sync error %lld:%lld\n", lastSample[i+1], lastSample[i]);
//        }
//      }
//    }
//  }

//  for (int i=0; i<m_NCIChannels; i++) {
//    for (int j=0; j<m_NContinuousSamples; j++) {
//      ciBuff[i][j] = ciBuff[i][j+1] - ciBuff[i][j];
//    }

//    ciBuff[i].resize(m_NContinuousSamples);
//  }

//  for (int i=0; i<m_NContinuousInputs; i++) {
//    if (m_ContinuousFlags.value(i) == 1) { // an Analog Input Channel
//      int chan = m_ContinuousChans.value(i);

//      m_ContinuousInputData[i] = aiBuff.mid(chan*m_NContinuousSamples, m_NContinuousSamples);
//    } else if (m_ContinuousFlags.value(i) == -1) { // a counter Input Channel
//      int chan = m_ContinuousChans.value(i);

//      m_ContinuousInputData[i] = ciBuff[chan].mid(0, m_NContinuousSamples);
//    }
//  }
//Error:
//  return 0;
//}

QStringList QxrdNIDAQSynchronizer::deviceNames()
{
  char buffer[5120]="";

  DAQmxErrChk(DAQmxGetSysDevNames(buffer, sizeof(buffer)));
  //  printf("%d: DAQmxGetSysDevNames : \"%s\"\n", res, buffer);

Error:
  QStringList result = QString(buffer).split(", ");

  return result;
}

int QxrdNIDAQSynchronizer::deviceIsSimulated(QString device)
{
  bool32 res = true;

  DAQmxErrChk(DAQmxGetDevIsSimulated(qPrintable(device), &res));

Error:
  return res;
}

QStringList QxrdNIDAQSynchronizer::deviceAIChannels(QString device)
{
  char buffer[5120]="";

  DAQmxErrChk(DAQmxGetDevAIPhysicalChans(qPrintable(device), buffer, sizeof(buffer)));
  //  printf("%d: DAQmxGetDevAIPhysicalChans : \"%s\"\n", res, buffer);

Error:
  QStringList result = QString(buffer).split(", ");

  return result;
}

QStringList QxrdNIDAQSynchronizer::deviceAOChannels(QString device)
{
  char buffer[5120]="";

  DAQmxErrChk(DAQmxGetDevAOPhysicalChans(qPrintable(device), buffer, sizeof(buffer)));

Error:
  QStringList result = QString(buffer).split(", ");

  return result;
}

QStringList QxrdNIDAQSynchronizer::deviceCOChannels(QString device)
{
  char buffer[5120]="";

  DAQmxErrChk(DAQmxGetDevCOPhysicalChans(qPrintable(device), buffer, sizeof(buffer)));

Error:
  QStringList result = QString(buffer).split(", ");

  return result;
}

static int32 CVICALLBACK syncCallback(TaskHandle taskHandle, int32 status, void *callbackData)
{
  QxrdNIDAQSynchronizer *syncro = reinterpret_cast<QxrdNIDAQSynchronizer*>(callbackData);

  if (syncro) {
    INVOKE_CHECK(
          QMetaObject::invokeMethod(syncro,
                                    [=]() { syncro -> syncCallback(taskHandle, status); }));
  }

  return 0;
}

static int32 CVICALLBACK aiCallback(TaskHandle taskHandle, int32 everyNSamplesEventType, uInt32 nSamples, void *callbackData)
{
  QxrdNIDAQSynchronizer *syncro = reinterpret_cast<QxrdNIDAQSynchronizer*>(callbackData);

  if (syncro) {
    INVOKE_CHECK(
          QMetaObject::invokeMethod(syncro,
                                    [=]() { syncro->aiCallback(taskHandle, everyNSamplesEventType, nSamples); }));
  }

  return 0;
}

//static int32 CVICALLBACK aoCallback(TaskHandle taskHandle, int32 status, void *callbackData)
//{
//  QxrdNIDAQPlugin *plugin = reinterpret_cast<QxrdNIDAQPlugin*>(callbackData);

//  if (plugin) {
//    return plugin->aoCallback(taskHandle, status);
//  } else {
//    return status;
//  }
//}

void QxrdNIDAQSynchronizer::clearSync()
{
  THREAD_CHECK;

  if (m_SyncTask) {
    if (qcepDebug(DEBUG_NIDAQ)) {
      printMessage(tr("Stopping NIDAQ sync channel"));
    }

    DAQmxErrChk(DAQmxClearTask(m_SyncTask));
    m_SyncTask = NULL;
  }

Error:
  return;
}

void QxrdNIDAQSynchronizer::clearDetectorSync()
{
  THREAD_CHECK;

  if (m_SyncDetTasks.count() > 0) {
    if (qcepDebug(DEBUG_NIDAQ)) {
      printMessage(tr("Stopping %1 NIDAQ detector sync channel(s)").arg(m_SyncDetTasks.count()));
    }

    while (m_SyncDetTasks.count()) {
      DAQmxErrChk(DAQmxClearTask(m_SyncDetTasks.takeFirst()));
    }

    m_DetectorChannels.clear();
  }

Error:
  return;
}

void QxrdNIDAQSynchronizer::clearOutputChannels()
{
  THREAD_CHECK;

  if (m_SyncAOTask) {
    if (qcepDebug(DEBUG_NIDAQ)) {
      printMessage(tr("Stopping NIDAQ output channel(s)"));
    }

    DAQmxErrChk(DAQmxClearTask(m_SyncAOTask));
    m_SyncAOTask = NULL;
    m_OutputChannels.clear();
    m_OutputNSamples = 0;
  }

Error:
  return;
}

void QxrdNIDAQSynchronizer::clearInputChannels()
{
  THREAD_CHECK;

  if (m_SyncAITask) {
    if (qcepDebug(DEBUG_NIDAQ)) {
      printMessage(tr("Stopping NIDAQ input channel(s)"));
    }

    DAQmxErrChk(DAQmxClearTask(m_SyncAITask));
    m_SyncAITask = NULL;
    m_InputChannels.clear();
    m_InputNSamples = 0;
  }

Error:
  return;
}

void QxrdNIDAQSynchronizer::changeExposureTime(double t, int n)
{
  THREAD_CHECK;

  m_ExposureTime = t;
  m_SyncNPhases  = n;
  m_SyncLongTime = m_ExposureTime*m_SyncNPhases;

  if (m_SyncTask == NULL) {
    DAQmxErrChk(DAQmxCreateTask("sync", &m_SyncTask));

    DAQmxErrChk(DAQmxCreateCOPulseChanTime(m_SyncTask,
                                           qPrintable(m_PrimaryCounterName),
                                           "",
                                           DAQmx_Val_Seconds,
                                           DAQmx_Val_Low,
                                           0.0,
                                           m_SyncLongTime / 2.0 + 0.001,
                                           m_SyncLongTime / 2.0
                                           ));

    DAQmxErrChk(DAQmxCfgImplicitTiming(m_SyncTask, DAQmx_Val_ContSamps, 100));
    DAQmxErrChk(DAQmxRegisterSignalEvent(m_SyncTask,
                                         DAQmx_Val_CounterOutputEvent,
                                         0 /*DAQmx_Val_SynchronousEventCallbacks*/,
                                         &::syncCallback,
                                         this));
  } else if (qcepDebug(DEBUG_NIDAQ)) {
    printMessage("QxrdNIDAQPlugin::changeExposureTime when sync task already running");
  }

Error:
  return;
}

void QxrdNIDAQSynchronizer::addDetectorSync(QxrdSynchronizedDetectorChannelWPtr p)
{
  THREAD_CHECK;

  QxrdSynchronizedDetectorChannelPtr sync(p);
  QxrdAcqCommonPtr acq(QxrdAcqCommon::findAcquisition(p));

  if (sync && acq) {
    int i = sync->get_DetectorNumber();

    QxrdDetectorSettingsPtr det = acq->detector(i);

    if (det) {
      int n = m_SyncDetTasks.count();

      QString taskName = tr("syncDet%1").arg(n);

      int relExpos = det->get_ExposureFactor();

      int     chNum    = sync->get_ChannelNumber();
      QString chanName = tr("ctr%1").arg(chNum);
      QString coName   = sync->channelName();
      double  rDelay   = det -> get_ReadoutDelay();

      if (qcepDebug(DEBUG_NIDAQ)) {
        printMessage(tr("Adding NIDAQ detector sync channel %1").arg(m_SyncDetTasks.count()));
        printMessage(tr("TaskName %1, Device Name %2, Channel Name %3, Detector Number %4, Exposure Time %5, Relative Exposure %6, Readout Delay %7")
                     .arg(taskName)
                     .arg(coName)
                     .arg(chanName)
                     .arg(i)
                     .arg(m_ExposureTime)
                     .arg(relExpos)
                     .arg(rDelay));
      }

      TaskHandle detTask;
      DAQmxErrChk(DAQmxCreateTask(qPrintable(taskName), &detTask));

      m_SyncDetTasks.append(detTask);
      m_DetectorChannels.append(p);

      double dp = m_ExposureTime / relExpos;

      DAQmxErrChk(DAQmxCreateCOPulseChanTime(detTask,
                                             qPrintable(coName),
                                             ""/*qPrintable(chanName)*/,
                                             DAQmx_Val_Seconds,
                                             DAQmx_Val_Low,
                                             0.0,
                                             rDelay,
                                             dp - rDelay));

      if (m_SyncNPhases*relExpos > 1) {
        DAQmxErrChk(DAQmxCfgImplicitTiming  (detTask,
                                             DAQmx_Val_FiniteSamps,
                                             m_SyncNPhases*relExpos));
      }

      DAQmxErrChk(DAQmxCfgDigEdgeStartTrig(detTask,
                                           qPrintable(m_PrimaryTriggerName),
                                           DAQmx_Val_Rising));

      DAQmxErrChk(DAQmxSetStartTrigRetriggerable(detTask,
                                                 true));
    }
  }

Error:
  return;
}

void QxrdNIDAQSynchronizer::addOutputChannel(QxrdSynchronizedOutputChannelWPtr p)
{
  THREAD_CHECK;

  QxrdSynchronizedOutputChannelPtr out(p);
  QxrdAcqCommonPtr                 acq(QxrdAcqCommon::findAcquisition(p));

  if (out && acq) {
    QxrdSynchronizedAcquisitionPtr sync(acq->synchronizedAcquisition());

    if (sync) {
      int n = m_OutputChannels.count();

      QString chanName = tr("aoTask%1").arg(n);
      QString aoName   = out->channelName();

      if (n == 0) {
        m_OutputNSamples = out->get_NSamples();
      }

      if (qcepDebug(DEBUG_NIDAQ)) {
        printMessage(tr("Adding NIDAQ output channel %1").arg(n));
        printMessage(tr("Task name aoTask, Device Name %1, Channel Name %2, Sample Rate %3, nSamples %4")
                     .arg(aoName)
                     .arg(chanName)
                     .arg(sync->get_OutputSampleRate())
                     .arg(m_OutputNSamples));
      }

      if (m_SyncAOTask == NULL) {
        DAQmxErrChk(DAQmxCreateTask("aoTask", &m_SyncAOTask));
      }

      m_OutputChannels.append(p);

      DAQmxErrChk(DAQmxCreateAOVoltageChan(m_SyncAOTask,
                                           qPrintable(aoName),
                                           ""/*qPrintable(chanName)*/,
                                           -10.0,
                                           10.0,
                                           DAQmx_Val_Volts,
                                           NULL));

      if (n == 0) {
        DAQmxErrChk(DAQmxCfgSampClkTiming(m_SyncAOTask,
                                          NULL,
                                          sync->get_OutputSampleRate(),
                                          DAQmx_Val_Rising,
                                          DAQmx_Val_FiniteSamps,
                                          m_OutputNSamples));


        DAQmxErrChk(DAQmxCfgDigEdgeStartTrig(m_SyncAOTask,
                                             qPrintable(m_PrimaryTriggerName),
                                             DAQmx_Val_Rising));

        DAQmxErrChk(DAQmxSetStartTrigRetriggerable(m_SyncAOTask,
                                                   true));
      }
    }
  }

Error:
  return;
}

void QxrdNIDAQSynchronizer::addInputChannel(QxrdSynchronizedInputChannelWPtr p)
{
  THREAD_CHECK;

  QxrdSynchronizedInputChannelPtr inp(p);
  QxrdAcqCommonPtr acq(QxrdAcqCommon::findAcquisition(p));

  if (inp && acq) {
    QxrdSynchronizedAcquisitionPtr sync(acq->synchronizedAcquisition());

    if (sync) {
      int n = m_InputChannels.count();

      QString chanName = tr("ai%1").arg(n);
      QString aiName   = inp->channelName();

      if (n == 0) {
        m_InputNSamples = inp->get_NSamples();
      }

      double sampleRate = sync->get_InputSampleRate();
      int    bufferSize = m_InputNSamples*3 + 5*int(sampleRate);

      if (qcepDebug(DEBUG_NIDAQ)) {
        printMessage(tr("Adding NIDAQ input channel %1").arg(n));
        printMessage(tr("Task name aiTask, Device Name %1, Channel Name %2, Sample Rate %3, NSamples %4, Buffer %5")
                     .arg(aiName)
                     .arg(chanName)
                     .arg(sampleRate)
                     .arg(m_InputNSamples)
                     .arg(bufferSize));
      }

      if (m_SyncAITask == NULL) {
        DAQmxErrChk(DAQmxCreateTask("aiTask", &m_SyncAITask));
      }

      m_InputChannels.append(p);

      DAQmxErrChk(DAQmxCreateAIVoltageChan(m_SyncAITask,
                                           qPrintable(aiName),
                                           ""/*qPrintable(chanName)*/,
                                           DAQmx_Val_Cfg_Default,
                                           -10.0,
                                           10.0,
                                           DAQmx_Val_Volts,
                                           NULL));

      if (n == 0) {
        DAQmxErrChk(DAQmxCfgSampClkTiming(m_SyncAITask,
                                          NULL,
                                          sampleRate,
                                          DAQmx_Val_Rising,
                                          DAQmx_Val_FiniteSamps,
                                          m_InputNSamples));

        DAQmxErrChk(DAQmxCfgDigEdgeStartTrig(m_SyncAITask,
                                             qPrintable(m_PrimaryTriggerName),
                                             DAQmx_Val_Rising));

        DAQmxErrChk(DAQmxSetBufInputBufSize(m_SyncAITask,
                                            bufferSize));

        DAQmxErrChk(DAQmxSetReadOverWrite(m_SyncAITask,
                                          DAQmx_Val_OverwriteUnreadSamps));

        DAQmxErrChk(DAQmxSetStartTrigRetriggerable(m_SyncAITask, true));

        DAQmxErrChk(DAQmxRegisterEveryNSamplesEvent(m_SyncAITask,
                                                    DAQmx_Val_Acquired_Into_Buffer,
                                                    m_InputNSamples,
                                                    0 /*DAQmx_Val_SynchronousEventCallbacks*/,
                                                    &::aiCallback,
                                                    this));
      }
    }
  }

Error:
  return;
}

void QxrdNIDAQSynchronizer::startSync()
{
  THREAD_CHECK;

  if (m_SyncTask) {
    if (qcepDebug(DEBUG_NIDAQ)) {
      printMessage(tr("Starting NIDAQ sync"));
    }

    DAQmxErrChk(DAQmxStartTask(m_SyncTask));
  }

Error:
  return;
}

void QxrdNIDAQSynchronizer::startDetectorSync()
{
  THREAD_CHECK;

  if (m_SyncDetTasks.count() > 0) {
    if (qcepDebug(DEBUG_NIDAQ)) {
      printMessage(tr("Starting %1 NIDAQ detector sync").arg(m_SyncDetTasks.count()));
    }

    foreach (TaskHandle task, m_SyncDetTasks) {
      DAQmxErrChk(DAQmxStartTask(task));
    }
  }

Error:
  return;
}

void QxrdNIDAQSynchronizer::startOutputChannels()
{
  THREAD_CHECK;

  if (m_SyncAOTask) {

    int32 nWritten;

    QVector<double> waveForms;

    for (int i=0; i<m_OutputChannels.count(); i++) {
      QxrdSynchronizedOutputChannelPtr out(m_OutputChannels.value(i));

      if (out) {
        waveForms.append(out->get_Waveform());
      }
    }

    DAQmxErrChk(DAQmxWriteAnalogF64(m_SyncAOTask,
                                    m_OutputNSamples,
                                    false,
                                    -1,
                                    DAQmx_Val_GroupByChannel,
                                    waveForms.data(),
                                    &nWritten,
                                    NULL));

    if (qcepDebug(DEBUG_NIDAQ)) {
      printMessage(tr("Starting NIDAQ output channels"));
    }

    DAQmxErrChk(DAQmxStartTask(m_SyncAOTask));
  }

Error:
  return;
}

void QxrdNIDAQSynchronizer::startInputChannels()
{
  THREAD_CHECK;

  if (m_SyncAITask) {
    if (qcepDebug(DEBUG_NIDAQ)) {
      printMessage(tr("Starting NIDAQ input channels"));
    }

    DAQmxErrChk(DAQmxStartTask(m_SyncAITask));
  }

Error:
  return;
}

int32 QxrdNIDAQSynchronizer::syncCallback(TaskHandle task, int32 status)
{
  THREAD_CHECK;

  QxrdAcqCommonPtr acq(m_Acquisition);


  int32 state;

  DAQmxErrChk(DAQmxGetCOOutputState(task, "/Dev1/ctr0", &state));

  if (state == DAQmx_Val_Low) {
    state = 0;
  } else if (state == DAQmx_Val_High) {
    state = 1;
  }

  if (acq) {
    acq -> appendEvent(QxrdAcqCommon::NIDAQSyncEvent, state);
  }

  m_SyncCounter += 1;

//  if (qcepDebug(DEBUG_NIDAQ)) {
//    printMessage(tr("QxrdNIDAQSynchronizer::syncCallback %1 = %2, state = %3")
//                 .arg(status)
//                 .arg(m_SyncCounter)
//                 .arg(state));
//  }

//  if (m_SyncCounter && (state == 1)) {
//    syncReadAnalogInputs();
//  }

Error:
  return status;
}

int32 QxrdNIDAQSynchronizer::aiCallback(TaskHandle task, int32 eventType, int32 nSamples)
{
  THREAD_CHECK;

  QxrdAcqCommonPtr               acq(m_Acquisition);
  QxrdSynchronizedAcquisitionPtr sync(m_SynchronizedAcquisition);
  QVector<double>                buff;

  uInt32 available;
  DAQmxErrChk(DAQmxGetReadAvailSampPerChan(task, &available));

  if (acq) {
    acq -> appendEvent(QxrdAcqCommon::NIDAQAnalogInputEvent, available);
  }

  int nChans = m_InputChannels.count();

  buff.resize(nChans*m_InputNSamples);

  int32 actuallyRead;

  DAQmxErrChk(DAQmxReadAnalogF64(task,
                                 m_InputNSamples,
                                 -1,
                                 DAQmx_Val_GroupByChannel,
                                 buff.data(),
                                 buff.count(),
                                 &actuallyRead,
                                 NULL));

  for (int i=0; i<nChans; i++) {
    QxrdSynchronizedInputChannelPtr inp(m_InputChannels.value(i));

    if (inp) {
      inp->set_Waveform(buff.mid(i*m_InputNSamples, m_InputNSamples));
    }
  }

  if (sync) {
    sync -> waveformsChanged();
  }

  if (acq) {
    acq -> appendEvent(QxrdAcqCommon::NIDAQAnalogPostEvent, actuallyRead);
  }

Error:
  return eventType;
}

//int32 QxrdNIDAQPlugin::aoCallback(TaskHandle task, int32 status)
//{
////  printMessage(tr("aoCallback %1").arg(status));

//  return status;
//}

int QxrdNIDAQSynchronizer::detectorDeviceCount()
{
  return m_DetectorDeviceCount;
}

QString QxrdNIDAQSynchronizer::detectorDeviceName(int n)
{
  return m_DetectorDeviceNames.value(n);
}

int QxrdNIDAQSynchronizer::outputDeviceCount()
{
  return m_OutputDeviceCount;
}

QString QxrdNIDAQSynchronizer::outputDeviceName(int n)
{
  return m_OutputDeviceNames.value(n);
}

int QxrdNIDAQSynchronizer::inputDeviceCount()
{
  return m_InputDeviceCount;
}

QString QxrdNIDAQSynchronizer::inputDeviceName(int n)
{
  return m_InputDeviceNames.value(n);
}

void QxrdNIDAQSynchronizer::updateSyncWaveforms(QxrdSynchronizedAcquisitionWPtr s,
                                                QxrdAcquisitionParameterPackWPtr p)
{
  THREAD_CHECK;

  QxrdAcquisitionParameterPackPtr parm(p);
  QxrdSynchronizedAcquisitionPtr  sync(s);

  if (qcepDebug(DEBUG_NIDAQ)) {
    printMessage("QxrdNIDAQSynchronizer::updateSyncWaveforms");
  }

  if (sync && parm) {
    m_SynchronizedAcquisition = sync;
    m_Acquisition = sync->acquisition();

    QxrdAcqCommonPtr acq(m_Acquisition);

    if (acq) {
      acq->appendEvent(QxrdAcqCommon::NIDAQStartEvent);
    }

    m_PrimaryCounterName = "/" + sync->primaryCounterName();
    m_PrimaryTriggerName = m_PrimaryCounterName + "InternalOutput";

    bool changedExposure = (parm->exposure() != m_ExposureTime ||
                            parm->nphases()  != m_SyncNPhases);

    if (changedExposure) {
      clearSync();
      changeExposureTime(parm->exposure(), parm->nphases());
    }

    clearDetectorSync();
    clearOutputChannels();
    clearInputChannels();

    for (int i=0; i<sync->detectorCount(); i++) {
      addDetectorSync(sync->detector(i));
    }

    for (int i=0; i<sync->outputCount(); i++) {
      addOutputChannel(sync->output(i));
    }

    for (int i=0; i<sync->inputCount(); i++) {
      addInputChannel(sync->input(i));
    }

    startInputChannels();
    startOutputChannels();
    startDetectorSync();

    if (changedExposure) {
      startSync();
    }
  }

  if (qcepDebug(DEBUG_NIDAQ)) {
    dumpTask(m_SyncTask);
    for (int i=0; i<sync->detectorCount(); i++) {
      dumpTask(m_SyncDetTasks.value(i));
    }
    dumpTask(m_SyncAOTask);
    dumpTask(m_SyncAITask);
  }
}

void QxrdNIDAQSynchronizer::prepareForIdling(QxrdSynchronizedAcquisitionWPtr s,
                                             QxrdAcquisitionParameterPackWPtr p)
{
  THREAD_CHECK;

  printMessage("QxrdNIDAQSynchronizer::prepareForIdling");

  updateSyncWaveforms(s, p);
}

void QxrdNIDAQSynchronizer::prepareForAcquisition(QxrdSynchronizedAcquisitionWPtr s,
                                                  QxrdAcquisitionParameterPackWPtr p)
{
  THREAD_CHECK;

  printMessage("QxrdNIDAQSynchronizer::prepareForAcquisition");

  updateSyncWaveforms(s, p);
}

void QxrdNIDAQSynchronizer::prepareForDarkAcquistion(QxrdSynchronizedAcquisitionWPtr s,
                                                     QxrdDarkAcquisitionParameterPackWPtr p)
{
  THREAD_CHECK;

  printMessage("QxrdNIDAQSynchronizer::prepareForDarkAcquisition");

//  updateSyncWaveforms(s, p);
}

void QxrdNIDAQSynchronizer::dumpTask(TaskHandle t)
{
  THREAD_CHECK;

  if (t) {
    char taskName[256];
    uInt32 numChans;
    char chanNames[512];
    uInt32 numDevs;
    char devNames[1024];
    int32  trigType;
    char trigTerm[256];
    char trigSrc[256];

    DAQmxErrChk(DAQmxGetTaskName(t, taskName, 256));
    DAQmxErrChk(DAQmxGetTaskNumChans(t, &numChans));
    DAQmxErrChk(DAQmxGetTaskChannels(t, chanNames, 512));
    DAQmxErrChk(DAQmxGetTaskNumDevices(t, &numDevs));
    DAQmxErrChk(DAQmxGetTaskDevices(t, devNames, 1024));
    DAQmxErrChk(DAQmxGetStartTrigType(t, &trigType));
    DAQmxErrChk(DAQmxGetStartTrigTerm(t, trigTerm, 256));
    DAQmxErrChk(DAQmxGetDigEdgeStartTrigSrc(t, trigSrc, 256));

    printMessage(tr("Task %1: %2 channels : %3").arg(taskName).arg(numChans).arg(chanNames));
    printMessage(tr("         %1 devices  : %2").arg(numDevs).arg(devNames));
    printMessage(tr("         Trig Type %1 : Terminal %2 : Source %3").arg(trigType).arg(trigTerm).arg(trigSrc));
  }

Error:
  return;
}