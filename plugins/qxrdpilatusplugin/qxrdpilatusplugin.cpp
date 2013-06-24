#include "qxrdpilatusplugin.h"

QxrdPilatusPlugin::QxrdPilatusPlugin()
{
}

QString QxrdPilatusPlugin::name() const
{
  return "Pilatus Detector";
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#else
Q_EXPORT_PLUGIN2(qxrdpilatusplugin, QxrdPilatusPlugin)
#endif


