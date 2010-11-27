#include "qxrdnidaqplugin.h"
#include "NIDAQmx.h"

QxrdNIDAQPlugin::QxrdNIDAQPlugin()
{
  TaskHandle taskHandle;
  int res = DAQmxCreateTask("", &taskHandle);
}

QString QxrdNIDAQPlugin::name() const
{
  return "NI DAQ Card";
}

Q_EXPORT_PLUGIN2(qxrdnidaqplugin, QxrdNIDAQPlugin);


