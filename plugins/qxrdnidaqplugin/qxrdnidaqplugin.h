#ifndef QXRDNIDAQPLUGIN_H
#define QXRDNIDAQPLUGIN_H

#include <QObject>
#include "qxrdnidaqplugininterface.h"

class QxrdNIDAQPlugin : public QObject, public QxrdNIDAQPluginInterface
{
  Q_OBJECT;
  Q_INTERFACES(QxrdNIDAQPluginInterface);

public:
    QxrdNIDAQPlugin();

    QString name() const;
};

#endif // QXRDNIDAQPLUGIN_H
