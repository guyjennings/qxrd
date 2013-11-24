#ifndef QXRDAREADETECTORPLUGIN_H
#define QXRDAREADETECTORPLUGIN_H

#include <QObject>
#include "qxrddetectorplugininterface.h"

class QxrdAreaDetectorPlugin : public QObject, public QxrdDetectorPluginInterface
{
  Q_OBJECT
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
  Q_PLUGIN_METADATA(IID DetectorPluginInterface_iid FILE "areaDetector.json")
#endif
  Q_INTERFACES(QxrdDetectorPluginInterface)

public:
    QxrdAreaDetectorPlugin();

    QString name() const;
};

#endif // QXRDAREADETECTORPLUGIN_H
