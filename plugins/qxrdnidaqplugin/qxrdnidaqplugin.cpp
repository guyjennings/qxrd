#include "qxrdnidaqplugin.h"
#include "NIDAQmx.h"
#include <QVector>
#if QT_VERSION >= 0x040700
#include <QElapsedTimer>
#endif
#include <stdio.h>
#include <QMutexLocker>
#include <QStringList>

#define DAQmxErrChk(functionCall) do { if( DAQmxFailed(error=(functionCall)) ) { QxrdNIDAQPlugin::errorCheck(__FILE__,__LINE__,error); goto Error; } } while(0)

QxrdNIDAQPlugin::QxrdNIDAQPlugin() :
  m_ErrorOutput(NULL),
  m_AOTaskHandle(0),
  m_AITaskHandle(0),
  m_TrigAOTask(0),
  m_PulseTask(0),
  m_CountersTask(0)
{
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

QString QxrdNIDAQPlugin::name() const
{
  return "NI DAQ Card";
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

void   QxrdNIDAQPlugin::setAnalogWaveform(int chan, double rate, double wfm[], int size)
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
                                          qPrintable(tr("Dev1/ao%1").arg(chan)), NULL, -10.0, 10.0, DAQmx_Val_Volts, NULL));

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

void QxrdNIDAQPlugin::pulseOutput()
{
  QMutexLocker lock(&m_Mutex);

  int error;

  DAQmxErrChk(DAQmxCreateTask("pulse-output", &m_PulseTask));
  DAQmxErrChk(DAQmxCreateCOPulseChanTime(m_PulseTask, "Dev1/ctr0", "", DAQmx_Val_Seconds, DAQmx_Val_Low, 0, 1e-6, 1e-6));

  DAQmxErrChk(DAQmxStartTask(m_PulseTask));
  DAQmxErrChk(DAQmxWaitUntilTaskDone(m_PulseTask, 0.5));
  DAQmxErrChk(DAQmxStopTask(m_PulseTask));
  DAQmxErrChk(DAQmxClearTask(m_PulseTask));

Error:
  return;
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

void   QxrdNIDAQPlugin::continuousAnalogInput(QStringList chans, double sampleRate, int bufferSize)
{
  int error;

  if (m_ContinuousInputTask) {
    DAQmxClearTask(m_ContinuousInputTask);
    m_ContinuousInputTask = NULL;
  }

  if (m_ContinuousInputTask == NULL) {
    DAQmxErrChk(DAQmxCreateTask("continuous", &m_ContinuousInputTask));

    m_NContinuousInputs = chans.count();
    m_NContinuousSamples = bufferSize;

    foreach(QString chan, chans) {
      DAQmxErrChk(DAQmxCreateAIVoltageChan(m_ContinuousInputTask, qPrintable(chan), "", DAQmx_Val_Cfg_Default, -10.0, 10.0, DAQmx_Val_Volts, NULL));
    }

    DAQmxErrChk(DAQmxCfgSampClkTiming(m_ContinuousInputTask, NULL, sampleRate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, bufferSize));
    DAQmxErrChk(DAQmxSetBufInputBufSize(m_ContinuousInputTask, bufferSize));
    DAQmxErrChk(DAQmxSetReadOverWrite(m_ContinuousInputTask, DAQmx_Val_OverwriteUnreadSamps));

    DAQmxErrChk(DAQmxStartTask(m_ContinuousInputTask));

    return;
  }

Error:
  DAQmxClearTask(m_ContinuousInputTask);

  m_ContinuousInputTask = 0;
}

void   QxrdNIDAQPlugin::haltContinuousAnalogInput()
{
  DAQmxClearTask(m_ContinuousInputTask);

  m_ContinuousInputTask = 0;
}

QVector<double> QxrdNIDAQPlugin::readContinuousAnalogInput(int nsamp)
{
  QVector<float64> buff;
  uInt32 available;
  uInt64 pos;
  int error;

  if (m_ContinuousInputTask) {
    int32 actuallyRead = 0;

    if (nsamp <= 0) {
      DAQmxErrChk(DAQmxGetReadCurrReadPos(m_ContinuousInputTask, &pos));

      //    printf("Current Read Position %lld\n", pos);

      DAQmxErrChk(DAQmxGetReadAvailSampPerChan(m_ContinuousInputTask, &available));

      if (available > 0) {
        buff.resize(available*m_NContinuousInputs);
        DAQmxErrChk(DAQmxReadAnalogF64(m_ContinuousInputTask, available, 0,
                                       DAQmx_Val_GroupByScanNumber, buff.data(), buff.count(),
                                       &actuallyRead, NULL));
      } else {
        buff.resize(m_NContinuousSamples*m_NContinuousInputs);
        DAQmxErrChk(DAQmxSetReadRelativeTo(m_ContinuousInputTask, DAQmx_Val_MostRecentSamp));
        DAQmxErrChk(DAQmxSetReadOffset(m_ContinuousInputTask, -m_NContinuousSamples));

        DAQmxErrChk(DAQmxReadAnalogF64(m_ContinuousInputTask, m_NContinuousSamples, 0,
                                       DAQmx_Val_GroupByScanNumber, buff.data(), buff.count(),
                                       &actuallyRead, NULL));

        DAQmxErrChk(DAQmxResetReadRelativeTo(m_ContinuousInputTask));
        DAQmxErrChk(DAQmxResetReadOffset(m_ContinuousInputTask));
      }
    } else {
      buff.resize(nsamp*m_NContinuousInputs);
      DAQmxErrChk(DAQmxSetReadRelativeTo(m_ContinuousInputTask, DAQmx_Val_MostRecentSamp));
      DAQmxErrChk(DAQmxSetReadOffset(m_ContinuousInputTask, -nsamp));

      DAQmxErrChk(DAQmxReadAnalogF64(m_ContinuousInputTask, nsamp, 0,
                                     DAQmx_Val_GroupByScanNumber, buff.data(), buff.count(),
                                     &actuallyRead, NULL));

      DAQmxErrChk(DAQmxResetReadRelativeTo(m_ContinuousInputTask));
      DAQmxErrChk(DAQmxResetReadOffset(m_ContinuousInputTask));
    }

    QVector<double> res(actuallyRead*m_NContinuousInputs);

    for (int i=0; i<actuallyRead*m_NContinuousInputs; i++) {
      res[i] = buff[i];
    }

    return res;
  }

Error:
  return QVector<double>();
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

void QxrdNIDAQPlugin::setAnalogOutput(QString channelName, double value)
{
  TaskHandle task = 0;
  int error;

  DAQmxErrChk(DAQmxCreateTask("qxrd-analog-out", &task));
  DAQmxErrChk(DAQmxCreateAOVoltageChan (m_AOTaskHandle,
                                        qPrintable(channelName),
                                        NULL,
                                        -10.0, 10.0,
                                        DAQmx_Val_Volts, NULL));

  DAQmxErrChk(DAQmxWriteAnalogScalarF64(task, true, 10.0, value, NULL));

Error:
  DAQmxClearTask(task);
}

Q_EXPORT_PLUGIN2(qxrdnidaqplugin, QxrdNIDAQPlugin)


