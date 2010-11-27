#include "qxrdnidaqplugin.h"
#include "NIDAQmx.h"

QxrdNIDAQPlugin::QxrdNIDAQPlugin()
{
}

QString QxrdNIDAQPlugin::name() const
{
  return "NI DAQ Card";
}

Q_EXPORT_PLUGIN2(qxrdnidaqplugin, QxrdNIDAQPlugin);


