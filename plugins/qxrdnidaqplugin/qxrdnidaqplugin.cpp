#include "qxrdnidaqplugin.h"
#include "NIDAQmx.h"
#include <QVector>
#if QT_VERSION >= 0x040700
#include <QElapsedTimer>
#endif
#include <stdio.h>
#include <QMutexLocker>

#define DAQmxErrChk(functionCall) { if( DAQmxFailed(error=(functionCall)) ) { QxrdNIDAQPlugin::errorCheck(__FILE__,__LINE__,error); goto Error; } }

QxrdNIDAQPlugin::QxrdNIDAQPlugin() :
  m_AOTaskHandle(0),
  m_AITaskHandle(0),
  m_TrigAOTask(0)
{
//  printf("NI-DAQ plugin constructed\n");
//  initTaskHandles();
}

QxrdNIDAQPlugin::~QxrdNIDAQPlugin()
{
//  printf("NI-DAQ plugin destroyed");
  closeTaskHandles();
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
        emit printMessage(QDateTime::currentDateTime(),
                          tr("%1:%2 NI-DAQ Error %3 : %4").arg(file).arg(line).arg(err).arg(buff));
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

void   QxrdNIDAQPlugin::aoSet(double val1, double val2)
{
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
}
void   QxrdNIDAQPlugin::aoSet(QString chan, double val)
{
//  initTaskHandles();

//  int error;
////  DAQmxErrChk(DAQmxCreateAOVoltageChan (m_TaskHandle, qPrintable(chan), NULL, -10.0, 10.0, DAQmx_Val_Volts, NULL));
//  DAQmxErrChk(DAQmxWriteAnalogScalarF64(m_AOTaskHandle, true, 10.0, val, NULL));

//Error:
//  return;
}

double QxrdNIDAQPlugin::aiGet(int chan)
{
  return 0;
}

double QxrdNIDAQPlugin::aiGet(QString chan)
{
//  initTaskHandles();

//  int error;
//  float64 res = 0;

//  DAQmxErrChk(DAQmxReadAnalogScalarF64(m_AITaskHandle, 10.0, &res, NULL))

//Error:
//  return res;
  return 0;
}

void   QxrdNIDAQPlugin::aoWave(QString chan, int type, double freq, double amplitude, double offset)
{
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
}

//void   QxrdNIDAQPlugin::setAnalogChannel(int chan)
//{
//  QMutexLocker lock(&m_Mutex);
////  printf("setAnalogChannel(%d)\n", chan);

//  int error;


//Error:
//  return;
//}

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
    DAQmxErrChk(DAQmxCreateTask("qxrd-output", &m_AOTaskHandle))
    DAQmxErrChk(DAQmxCreateAOVoltageChan (m_AOTaskHandle,
                                          qPrintable(tr("Dev1/ao%1").arg(chan)), NULL, -10.0, 10.0, DAQmx_Val_Volts, NULL))

    if (m_AOTaskHandle) {
      DAQmxErrChk(
          DAQmxCfgSampClkTiming(m_AOTaskHandle, NULL, rate, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, size)
          )

      DAQmxErrChk(
              DAQmxWriteAnalogF64(m_AOTaskHandle, size, false, -1, DAQmx_Val_GroupByChannel, wfm, &nsampwrt, NULL)
          )
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
    DAQmxErrChk(
      DAQmxWaitUntilTaskDone(m_AOTaskHandle, -1)
    )

    DAQmxErrChk(
      DAQmxStopTask(m_AOTaskHandle)
    )

    DAQmxErrChk(
      DAQmxStartTask(m_AOTaskHandle)
    )
  }

Error:
  return;
}

Q_EXPORT_PLUGIN2(qxrdnidaqplugin, QxrdNIDAQPlugin);


