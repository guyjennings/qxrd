#include "qxrdnidaqplugin.h"
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

#define DAQmxErrChk(functionCall) do { int error; if( DAQmxFailed(error=(functionCall)) ) { QxrdNIDAQPlugin::errorCheck(__FILE__,__LINE__,error); goto Error; } } while(0)

QxrdNIDAQPlugin::QxrdNIDAQPlugin() :
  inherited("nidaqPlugin"),
  m_SyncTask(0),
  m_SyncAOTask(0),
  m_SyncAITask(0),
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

QxrdNIDAQPlugin::~QxrdNIDAQPlugin()
{
  closeTaskHandles();
}

void QxrdNIDAQPlugin::registerMetaTypes()
{
  qRegisterMetaType<QxrdNIDAQPlugin*>("QxrdNIDAQPlugin*");
}

void QxrdNIDAQPlugin::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  printMessage("QxrdNIDAQPlugin::initialize");

  m_DeviceNames = deviceNames();

  m_DeviceCount = m_DeviceNames.count();

  for (int i=0; i<m_DeviceCount; i++) {
    QString deviceName = m_DeviceNames.value(i);

    if (!deviceIsSimulated(deviceName)) {
      QStringList counterChans = deviceCOChannels(deviceName);
      QStringList outputChans  = deviceAOChannels(deviceName);
      QStringList inputChans   = deviceAIChannels(deviceName);

      m_DetectorDeviceNames.append(counterChans);
      m_OutputDeviceNames.append(outputChans);
      m_InputDeviceNames.append(inputChans);
    }
  }

  m_DetectorDeviceCount = m_DetectorDeviceNames.count();
  m_OutputDeviceCount   = m_OutputDeviceNames.count();
  m_InputDeviceCount    = m_InputDeviceNames.count();
}

QString QxrdNIDAQPlugin::name() const
{
  return "NI DAQ Plugin";
}

void QxrdNIDAQPlugin::errorCheck(const char* file, int line, int err)
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

void QxrdNIDAQPlugin::closeTaskHandles()
{
  clearSync();
  clearDetectorSync();
  clearOutputChannels();
  clearInputChannels();
}

//double QxrdNIDAQPlugin::aiGet(QString chan)
//{
//  initTaskHandles();

//  int error;
//  float64 res = 0;

//  DAQmxErrChk(DAQmxReadAnalogScalarF64(m_AITaskHandle, 10.0, &res, NULL))

//Error:
//  return res;
//  return 0;
//}

//double QxrdNIDAQPlugin::getAnalogInput(int chan)
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

//int QxrdNIDAQPlugin::prepareContinuousInput(double sampleRate,
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

QStringList QxrdNIDAQPlugin::deviceNames()
{
  char buffer[5120]="";

  DAQmxErrChk(DAQmxGetSysDevNames(buffer, sizeof(buffer)));
  //  printf("%d: DAQmxGetSysDevNames : \"%s\"\n", res, buffer);

Error:
  QStringList result = QString(buffer).split(", ");

  return result;
}

int QxrdNIDAQPlugin::deviceIsSimulated(QString device)
{
  bool32 res = true;

  DAQmxErrChk(DAQmxGetDevIsSimulated(qPrintable(device), &res));

Error:
  return res;
}

QStringList QxrdNIDAQPlugin::deviceAIChannels(QString device)
{
  char buffer[5120]="";

  DAQmxErrChk(DAQmxGetDevAIPhysicalChans(qPrintable(device), buffer, sizeof(buffer)));
  //  printf("%d: DAQmxGetDevAIPhysicalChans : \"%s\"\n", res, buffer);

Error:
  QStringList result = QString(buffer).split(", ");

  return result;
}

QStringList QxrdNIDAQPlugin::deviceAOChannels(QString device)
{
  char buffer[5120]="";

  DAQmxErrChk(DAQmxGetDevAOPhysicalChans(qPrintable(device), buffer, sizeof(buffer)));

Error:
  QStringList result = QString(buffer).split(", ");

  return result;
}

QStringList QxrdNIDAQPlugin::deviceCOChannels(QString device)
{
  char buffer[5120]="";

  DAQmxErrChk(DAQmxGetDevCOPhysicalChans(qPrintable(device), buffer, sizeof(buffer)));

Error:
  QStringList result = QString(buffer).split(", ");

  return result;
}

static int32 CVICALLBACK syncCallback(TaskHandle taskHandle, int32 status, void *callbackData)
{
  QxrdNIDAQPlugin *plugin = reinterpret_cast<QxrdNIDAQPlugin*>(callbackData);

  if (plugin) {
    return plugin->syncCallback(taskHandle, status);
  } else {
    return status;
  }
}

//static int32 CVICALLBACK aiCallback(TaskHandle taskHandle, int32 status, void *callbackData)
//{
//  QxrdNIDAQPlugin *plugin = reinterpret_cast<QxrdNIDAQPlugin*>(callbackData);

//  if (plugin) {
//    return plugin->aiCallback(taskHandle, status);
//  } else {
//    return status;
//  }
//}

//static int32 CVICALLBACK aoCallback(TaskHandle taskHandle, int32 status, void *callbackData)
//{
//  QxrdNIDAQPlugin *plugin = reinterpret_cast<QxrdNIDAQPlugin*>(callbackData);

//  if (plugin) {
//    return plugin->aoCallback(taskHandle, status);
//  } else {
//    return status;
//  }
//}

void QxrdNIDAQPlugin::clearSync()
{

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

void QxrdNIDAQPlugin::clearDetectorSync()
{
  if (m_SyncDetTasks.count() > 0) {
    if (qcepDebug(DEBUG_NIDAQ)) {
      printMessage(tr("Stopping %1 NIDAQ detector sync channel(s)").arg(m_SyncDetTasks.count()));
    }

    while (m_SyncDetTasks.count()) {
      DAQmxClearTask(m_SyncDetTasks.takeFirst());
    }

    m_DetectorChannels.clear();
  }

  return;
}

void QxrdNIDAQPlugin::clearOutputChannels()
{
  if (m_SyncAOTask) {
    if (qcepDebug(DEBUG_NIDAQ)) {
      printMessage(tr("Stopping NIDAQ output channel(s)"));
    }

    DAQmxClearTask(m_SyncAOTask);
    m_SyncAOTask = NULL;
    m_OutputChannels.clear();
    m_OutputNSamples = 0;
  }

  return;
}

void QxrdNIDAQPlugin::clearInputChannels()
{
  if (m_SyncAITask) {
    if (qcepDebug(DEBUG_NIDAQ)) {
      printMessage(tr("Stopping NIDAQ input channel(s)"));
    }

    DAQmxClearTask(m_SyncAITask);
    m_SyncAITask = NULL;
    m_InputChannels.clear();
    m_InputNSamples = 0;
  }

  return;
}

void QxrdNIDAQPlugin::changeExposureTime(double t, int n)
{
  m_ExposureTime = t;
  m_SyncNPhases  = n;
  m_SyncLongTime = m_ExposureTime*m_SyncNPhases;

  if (m_SyncTask == NULL) {
    DAQmxErrChk(DAQmxCreateTask("sync", &m_SyncTask));

    DAQmxErrChk(DAQmxCreateCOPulseChanTime(m_SyncTask,
                                           "Dev1/ctr0",
                                           "ctr0",
                                           DAQmx_Val_Seconds,
                                           DAQmx_Val_Low,
                                           0.0,
                                           m_SyncLongTime / 2.0 + 0.0001,
                                           m_SyncLongTime / 2.0
                                           ));

    DAQmxErrChk(DAQmxCfgImplicitTiming(m_SyncTask, DAQmx_Val_ContSamps, 100));
    DAQmxErrChk(DAQmxRegisterSignalEvent(m_SyncTask,
                                         DAQmx_Val_CounterOutputEvent,
                                         /*0*/ DAQmx_Val_SynchronousEventCallbacks,
                                         &::syncCallback,
                                         this));
  } else if (qcepDebug(DEBUG_NIDAQ)) {
    printMessage("QxrdNIDAQPlugin::changeExposureTime when sync task already running");
  }

Error:
  return;
}

void QxrdNIDAQPlugin::addDetectorSync(QxrdSynchronizedDetectorChannelWPtr p)
{
  QxrdSynchronizedDetectorChannelPtr sync(p);
  QxrdAcqCommonPtr acq(QxrdAcqCommon::findAcquisition(p));

  if (sync && acq) {
    int i = sync->get_DetectorNumber();

    QxrdDetectorSettingsPtr det = acq->detector(i);

    if (det) {
      int n = m_SyncDetTasks.count();

      QString taskName = tr("syncDet%1").arg(n);

      int relExpos = det->get_ExposureFactor();

      if (qcepDebug(DEBUG_NIDAQ)) {
        printMessage(tr("Adding NIDAQ detector sync channel %1").arg(m_SyncDetTasks.count()));
        printMessage(tr("TaskName %1, Channel Name %2, Detector Number %3, Exposure Time %4, Relative Exposure %5, Readout Delay %6")
                     .arg(taskName)
                     .arg(sync->channelName())
                     .arg(i)
                     .arg(m_ExposureTime)
                     .arg(relExpos)
                     .arg(det->get_ReadoutDelay()));
      }

      TaskHandle detTask;
      DAQmxErrChk(DAQmxCreateTask(qPrintable(taskName), &detTask));

      m_SyncDetTasks.append(detTask);
      m_DetectorChannels.append(p);

      double dp = m_ExposureTime / relExpos;

      DAQmxErrChk(DAQmxCreateCOPulseChanTime(detTask,
                                             qPrintable(sync->channelName()),
                                             qPrintable(tr("ctr%1").arg(sync->get_ChannelNumber())),
                                             DAQmx_Val_Seconds,
                                             DAQmx_Val_Low,
                                             0.0,
                                             det -> get_ReadoutDelay(),
                                             dp - det->get_ReadoutDelay()));

      if (m_SyncNPhases*relExpos > 1) {
        DAQmxErrChk(DAQmxCfgImplicitTiming  (detTask, DAQmx_Val_FiniteSamps, m_SyncNPhases*relExpos));
      }

      DAQmxErrChk(DAQmxCfgDigEdgeStartTrig(detTask, "/Dev1/Ctr0InternalOutput", DAQmx_Val_Rising));
      DAQmxErrChk(DAQmxSetStartTrigRetriggerable(detTask, true));
    }
  }

Error:
  return;
}

void QxrdNIDAQPlugin::addOutputChannel(QxrdSynchronizedOutputChannelWPtr p)
{
  QxrdSynchronizedOutputChannelPtr out(p);
  QxrdAcqCommonPtr                 acq(QxrdAcqCommon::findAcquisition(p));

  if (out && acq) {
    QxrdSynchronizedAcquisitionPtr sync(acq->synchronizedAcquisition());

    if (sync) {
      int n = m_OutputChannels.count();

      QString chanName = tr("aoTask%1").arg(n);

      if (n == 0) {
        m_OutputNSamples = out->get_NSamples();
      }

      if (qcepDebug(DEBUG_NIDAQ)) {
        printMessage(tr("Adding NIDAQ output channel %1").arg(n));
        printMessage(tr("Task name aoTask, Channel Name %1, Device Name %2, Sample Rate %3, nSamples %4")
                     .arg(chanName)
                     .arg(out->channelName())
                     .arg(sync->get_OutputSampleRate())
                     .arg(m_OutputNSamples));
      }

      if (m_SyncAOTask == NULL) {
        DAQmxErrChk(DAQmxCreateTask("aoTask", &m_SyncAOTask));
      }

      m_OutputChannels.append(p);

      DAQmxErrChk(DAQmxCreateAOVoltageChan(m_SyncAOTask,
                                           qPrintable(out->channelName()),
                                           qPrintable(chanName),
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
                                             "/Dev1/Ctr0InternalOutput",
                                             DAQmx_Val_Rising));

        DAQmxErrChk(DAQmxSetStartTrigRetriggerable(m_SyncAOTask, true));
      }
    }
  }

Error:
  return;
}

void QxrdNIDAQPlugin::addInputChannel(QxrdSynchronizedInputChannelWPtr p)
{
  QxrdSynchronizedInputChannelPtr inp(p);
  QxrdAcqCommonPtr acq(QxrdAcqCommon::findAcquisition(p));

  if (inp && acq) {
    QxrdSynchronizedAcquisitionPtr sync(acq->synchronizedAcquisition());

    if (sync) {
      int n = m_InputChannels.count();

      QString chanName = tr("ai%1").arg(n);

      if (n == 0) {
        m_InputNSamples = inp->get_NSamples();
      }

      if (qcepDebug(DEBUG_NIDAQ)) {
        printMessage(tr("Adding NIDAQ input channel %1").arg(n));
        printMessage(tr("Task name aoTask, Channel Name %1, Device Name %2, Sample Rate %3, NSamples %4")
                     .arg(chanName)
                     .arg(inp->channelName())
                     .arg(sync->get_InputSampleRate())
                     .arg(m_InputNSamples));
      }

      if (m_SyncAITask == NULL) {
        DAQmxErrChk(DAQmxCreateTask("aiTask", &m_SyncAITask));
      }

      m_InputChannels.append(p);

      DAQmxErrChk(DAQmxCreateAIVoltageChan(m_SyncAITask,
                                           qPrintable(inp->channelName()),
                                           qPrintable(chanName),
                                           DAQmx_Val_Cfg_Default,
                                           -10.0,
                                           10.0,
                                           DAQmx_Val_Volts,
                                           NULL));

      if (n == 0) {
        DAQmxErrChk(DAQmxCfgSampClkTiming(m_SyncAITask,
                                          NULL,
                                          sync->get_InputSampleRate(),
                                          DAQmx_Val_Rising,
                                          DAQmx_Val_FiniteSamps,
                                          m_InputNSamples));

        DAQmxErrChk(DAQmxCfgDigEdgeStartTrig(m_SyncAITask,
                                             "/Dev1/Ctr0InternalOutput",
                                             DAQmx_Val_Rising));

        DAQmxErrChk(DAQmxSetStartTrigRetriggerable(m_SyncAITask, true));
      }
    }
  }

Error:
  return;
}

void QxrdNIDAQPlugin::startSync()
{
  if (m_SyncTask) {
    if (qcepDebug(DEBUG_NIDAQ)) {
      printMessage(tr("Starting NIDAQ sync"));
    }

    DAQmxErrChk(DAQmxStartTask(m_SyncTask));
  }

Error:
  return;
}

void QxrdNIDAQPlugin::startDetectorSync()
{
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

void QxrdNIDAQPlugin::startOutputChannels()
{
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
                                    DAQmx_Val_GroupByScanNumber,
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

void QxrdNIDAQPlugin::startInputChannels()
{
  if (m_SyncAITask) {
    if (qcepDebug(DEBUG_NIDAQ)) {
      printMessage(tr("Starting NIDAQ input channels"));
    }

    DAQmxErrChk(DAQmxStartTask(m_SyncAITask));
  }

Error:
  return;
}

int32 QxrdNIDAQPlugin::syncCallback(TaskHandle task, int32 status)
{
  int32 state;

  DAQmxErrChk(DAQmxGetCOOutputState(task, "ctr0", &state));

  if (state == DAQmx_Val_Low) {
    state = 0;
  } else if (state == DAQmx_Val_High) {
    state = 1;
  }

  m_SyncCounter += 1;

//  if (qcepDebug(DEBUG_NIDAQ)) {
//    printMessage(tr("QxrdNIDAQPlugin::syncCallback %1 = %2, state = %3")
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

//int32 QxrdNIDAQPlugin::aiCallback(TaskHandle task, int32 status)
//{
//  printMessage(tr("aiCallback %1").arg(status));

//  return status;
//}

//int32 QxrdNIDAQPlugin::aoCallback(TaskHandle task, int32 status)
//{
////  printMessage(tr("aoCallback %1").arg(status));

//  return status;
//}

int QxrdNIDAQPlugin::detectorDeviceCount()
{
  return m_DetectorDeviceCount;
}

QString QxrdNIDAQPlugin::detectorDeviceName(int n)
{
  return m_DetectorDeviceNames.value(n);
}

int QxrdNIDAQPlugin::outputDeviceCount()
{
  return m_OutputDeviceCount;
}

QString QxrdNIDAQPlugin::outputDeviceName(int n)
{
  return m_OutputDeviceNames.value(n);
}

int QxrdNIDAQPlugin::inputDeviceCount()
{
  return m_InputDeviceCount;
}

QString QxrdNIDAQPlugin::inputDeviceName(int n)
{
  return m_InputDeviceNames.value(n);
}

void QxrdNIDAQPlugin::updateSyncWaveforms(QxrdSynchronizedAcquisitionWPtr s, QxrdAcquisitionParameterPackWPtr p)
{
  QxrdAcquisitionParameterPackPtr parm(p);
  QxrdSynchronizedAcquisitionPtr  sync(s);

  if (qcepDebug(DEBUG_NIDAQ)) {
    printMessage("QxrdNIDAQPlugin::updateSyncWaveforms");
  }

  if (sync && parm) {
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
}

void QxrdNIDAQPlugin::prepareForAcquisition(QxrdSynchronizedAcquisitionWPtr s, QxrdAcquisitionParameterPackWPtr p)
{
  printMessage("QxrdNIDAQPlugin::prepareForAcquisition");
}

void QxrdNIDAQPlugin::prepareForDarkAcquistion(QxrdSynchronizedAcquisitionWPtr s, QxrdDarkAcquisitionParameterPackWPtr p)
{
  printMessage("QxrdNIDAQPlugin::prepareForDarkAcquisition");
}
