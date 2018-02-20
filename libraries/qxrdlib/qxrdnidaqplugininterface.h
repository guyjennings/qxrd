#ifndef QXRDNIDAQPLUGININTERFACE_H
#define QXRDNIDAQPLUGININTERFACE_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include <QtPlugin>
#include "qcepobject-ptr.h"

class QXRD_EXPORT QxrdNIDAQPluginInterface
{
public:
  virtual ~QxrdNIDAQPluginInterface() {}

public:
  virtual QString name() const = 0;
  virtual void initialize(QcepObjectWPtr parent) = 0;

  virtual void setErrorOutput(QObject *errors) = 0;

//  virtual void setAnalogChannel(int chan) = 0;
  virtual void setAnalogWaveform(QString chan, double rate, double wfm[], int size) = 0;
  virtual void triggerAnalogWaveform() = 0;
  virtual void pulseOutput() = 0;

  virtual QStringList deviceNames() = 0;
  virtual QString     deviceType(QString device) = 0;
  virtual int         deviceIsSimulated(QString device) = 0;
  virtual QStringList deviceAIChannels(QString device) = 0;
  virtual QStringList deviceAOChannels(QString device) = 0;
  virtual QStringList deviceDIPorts(QString device) = 0;
  virtual QStringList deviceDILines(QString port) = 0;
  virtual QStringList deviceDOPorts(QString device) = 0;
  virtual QStringList deviceDOLines(QString port) = 0;
  virtual QStringList deviceCIChannels(QString device) = 0;
  virtual QStringList deviceCOChannels(QString device) = 0;

  virtual double getAnalogInput(QString channelName) = 0;
  virtual void setAnalogOutput(QString channelName, double value) = 0;
  virtual void setAnalogOutput(double value) = 0;

  virtual int prepareContinuousInput(double sampleRate,
                                     double acquireDelay,
                                     double exposureTime,
                                     QStringList chans,
                                     QVector<double> minVals,
                                     QVector<double> maxVals) = 0;
  virtual int countContinuousInput() = 0;
  virtual int readContinuousInput() = 0;
  virtual QVector<double> readContinuousInputChannel(int ch) = 0;
  virtual void finishContinuousInput() = 0;
};

#define QxrdNIDAQPluginInterface_iid "gov.anl.aps.cep.Qxrd.NIDAQInterface/1.0"
Q_DECLARE_INTERFACE(QxrdNIDAQPluginInterface, QxrdNIDAQPluginInterface_iid)

#endif // QXRDNIDAQPLUGININTERFACE_H
