#include "qxrdnidaqplugin.h"
#include "NIDAQmx.h"

#define DAQmxErrChk(functionCall) { if( DAQmxFailed(error=(functionCall)) ) { goto Error; } }

QxrdNIDAQPlugin::QxrdNIDAQPlugin()
{
  TaskHandle taskHandle;
  int res = DAQmxCreateTask("", &taskHandle);
}

QString QxrdNIDAQPlugin::name() const
{
  return "NI DAQ Card";
}

void   QxrdNIDAQPlugin::aoSet(QString chan, double val)
{
  int error;
  TaskHandle taskHandle = 0;

  DAQmxErrChk(DAQmxCreateTask("", &taskHandle));

  DAQmxErrChk(DAQmxCreateAOVoltageChan (taskHandle, qPrintable(chan), NULL, -10.0, 10.0, DAQmx_Val_Volts, NULL));

  DAQmxErrChk(DAQmxWriteAnalogScalarF64(taskHandle, true, 10.0, val, NULL));

Error:
  if (taskHandle) {
    DAQmxClearTask(taskHandle);
  }
}

double QxrdNIDAQPlugin::aiGet(QString chan)
{
  return 42;
}

Q_EXPORT_PLUGIN2(qxrdnidaqplugin, QxrdNIDAQPlugin);


