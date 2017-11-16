#include "qxrddexeladetectorplugin.h"
#include "qxrddetectorinterface.h"

QxrdDexelaDetectorPlugin::QxrdDexelaDetectorPlugin()
{
}

QString QxrdDexelaDetectorPlugin::name() const
{
  return "Dexela Detector Plugin";
}

QxrdDetectorInterfacePtr QxrdDexelaDetectorPlugin::createDetector(QString name)
{
  static QxrdDetectorInterfacePtr res =
      QxrdDetectorInterfacePtr(new QxrdDetectorInterface(name));

  return res;
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#else
Q_EXPORT_PLUGIN2(qxrddexeladetectorplugin, QxrdDexelaDetectorPlugin)
#endif
