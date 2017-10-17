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

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#else
Q_EXPORT_PLUGIN2(qxrdpilatusplugin, QxrdPilatusPlugin)
#endif


