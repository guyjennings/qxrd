#include "qxrdpilatusplugin.h"

QxrdPilatusPlugin::QxrdPilatusPlugin()
{
}

QString QxrdPilatusPlugin::name() const
{
  return "Pilatus Detector";
}

Q_EXPORT_PLUGIN2(qxrdpilatusplugin, QxrdPilatusPlugin);
\
