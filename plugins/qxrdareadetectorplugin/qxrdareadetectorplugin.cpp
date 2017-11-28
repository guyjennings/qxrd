#include "qxrdareadetectorplugin.h"
#include "qxrddetectorinterface.h"

QxrdAreaDetectorPlugin::QxrdAreaDetectorPlugin()
{
}

QString QxrdAreaDetectorPlugin::name() const
{
  return "EPICS Area Detector Plugin";
}

QxrdDetectorInterfacePtr QxrdAreaDetectorPlugin::createDetector(QString name)
{
  static QxrdDetectorInterfacePtr res =
      QxrdDetectorInterfacePtr(new QxrdDetectorInterface(name));

  return res;
}
