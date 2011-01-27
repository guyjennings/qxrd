#ifndef QXRDNIDAQPLUGININTERFACE_H
#define QXRDNIDAQPLUGININTERFACE_H

#include <QtPlugin>
#include "qxrdnamedplugininterface.h"

class QxrdNIDAQPluginInterface : public QxrdNamedPluginInterface
{
public:
  virtual ~QxrdNIDAQPluginInterface() {}

public:
  virtual void   setAnalogOut(int chan, double val) = 0;
  virtual void   aoSet(QString chan, double val) = 0;
  virtual double aiGet(QString chan) = 0;
};

Q_DECLARE_INTERFACE(QxrdNIDAQPluginInterface, "gov.anl.aps.cep.Qxrd.NIDAQInterface/1.0");

#endif // QXRDNIDAQPLUGININTERFACE_H
