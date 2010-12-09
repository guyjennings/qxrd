#ifndef QXRDNIDAQPLUGININTERFACE_H
#define QXRDNIDAQPLUGININTERFACE_H

#include <QtPlugin>
#include "qxrdnamedplugininterface.h"

class QxrdNIDAQPluginInterface : public QxrdNamedPluginInterface
{
  Q_OBJECT;

public:
  virtual ~QxrdNIDAQPluginInterface() {}

public slots:
  virtual void   aoSet(QString chan, double val) = 0;
  virtual double aiGet(QString chan) = 0;
};

Q_DECLARE_INTERFACE(QxrdNIDAQPluginInterface, "gov.anl.aps.cep.Qxrd.NIDAQInterface/1.0");

#endif // QXRDNIDAQPLUGININTERFACE_H
