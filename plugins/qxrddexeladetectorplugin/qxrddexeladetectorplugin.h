#ifndef QXRDDEXELADETECTORPLUGIN_H
#define QXRDDEXELADETECTORPLUGIN_H

#include <QObject>
#include "qxrddetectorplugininterface.h"
#include <QTimer>

class QxrdDexelaDetectorPlugin : public QObject, public QxrdDetectorPluginInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID DetectorPluginInterface_iid FILE "dexelaDetector.json")
  Q_INTERFACES(QxrdDetectorPluginInterface)

public:
    QxrdDexelaDetectorPlugin();

    QString name() const;

    QxrdDetectorInterfacePtr createDetector(QString name);
};

#endif // QXRDDEXELADETECTORPLUGIN_H
