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

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#else
Q_EXPORT_PLUGIN2(qxrdsimulateddetectorplugin, QxrdSimulatedDetectorPlugin)
#endif
