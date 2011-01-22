#include "qxrdnidaqplugin.h"
#include "NIDAQmx.h"
#include <QVector>
#include <stdio.h>

#define DAQmxErrChk(functionCall) { if( DAQmxFailed(error=(functionCall)) ) { printf("%s:%d NI-DAQ Error %d\n",__FILE__,__LINE__,error); goto Error; } }

QxrdNIDAQPlugin::QxrdNIDAQPlugin()
{
}

QString QxrdNIDAQPlugin::name() const
{
  return "NI DAQ Card";
}

void   QxrdNIDAQPlugin::aoSet(QString chan, double val)
{
  int error;
  TaskHandle taskHandle = 0;

  DAQmxErrChk(DAQmxCreateTask("", &taskHandle))

  DAQmxErrChk(DAQmxCreateAOVoltageChan (taskHandle, qPrintable(chan), NULL, -10.0, 10.0, DAQmx_Val_Volts, NULL))

  DAQmxErrChk(DAQmxWriteAnalogScalarF64(taskHandle, true, 10.0, val, NULL))

Error:
  if (taskHandle) {
    DAQmxClearTask(taskHandle);
  }
}

double QxrdNIDAQPlugin::aiGet(QString chan)
{
  int error;
  TaskHandle taskHandle = 0;
  float64 res = 0;

  DAQmxErrChk(DAQmxCreateTask("", &taskHandle))

  DAQmxErrChk(DAQmxCreateAIVoltageChan (taskHandle, qPrintable(chan), NULL, DAQmx_Val_Cfg_Default, -10.0, 10.0, DAQmx_Val_Volts, NULL))

  DAQmxErrChk(DAQmxReadAnalogScalarF64(taskHandle, 10.0, &res, NULL))

Error:
  if (taskHandle) {
    DAQmxClearTask(taskHandle);
  }

  return res;
}

void   QxrdNIDAQPlugin::aoWave(QString chan, int type, double freq, double amplitude, double offset)
{
  int error;
  TaskHandle taskHandle = 0;
  float64 res = 0;
  QVector<float64> waveform(1024);

  DAQmxErrChk(DAQmxCreateTask("", &taskHandle))

  DAQmxErrChk(DAQmxCreateAOVoltageChan (taskHandle, qPrintable(chan), NULL, -10.0, 10.0, DAQmx_Val_Volts, NULL))


  for (int i=0; i<1024; i++) {
    waveform[i] = (i>512 ? 1 : -1);
  }

  int32 numWritten;

  DAQmxErrChk(DAQmxWriteAnalogF64(taskHandle, 1024, true, 10.0, DAQmx_Val_GroupByChannel, waveform.data(), &numWritten, NULL))

  DAQmxErrChk(DAQmxStartTask(taskHandle))

Error:
  if (taskHandle) {
    DAQmxClearTask(taskHandle);
  }
}

Q_EXPORT_PLUGIN2(qxrdnidaqplugin, QxrdNIDAQPlugin);


