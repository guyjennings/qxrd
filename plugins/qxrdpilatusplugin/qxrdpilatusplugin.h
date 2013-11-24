#ifndef QXRDPILATUSPLUGIN_H
#define QXRDPILATUSPLUGIN_H

#include <QObject>
#include "qxrddetectorplugininterface.h"

class QxrdPilatusPlugin : public QObject, public QxrdDetectorPluginInterface
{
  Q_OBJECT
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
  Q_PLUGIN_METADATA(IID DetectorPluginInterface_iid FILE "pilatus.json")
#endif
  Q_INTERFACES(QxrdDetectorPluginInterface)

public:
  QxrdPilatusPlugin();

  QString name() const;
};

#endif // QXRDPILATUSPLUGIN_H
