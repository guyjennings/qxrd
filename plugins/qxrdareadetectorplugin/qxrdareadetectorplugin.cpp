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

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#else
Q_EXPORT_PLUGIN2(qxrdareadetectorplugin, QxrdAreaDetectorPlugin)
#endif
