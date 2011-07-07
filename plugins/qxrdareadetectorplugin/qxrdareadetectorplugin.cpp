#include "qxrdareadetectorplugin.h"

QxrdAreaDetectorPlugin::QxrdAreaDetectorPlugin()
{
}

QString QxrdAreaDetectorPlugin::name() const
{
  return "EPICS Area Detector Plugin";
}

Q_EXPORT_PLUGIN2(qxrdareadetectorplugin, QxrdAreaDetectorPlugin)
