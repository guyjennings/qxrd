#include "qxrdareadetectorplugin.h"

QxrdAreaDetectorPlugin::QxrdAreaDetectorPlugin()
{
}

QString QxrdAreaDetectorPlugin::name() const
{
  return "EPICS Area Detector Plugin";
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#else
Q_EXPORT_PLUGIN2(qxrdareadetectorplugin, QxrdAreaDetectorPlugin)
#endif
