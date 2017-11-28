#include "qxrdpilatusplugin.h"
#include "qxrddetectorinterface.h"

QxrdPilatusPlugin::QxrdPilatusPlugin()
{
}

QString QxrdPilatusPlugin::name() const
{
  return "Pilatus Detector";
}

QxrdDetectorInterfacePtr QxrdPilatusPlugin::createDetector(QString name)
{
  static QxrdDetectorInterfacePtr res =
      QxrdDetectorInterfacePtr(new QxrdDetectorInterface(name));

  return res;
}
