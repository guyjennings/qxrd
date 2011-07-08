#ifndef QXRDAREADETECTORPLUGIN_H
#define QXRDAREADETECTORPLUGIN_H

#include <QObject>
#include "qxrddetectorplugininterface.h"

class QxrdAreaDetectorPlugin : public QObject, public QxrdDetectorPluginInterface
{
  Q_OBJECT
  Q_INTERFACES(QxrdDetectorPluginInterface)

public:
    QxrdAreaDetectorPlugin();

    QString name() const;
};

#endif // QXRDAREADETECTORPLUGIN_H
