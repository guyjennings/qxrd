#ifndef QXRDDEXELADETECTORPLUGIN_H
#define QXRDDEXELADETECTORPLUGIN_H

#include <QObject>
#include "qxrddetectorplugininterface.h"
#include <QTimer>

class QxrdDexelaDetectorPlugin : public QObject, public QxrdDetectorPluginInterface
{
  Q_OBJECT
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
  Q_PLUGIN_METADATA(IID DetectorPluginInterface_iid FILE "dexelaDetector.json")
#endif
  Q_INTERFACES(QxrdDetectorPluginInterface)

public:
    QxrdDexelaDetectorPlugin();

    QString name() const;

    QxrdDetectorInterfacePtr createDetector(QString name);
};

#endif // QXRDDEXELADETECTORPLUGIN_H
