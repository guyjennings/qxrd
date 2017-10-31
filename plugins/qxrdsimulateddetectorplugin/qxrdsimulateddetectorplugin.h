#ifndef QXRDSIMULATEDDETECTORPLUGIN_H
#define QXRDSIMULATEDDETECTORPLUGIN_H

#include <QObject>
#include "qxrddetectorplugininterface.h"
#include <QTimer>

class QxrdSimulatedDetectorPlugin : public QObject, public QxrdDetectorPluginInterface
{
  Q_OBJECT
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
  Q_PLUGIN_METADATA(IID DetectorPluginInterface_iid FILE "simulatedDetector.json")
#endif
  Q_INTERFACES(QxrdDetectorPluginInterface)

public:
    QxrdSimulatedDetectorPlugin();

    QString name() const;

    QxrdDetectorInterfacePtr createDetector(QString name);
};

#endif // QXRDSIMULATEDDETECTORPLUGIN_H
