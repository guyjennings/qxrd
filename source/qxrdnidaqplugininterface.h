#ifndef QXRDNIDAQPLUGININTERFACE_H
#define QXRDNIDAQPLUGININTERFACE_H

#include <QtPlugin>
#include "qxrdnamedplugininterface.h"

class QxrdNIDAQPluginInterface : public QxrdNamedPluginInterface
{
public:
  virtual ~QxrdNIDAQPluginInterface() {}

public:
//  virtual void setAnalogChannel(int chan) = 0;
  virtual void setAnalogWaveform(int chan, double rate, double wfm[], int size) = 0;
  virtual void triggerAnalogWaveform() = 0;
  virtual void pulseOutput() = 0;
};

Q_DECLARE_INTERFACE(QxrdNIDAQPluginInterface, "gov.anl.aps.cep.Qxrd.NIDAQInterface/1.0")

#endif // QXRDNIDAQPLUGININTERFACE_H
