#ifndef QXRDNIDAQ_H
#define QXRDNIDAQ_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qxrdsynchronizedacquisition-ptr.h"
#include "qxrdacqcommon-ptr.h"
#include "qxrdacquisitionparameterpack-ptr.h"
#include "qxrddarkacquisitionparameterpack-ptr.h"

class QXRD_EXPORT QxrdNIDAQ : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  QxrdNIDAQ(QString name);

  virtual QString name() const = 0;

  virtual void registerMetaTypes() = 0;

  virtual void setErrorOutput(QObject *errors) = 0;

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

  virtual int detectorDeviceCount() = 0;
  virtual QString detectorDeviceName(int n) = 0;

  virtual int outputDeviceCount() = 0;
  virtual QString outputDeviceName(int n) = 0;

  virtual int inputDeviceCount() = 0;
  virtual QString inputDeviceName(int n) = 0;

  virtual void updateSyncWaveforms     (QxrdSynchronizedAcquisitionWPtr      s,
                                        QxrdAcquisitionParameterPackWPtr     p) = 0;
  virtual void prepareForAcquisition   (QxrdSynchronizedAcquisitionWPtr      s,
                                        QxrdAcquisitionParameterPackWPtr     p) = 0;
  virtual void prepareForDarkAcquistion(QxrdSynchronizedAcquisitionWPtr      s,
                                        QxrdDarkAcquisitionParameterPackWPtr p) = 0;
};

#define NIDAQInterface_iid "gov.anl.aps.cep.Qxrd.NIDAQ"
Q_DECLARE_INTERFACE(QxrdNIDAQ, NIDAQInterface_iid)

#endif // QXRDNIDAQ_H
