#ifndef QXRDNIDAQPLUGININTERFACE_H
#define QXRDNIDAQPLUGININTERFACE_H

#include <QtPlugin>
#include <QSharedPointer>
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

  virtual QStringList deviceNames() = 0;
  virtual QStringList deviceAIChannels(QString device) = 0;
  virtual QStringList deviceAOChannels(QString device) = 0;
  virtual QStringList deviceDIPorts(QString device) = 0;
  virtual QStringList deviceDILines(QString port) = 0;
  virtual QStringList deviceDOPorts(QString device) = 0;
  virtual QStringList deviceDOLines(QString port) = 0;
  virtual QStringList deviceCIChannels(QString device) = 0;
  virtual QStringList deviceCOChannels(QString device) = 0;
};

Q_DECLARE_INTERFACE(QxrdNIDAQPluginInterface, "gov.anl.aps.cep.Qxrd.NIDAQInterface/1.0")

#endif // QXRDNIDAQPLUGININTERFACE_H
