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

public slots:
  void   aoSet(QString chan, double val);
  double aiGet(QString chan);
  void   aoWave(QString chan, int type, double freq, double amplitude, double offset);
};

#endif // QXRDNIDAQPLUGIN_H
