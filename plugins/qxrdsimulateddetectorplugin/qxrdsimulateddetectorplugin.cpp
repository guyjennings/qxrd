#include "qxrdsimulateddetectorplugin.h"
#include "qxrddetectorinterface.h"

QxrdSimulatedDetectorPlugin::QxrdSimulatedDetectorPlugin()
{
}

QString QxrdSimulatedDetectorPlugin::name() const
{
  return "Simulated Detector Plugin";
}

QxrdDetectorInterfacePtr QxrdSimulatedDetectorPlugin::createDetector(QString name)
{
  static QxrdDetectorInterfacePtr res =
      QxrdDetectorInterfacePtr(new QxrdDetectorInterface(name));

  return res;
}
