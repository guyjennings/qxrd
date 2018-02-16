#include "qxrdnidaqplugin.h"
#include "NIDAQmx.h"
#include <QVector>
#include <QElapsedTimer>
#include <stdio.h>
#include <QMutexLocker>
#include <QStringList>

#define DAQmxErrChk(functionCall) do { if( DAQmxFailed(error=(functionCall)) ) { QxrdNIDAQPlugin::errorCheck(__FILE__,__LINE__,error); goto Error; } } while(0)

QxrdNIDAQPlugin::QxrdNIDAQPlugin() :
  inherited("nidaqPlugin"),
  m_ErrorOutput(NULL),
  m_AOTaskHandle(0),
  m_AITaskHandle(0),
  m_TrigAOTask(0),
  m_PulseTask(0),
  m_CountersTask(0)
{
  setObjectName("nidaq");

  //  printf("NI-DAQ plugin constructed\n");
  //  initTaskHandles();

  //  res = DAQmxGetDevTerminals("Dev1", buffer, sizeof(buffer));
  //  printf("%d: DAQmxGetDevTerminals : \"%s\"\n", res, buffer);

  //  int32 aiTrigUsage;
  //  int32 aoTrigUsage;
  //  int32 diTrigUsage;
  //  int32 doTrigUsage;
  //  int32 ciTrigUsage;
  //  int32 coTrigUsage;

  //  res = DAQmxGetDevAITrigUsage("Dev1", &aiTrigUsage);
  //  res = DAQmxGetDevAOTrigUsage("Dev1", &aoTrigUsage);
  //  res = DAQmxGetDevDITrigUsage("Dev1", &diTrigUsage);
  //  res = DAQmxGetDevDOTrigUsage("Dev1", &doTrigUsage);
  //  res = DAQmxGetDevCITrigUsage("Dev1", &ciTrigUsage);
  //  res = DAQmxGetDevCOTrigUsage("Dev1", &coTrigUsage);

  //  printf("AI:%02x, AO:%02x, DI:%02x, DO:%02x, CI:%02x, CO:%02x\n",
  //         aiTrigUsage, aoTrigUsage, diTrigUsage, doTrigUsage, ciTrigUsage, coTrigUsage
  //         );

  //  bool32 isSupported=false;
  //  res = DAQmxGetDevCISampClkSupported("Dev1", &isSupported);

  //  printf("%d: DAQmxGetDevCISampClkSupported=%d\n", res, isSupported);
}

QxrdNIDAQPlugin::~QxrdNIDAQPlugin()
{
  //  printf("NI-DAQ plugin destroyed");
  closeTaskHandles();
}

void QxrdNIDAQPlugin::setErrorOutput(QObject *errors)
{
  m_ErrorOutput = errors;
}

void QxrdNIDAQPlugin::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  printMessage("QxrdNIDAQPlugin::initialize");
}

QString QxrdNIDAQPlugin::name() const
{
  return "NI DAQ Card";
}

void QxrdNIDAQPlugin::printMessage(QString msg)
{
  if (m_ErrorOutput) {
    QMetaObject::invokeMethod(m_ErrorOutput, "printMessage", Qt::QueuedConnection, Q_ARG(QString, msg));
  }
}

void QxrdNIDAQPlugin::errorCheck(const char* file, int line, int err)
{
  if (DAQmxFailed(err)) {
    int sz = DAQmxGetErrorString(err, NULL, 0);

    if (sz > 0) {
      char *buff = (char*) malloc(sz);

      if (DAQmxGetErrorString(err, buff, sz) == 0) {
        if (m_ErrorOutput &&
            QMetaObject::invokeMethod(m_ErrorOutput, "printMessage", Qt::QueuedConnection,
                                      Q_ARG(QString, tr("%1:%2 NI-DAQ Error %3 : %4").arg(file).arg(line).arg(err).arg(buff)))) {
        } else {
          printf("%s:%d NI-DAQ Error %d : %s\n", file, line, err, buff);
        }
      }

      free(buff);
    }
  }
}

void QxrdNIDAQPlugin::initTaskHandles()
{
  //  int error;

  //  if (m_AOTaskHandle == 0) {
  //    DAQmxErrChk(DAQmxCreateTask("", &m_AOTaskHandle));
  //    DAQmxErrChk(DAQmxCreateAOVoltageChan (m_AOTaskHandle, "Dev1/ao0", NULL, -10.0, 10.0, DAQmx_Val_Volts, NULL));
  //    DAQmxErrChk(DAQmxCreateAOVoltageChan (m_AOTaskHandle, "Dev1/ao1", NULL, -10.0, 10.0, DAQmx_Val_Volts, NULL));
  //  }

  //  if (m_AITaskHandle == 0) {
  //    DAQmxErrChk(DAQmxCreateTask("", &m_AITaskHandle));
  //    DAQmxErrChk(DAQmxCreateAIVoltageChan (m_AITaskHandle, "Dev1/ai0", NULL, DAQmx_Val_Cfg_Default, -10.0, 10.0, DAQmx_Val_Volts, NULL));
  //    DAQmxErrChk(DAQmxCreateAIVoltageChan (m_AITaskHandle, "Dev1/ai1", NULL, DAQmx_Val_Cfg_Default, -10.0, 10.0, DAQmx_Val_Volts, NULL));
  //  }

  //  if (m_TrigAOTask == 0) {
  //    DAQmxErrChk(DAQmxCreateTask("", &m_TrigAOTask));
  //    DAQmxErrChk(DAQmxCreateAOVoltageChan (m_TrigAOTask, "Dev1/ao0", NULL, -10.0, 10.0, DAQmx_Val_Volts, NULL));
  ////    DAQmxErrChk(DAQmxCfgAnlgEdgeStartTrig(m_TrigAOTask, "Dev1/ai2", DAQmx_Val_FallingSlope, 1.0));
  //  }

  //  return;

  //Error:
  //  printf("Error in initTaskHandles\n");

  //  closeTaskHandles();
}

void QxrdNIDAQPlugin::closeTaskHandles()
{
  QMutexLocker lock(&m_Mutex);

  if (m_AOTaskHandle) {
    DAQmxClearTask(m_AOTaskHandle);
    m_AOTaskHandle = 0;
  }

  if (m_AITaskHandle) {
    DAQmxClearTask(m_AITaskHandle);
    m_AITaskHandle = 0;
  }

  if (m_TrigAOTask) {
    DAQmxClearTask(m_TrigAOTask);
    m_TrigAOTask = 0;
  }
}

//void   QxrdNIDAQPlugin::aoSet(double val1, double val2)
//{
//#if QT_VERSION >= 0x040700
//  QElapsedTimer t;
//#else
//  QTime t;
//#endif

//  t.start();

//  double vals[2];
//  int32 nWritten;

//  vals[0]=val1;
//  vals[1]=val2;

//  int error;
//  DAQmxErrChk(DAQmxWriteAnalogF64(m_AOTaskHandle, 1, true, 10.0,
//                                  DAQmx_Val_GroupByScanNumber,
//                                  vals, &nWritten,
//                                  NULL));

////  printf("Analog output took %d msec\n", t.elapsed());

//Error:
//  return;
//}
//void   QxrdNIDAQPlugin::aoSet(QString chan, double val)
//{
//  initTaskHandles();

//  int error;
////  DAQmxErrChk(DAQmxCreateAOVoltageChan (m_TaskHandle, qPrintable(chan), NULL, -10.0, 10.0, DAQmx_Val_Volts, NULL));
//  DAQmxErrChk(DAQmxWriteAnalogScalarF64(m_AOTaskHandle, true, 10.0, val, NULL));

//Error:
//  return;
//}

//double QxrdNIDAQPlugin::aiGet(int chan)
//{
//  return 0;
//}

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

//void   QxrdNIDAQPlugin::aoWave(QString chan, int type, double freq, double amplitude, double offset)
//{
//  int error;
//  float64 res = 0;
//  QVector<float64> waveform(1024);

//  for (int i=0; i<1024; i++) {
//    waveform[i] = (i>512 ? 1 : -1);
//  }

//  int32 numWritten;

//  DAQmxErrChk(DAQmxWriteAnalogF64(m_AOTaskHandle, 1024, true, 10.0, DAQmx_Val_GroupByChannel, waveform.data(), &numWritten, NULL))

//Error:
//  return;
//}

//void   QxrdNIDAQPlugin::setAnalogChannel(int chan)
//{
//  QMutexLocker lock(&m_Mutex);
////  printf("setAnalogChannel(%d)\n", chan);

//  int error;


//Error:
//  return;
//}

double QxrdNIDAQPlugin::getAnalogInput(int chan)
{
  QMutexLocker lock(&m_Mutex);

  int error;
  float64 res = 0;

  if (m_AITaskHandle) {
    DAQmxStopTask(m_AITaskHandle);
    DAQmxClearTask(m_AITaskHandle);
    m_AITaskHandle = 0;
  }

  if (chan >= 0) {
    DAQmxErrChk(DAQmxCreateTask("qxrd-input", &m_AITaskHandle));
    DAQmxErrChk(DAQmxCreateAIVoltageChan (m_AITaskHandle,
                                          qPrintable(tr("Dev1/ai%1").arg(chan)), NULL, DAQmx_Val_Cfg_Default, -10.0, 10.0, DAQmx_Val_Volts, NULL));

    if (m_AITaskHandle) {
      DAQmxErrChk(DAQmxReadAnalogScalarF64(m_AITaskHandle, 10.0, &res, NULL));
    }
  }

Error:
  return res;
}

void   QxrdNIDAQPlugin::setAnalogOutput(int chan, double value)
{
  QMutexLocker lock(&m_Mutex);

  int error;

  if (m_AOTaskHandle) {
    DAQmxStopTask(m_AOTaskHandle);
    DAQmxClearTask(m_AOTaskHandle);
    m_AOTaskHandle = 0;
  }

  if (chan >= 0) {
    DAQmxErrChk(DAQmxCreateTask("qxrd-output", &m_AOTaskHandle));
    DAQmxErrChk(DAQmxCreateAOVoltageChan (m_AOTaskHandle,
                                          qPrintable(tr("Dev1/ao%1").arg(chan)), NULL, -10.0, 10.0, DAQmx_Val_Volts, NULL));

    if (m_AOTaskHandle) {
      DAQmxErrChk(DAQmxWriteAnalogScalarF64(m_AOTaskHandle, true, 10.0, value, NULL));
    }
  }

Error:
  return;
}

void QxrdNIDAQPlugin::setAnalogOutput(QString channelName, double value)
{
  QMutexLocker lock(&m_Mutex);

  int error;

  if (m_AOTaskHandle) {
    DAQmxStopTask(m_AOTaskHandle);
    DAQmxClearTask(m_AOTaskHandle);
    m_AOTaskHandle = 0;
  }

  DAQmxErrChk(DAQmxCreateTask("qxrd-analog-out", &m_AOTaskHandle));
  DAQmxErrChk(DAQmxCreateAOVoltageChan (m_AOTaskHandle,
                                        qPrintable(channelName),
                                        NULL,
                                        -10.0, 10.0,
                                        DAQmx_Val_Volts, NULL));

  DAQmxErrChk(DAQmxWriteAnalogScalarF64(m_AOTaskHandle, true, 10.0, value, NULL));

Error:
  return;
}

void   QxrdNIDAQPlugin::setAnalogWaveform(QString chan, double rate, double wfm[], int size)
{
  QMutexLocker lock(&m_Mutex);
//  printf("setAnalogWaveform(%g,%g...,%d)\n", rate, wfm[0], size);

  int error;
  int32 nsampwrt;

  if (m_AOTaskHandle) {
    DAQmxStopTask(m_AOTaskHandle);
    DAQmxClearTask(m_AOTaskHandle);
    m_AOTaskHandle = 0;
  }

  if (chan >= 0) {
    DAQmxErrChk(DAQmxCreateTask("qxrd-output", &m_AOTaskHandle));
    DAQmxErrChk(DAQmxCreateAOVoltageChan (m_AOTaskHandle,
                                          qPrintable(chan), NULL, -10.0, 10.0, DAQmx_Val_Volts, NULL));

    if (m_AOTaskHandle) {
      DAQmxErrChk(
            DAQmxCfgSampClkTiming(m_AOTaskHandle, NULL, rate, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, size)
            );

      DAQmxErrChk(
            DAQmxWriteAnalogF64(m_AOTaskHandle, size, false, -1, DAQmx_Val_GroupByChannel, wfm, &nsampwrt, NULL)
            );
    }
  }

//  printf("%d samples written\n", nsampwrt);

Error:
  return;
}

void QxrdNIDAQPlugin::triggerAnalogWaveform()
{
  QMutexLocker lock(&m_Mutex);
//  printf("triggerAnalogWaveform()\n");

  int error;

  if (m_AOTaskHandle) {
    //    DAQmxErrChk(
    //      DAQmxWaitUntilTaskDone(m_AOTaskHandle, -1)
    //    )

    DAQmxErrChk(
          DAQmxStopTask(m_AOTaskHandle)
          );

    DAQmxErrChk(
          DAQmxStartTask(m_AOTaskHandle)
          );
  }

Error:
  return;
}

void QxrdNIDAQPlugin::setAnalogOutput(double value)
{
  int error;

  if (m_AOTaskHandle) {
    DAQmxErrChk(DAQmxWriteAnalogScalarF64(m_AOTaskHandle, true, 10.0, value, NULL));
  }

Error:
  return;
}

void QxrdNIDAQPlugin::pulseOutput()
{
//  QMutexLocker lock(&m_Mutex);

//  int error;

//  DAQmxErrChk(DAQmxCreateTask("", &m_PulseTask));
//  DAQmxErrChk(DAQmxCreateCOPulseChanTime(m_PulseTask, "Dev1/ctr0", "", DAQmx_Val_Seconds, DAQmx_Val_Low, 0, 1e-6, 1e-6));

//  DAQmxErrChk(DAQmxStartTask(m_PulseTask));
//  DAQmxErrChk(DAQmxWaitUntilTaskDone(m_PulseTask, 0.5));
//  DAQmxErrChk(DAQmxStopTask(m_PulseTask));

//Error:

//  if (m_PulseTask) {
//    DAQmxClearTask(m_PulseTask);
//  }

//  m_PulseTask = 0;
//  return;
}

double QxrdNIDAQPlugin::count(int /* chan */, double /* time */)
{
  QMutexLocker lock(&m_Mutex);

  static TaskHandle counterTask = 0;
  int error;
  float64 res = 0;

  if (counterTask == 0) {
    DAQmxErrChk(DAQmxCreateTask("counter", &counterTask));
    DAQmxErrChk(DAQmxCreateCICountEdgesChan(counterTask,"Dev1/ctr2", "", DAQmx_Val_Rising, 0, DAQmx_Val_CountUp));
    DAQmxErrChk(DAQmxSetCICountEdgesTerm(counterTask, "/Dev1/ctr2", "/Dev1/100MHzTimebase"));
    DAQmxErrChk(DAQmxStartTask(counterTask));
  }

  if (counterTask) {
    DAQmxErrChk(DAQmxReadCounterScalarF64(counterTask, 0, &res, NULL));
  }

  return res;

Error:
  DAQmxClearTask(counterTask);
  return res;
}

int QxrdNIDAQPlugin::configCounters(QStringList chans)
{
  int error;

  if (m_CountersTask) {
    DAQmxClearTask(m_CountersTask);

    m_CountersTask = 0;
  }

  if (m_CountersTask == 0) {
    DAQmxErrChk(DAQmxCreateTask("counters", &m_CountersTask));

    m_NCounters = chans.count();
    m_Counts.resize(m_NCounters);

    foreach(QString chan, chans) {
      QStringList parsed = chan.split(",");
      QString ch = parsed.value(0);
      QString sig = parsed.value(1);

      DAQmxErrChk(DAQmxCreateCICountEdgesChan(m_CountersTask, qPrintable(parsed.value(0)), "", DAQmx_Val_Rising, 0, DAQmx_Val_CountUp));
      DAQmxErrChk(DAQmxSetCICountEdgesTerm(m_CountersTask, qPrintable(ch), qPrintable(sig)));
    }

    DAQmxErrChk(DAQmxStartTask(m_CountersTask));

    return m_NCounters;
  }

Error:
  DAQmxClearTask(m_CountersTask);

  m_CountersTask = 0;

  return error;
}

QVector<double> QxrdNIDAQPlugin::readCounters()
{
  int error;

  if (m_CountersTask) {
    QVector<float64> counts(m_NCounters);

    DAQmxErrChk(DAQmxReadCounterScalarF64(m_CountersTask, 0, counts.data(), NULL));

    QVector<double> res(m_NCounters);

    for (int i=0; i<m_NCounters; i++) {
      res[i] = counts[i] - m_Counts[i];
      m_Counts[i] = counts[i];
    }

    return res;
  }

Error:
  return QVector<double>();
}

int QxrdNIDAQPlugin::prepareContinuousInput(double sampleRate,
                                            double acquireDelay,
                                            double exposureTime,
                                            QStringList chans,
                                            QVector<double> minVals,
                                            QVector<double> maxVals)
{
  int error = 0;
  int bufferSize = 0;

  m_SampleRate = sampleRate;
  m_AcquireDelay = acquireDelay;
  m_ExposureTime = exposureTime;

  finishContinuousInput();

  QRegExp re_ai("^(.*)/ai\\d+$");
  QRegExp re_ci("^(.*)/ctr\\d+$");
  QString aiDevice;

  if (m_ContinuousAITask == NULL) {
    DAQmxErrChk(DAQmxCreateTask("continuousAI", &m_ContinuousAITask));
  }

  for(int i=0; i<chans.count(); i++) {
    QString chan = chans.value(i);

    if (re_ai.exactMatch(chan)) {

      DAQmxErrChk(DAQmxCreateAIVoltageChan(m_ContinuousAITask, qPrintable(chan), "",
                                           DAQmx_Val_Cfg_Default,
                                           minVals.value(i),
                                           maxVals.value(i),
                                           DAQmx_Val_Volts, NULL ));

      m_ContinuousFlags.append(1);
      m_ContinuousChans.append(m_NAIChannels++);

      if (re_ai.capturedTexts().count() >= 2) {
        if (aiDevice.count() == 0) {
          aiDevice = re_ai.capturedTexts().value(1);
        }
      }

      printMessage(tr("Analog input channel : %1").arg(chan));
    } else if (re_ci.exactMatch(chan)) {
      TaskHandle thdl;

      DAQmxErrChk(DAQmxCreateTask(qPrintable(tr("continuousCI_%1").arg(m_ContinuousCITasks.count())), &thdl));

      m_ContinuousCITasks.append(thdl);

      DAQmxErrChk(DAQmxCreateCICountEdgesChan(thdl, qPrintable(chan), "", DAQmx_Val_Rising, 0, DAQmx_Val_CountUp));

      m_ContinuousFlags.append(-1);
      m_ContinuousChans.append(m_NCIChannels++);

      if (re_ci.capturedTexts().count() >= 2) {
        if (aiDevice.count() == 0) {
          aiDevice = re_ci.capturedTexts().value(1);
        }
      }

      printMessage(tr("Counter input channel : %1").arg(chan));
    } else {
      m_ContinuousFlags.append(0);
      m_ContinuousChans.append(0);

      printMessage(tr("Skipped channel : %1").arg(chan));
    }
  }

  if (m_NAIChannels == 0) {
    DAQmxErrChk(DAQmxCreateAIVoltageChan(m_ContinuousAITask,
                                         qPrintable(tr("%1/ai0").arg(aiDevice)), "",
                                         DAQmx_Val_Cfg_Default, -10.0, 10.0, DAQmx_Val_Volts, NULL ));

  }

  m_NContinuousSamples = m_SampleRate*(m_ExposureTime + m_AcquireDelay + 0.1);

  bufferSize = m_NContinuousSamples+(10.0*m_SampleRate);

  printMessage(tr("Buffer size %1").arg(bufferSize));

  if (m_ContinuousAITask) {
    DAQmxErrChk(DAQmxCfgSampClkTiming(m_ContinuousAITask, NULL,
                                      sampleRate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, bufferSize));
    DAQmxErrChk(DAQmxSetBufInputBufSize(m_ContinuousAITask, bufferSize));
    DAQmxErrChk(DAQmxSetReadOverWrite(m_ContinuousAITask, DAQmx_Val_OverwriteUnreadSamps));
  }

  foreach (TaskHandle tsk, m_ContinuousCITasks) {
    DAQmxErrChk(DAQmxCfgSampClkTiming(tsk, qPrintable(tr("/%1/ai/SampleClock").arg(aiDevice)),
                                      sampleRate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, bufferSize));
    DAQmxErrChk(DAQmxSetBufInputBufSize(tsk, bufferSize));
    DAQmxErrChk(DAQmxSetReadOverWrite(tsk, DAQmx_Val_OverwriteUnreadSamps));
    //    DAQmxErrChk(DAQmxCfgDigEdgeStartTrig(tsk, qPrintable(tr("/%1/ai/StartTrigger").arg(aiDevice)),
    //                                         DAQmx_Val_Rising));
  }

  foreach (TaskHandle tsk, m_ContinuousCITasks) {
    DAQmxErrChk(DAQmxStartTask(tsk));
  }

  if (m_ContinuousAITask) {
    DAQmxErrChk(DAQmxStartTask(m_ContinuousAITask));
  }

  m_NContinuousInputs = chans.count();

  m_ContinuousInputData.resize(m_NContinuousInputs);

  for (int i=0; i<m_NContinuousInputs; i++) {
    m_ContinuousInputData[i].resize(m_NContinuousSamples);
  }
Error:

  return error;
}

int QxrdNIDAQPlugin::countContinuousInput()
{
  return m_NContinuousSamples;
}

int QxrdNIDAQPlugin::readContinuousInput()
{
  int error = 0;

  uInt32 avail;

  QVector<double> aiBuff(m_NAIChannels*m_NContinuousSamples);
  QVector< QVector<double> > ciBuff(m_NCIChannels);

  for (int i=0; i<m_NCIChannels; i++) {
    ciBuff[i].resize(m_NContinuousSamples + 1);
  }

  if (m_ContinuousAITask) {
    DAQmxErrChk(DAQmxGetReadAvailSampPerChan(m_ContinuousAITask, &avail));

//    printMessage(tr("%1 samples available").arg(avail));

    if (avail < m_NContinuousSamples) {
      return -1;
    }

    DAQmxErrChk(DAQmxSetReadRelativeTo(m_ContinuousAITask, DAQmx_Val_MostRecentSamp));
    DAQmxErrChk(DAQmxSetReadOffset(m_ContinuousAITask, -m_NContinuousSamples));
  }

//  foreach(TaskHandle tsk, m_ContinuousCITasks) {
//    DAQmxErrChk(DAQmxSetReadRelativeTo(tsk, DAQmx_Val_MostRecentSamp));
//    DAQmxErrChk(DAQmxSetReadOffset(tsk, -m_NContinuousSamples));
//  }

  int32 actuallyRead;
  uInt64 lastSample[30];

  if (m_ContinuousAITask && m_NAIChannels) {
    DAQmxErrChk(DAQmxReadAnalogF64(m_ContinuousAITask, m_NContinuousSamples, -1,
                                   DAQmx_Val_GroupByChannel, aiBuff.data(), aiBuff.count(),
                                   &actuallyRead, NULL));

    DAQmxErrChk(DAQmxGetReadCurrReadPos(m_ContinuousAITask, &lastSample[0]));
  }

  for(int i=0; i<m_NCIChannels; i++) {
    TaskHandle tsk = m_ContinuousCITasks.value(i);

    if (tsk) {
      uInt64 currentPos;

      DAQmxErrChk(DAQmxGetReadCurrReadPos(tsk, &currentPos));
      DAQmxErrChk(DAQmxSetReadRelativeTo(tsk, DAQmx_Val_CurrReadPos));
      int32 offset = ((int64)lastSample[0]) - ((int64)currentPos) - (m_NContinuousSamples+1);
      DAQmxErrChk(DAQmxSetReadOffset(tsk, offset));
      DAQmxErrChk(DAQmxReadCounterF64(tsk, m_NContinuousSamples+1, -1,
                                      ciBuff[i].data(), ciBuff[i].count(),
                                      &actuallyRead, NULL));

      if (i<29) {
        DAQmxErrChk(DAQmxGetReadCurrReadPos(tsk, &lastSample[i+1]));

        if (lastSample[i+1] != lastSample[i]) {
          printf("Sync error %lld:%lld\n", lastSample[i+1], lastSample[i]);
        }
      }
    }
  }

  for (int i=0; i<m_NCIChannels; i++) {
    for (int j=0; j<m_NContinuousSamples; j++) {
      ciBuff[i][j] = ciBuff[i][j+1] - ciBuff[i][j];
    }

    ciBuff[i].resize(m_NContinuousSamples);
  }

  for (int i=0; i<m_NContinuousInputs; i++) {
    if (m_ContinuousFlags.value(i) == 1) { // an Analog Input Channel
      int chan = m_ContinuousChans.value(i);

      m_ContinuousInputData[i] = aiBuff.mid(chan*m_NContinuousSamples, m_NContinuousSamples);
    } else if (m_ContinuousFlags.value(i) == -1) { // a counter Input Channel
      int chan = m_ContinuousChans.value(i);

      m_ContinuousInputData[i] = ciBuff[chan].mid(0, m_NContinuousSamples);
    }
  }
Error:
  return error;
}

QVector<double> QxrdNIDAQPlugin::readContinuousInputChannel(int ch)
{
  return m_ContinuousInputData.value(ch);
}

void QxrdNIDAQPlugin::finishContinuousInput()
{
  if (m_ContinuousAITask) {
    DAQmxClearTask(m_ContinuousAITask);
  }

  foreach (TaskHandle tsk, m_ContinuousCITasks) {
    DAQmxClearTask(tsk);
  }

  m_ContinuousCITasks.clear();
  m_ContinuousAITask = 0;
  m_ContinuousFlags.clear();
  m_ContinuousChans.clear();

  m_NAIChannels = 0;
  m_NCIChannels = 0;
}

QStringList QxrdNIDAQPlugin::deviceNames()
{
  char buffer[5120]="";
  int error;

  DAQmxErrChk(DAQmxGetSysDevNames(buffer, sizeof(buffer)));
  //  printf("%d: DAQmxGetSysDevNames : \"%s\"\n", res, buffer);

Error:
  QStringList result = QString(buffer).split(", ");

  return result;
}

QString QxrdNIDAQPlugin::deviceType(QString device)
{
  char buffer[5120]="";
  int error;

  DAQmxErrChk(DAQmxGetDevProductType(qPrintable(device), buffer, sizeof(buffer)));

Error:
  QString result = QString(buffer);

  return result;
}

int QxrdNIDAQPlugin::deviceIsSimulated(QString device)
{
  bool32 res = true;

  int error;

  DAQmxErrChk(DAQmxGetDevIsSimulated(qPrintable(device), &res));

Error:
  return res;
}

QStringList QxrdNIDAQPlugin::deviceAIChannels(QString device)
{
  char buffer[5120]="";
  int error;

  DAQmxErrChk(DAQmxGetDevAIPhysicalChans(qPrintable(device), buffer, sizeof(buffer)));
  //  printf("%d: DAQmxGetDevAIPhysicalChans : \"%s\"\n", res, buffer);

Error:
  QStringList result = QString(buffer).split(", ");

  return result;
}

QStringList QxrdNIDAQPlugin::deviceAOChannels(QString device)
{
  char buffer[5120]="";
  int error;

  DAQmxErrChk(DAQmxGetDevAOPhysicalChans(qPrintable(device), buffer, sizeof(buffer)));

Error:
  QStringList result = QString(buffer).split(", ");

  return result;
}

QStringList QxrdNIDAQPlugin::deviceDIPorts(QString device)
{
  char buffer[5120]="";
  int error;

  DAQmxErrChk(DAQmxGetDevDIPorts(qPrintable(device), buffer, sizeof(buffer)));

Error:
  QStringList result = QString(buffer).split(", ");

  return result;
}

QStringList QxrdNIDAQPlugin::deviceDILines(QString port)
{
  char buffer[5120]="";
  int error;

  DAQmxErrChk(DAQmxGetDevDILines(qPrintable(port), buffer, sizeof(buffer)));

Error:
  QStringList result = QString(buffer).split(", ");

  return result;
}

QStringList QxrdNIDAQPlugin::deviceDOPorts(QString device)
{
  char buffer[5120]="";
  int error;

  DAQmxErrChk(DAQmxGetDevDOPorts(qPrintable(device), buffer, sizeof(buffer)));

Error:
  QStringList result = QString(buffer).split(", ");

  return result;
}

QStringList QxrdNIDAQPlugin::deviceDOLines(QString port)
{
  char buffer[5120]="";
  int error;

  DAQmxErrChk(DAQmxGetDevDOLines(qPrintable(port), buffer, sizeof(buffer)));

Error:
  QStringList result = QString(buffer).split(", ");

  return result;
}

QStringList QxrdNIDAQPlugin::deviceCIChannels(QString device)
{
  char buffer[5120]="";
  int error;

  DAQmxErrChk(DAQmxGetDevCIPhysicalChans(qPrintable(device), buffer, sizeof(buffer)));

Error:
  QStringList result = QString(buffer).split(", ");

  return result;
}

QStringList QxrdNIDAQPlugin::deviceCOChannels(QString device)
{
  char buffer[5120]="";
  int error;

  DAQmxErrChk(DAQmxGetDevCOPhysicalChans(qPrintable(device), buffer, sizeof(buffer)));

Error:
  QStringList result = QString(buffer).split(", ");

  return result;
}

double QxrdNIDAQPlugin::getAnalogInput(QString channelName)
{
  TaskHandle task = 0;
  float64 res = 0;
  int error;

  DAQmxErrChk(DAQmxCreateTask("qxrd-analog-in", &task));
  DAQmxErrChk(DAQmxCreateAIVoltageChan (task,
                                        qPrintable(channelName),
                                        NULL,
                                        DAQmx_Val_Cfg_Default, -10.0, 10.0,
                                        DAQmx_Val_Volts, NULL));

  DAQmxErrChk(DAQmxReadAnalogScalarF64(task, 10.0, &res, NULL));

Error:
  DAQmxClearTask(task);

  return res;
}
