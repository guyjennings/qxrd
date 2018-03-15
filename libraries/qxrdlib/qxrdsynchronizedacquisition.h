#ifndef QXRDSYNCHRONIZEDACQUISITION_H
#define QXRDSYNCHRONIZEDACQUISITION_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include <QVector>
#include <QMutex>
#include "qcepproperty.h"
#include "qxrdacqcommon-ptr.h"
#include "qxrdacquisitionparameterpack-ptr.h"
#include "qxrddarkacquisitionparameterpack-ptr.h"
#include "qxrdsynchronizedacquisition-ptr.h"
#include "qxrdnidaq-ptr.h"
#include "qxrdsynchronizeddetectorchannel-ptr.h"
#include "qxrdsynchronizedoutputchannel-ptr.h"
#include "qxrdsynchronizedinputchannel-ptr.h"

class QXRD_EXPORT QxrdSynchronizedAcquisition : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  Q_INVOKABLE QxrdSynchronizedAcquisition(QString name);
  virtual ~QxrdSynchronizedAcquisition();

  void initialize(QcepObjectWPtr parent);

  static void registerMetaTypes();

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

public slots:
  int detectorCount();
  QxrdSynchronizedDetectorChannelPtr detector(int n);
  void newDetector(int before);
  void deleteDetector(int n);
  void renumberDetectors();

  int detectorDeviceCount();
  QString detectorDeviceName(int n);

  int outputCount();
  QxrdSynchronizedOutputChannelPtr output(int n);
  void newOutput(int before);
  void deleteOutput(int n);
  void renumberOutputs();

  int outputDeviceCount();
  QString outputDeviceName(int n);

  int inputCount();
  QxrdSynchronizedInputChannelPtr input(int n);
  void newInput(int before);
  void deleteInput(int n);
  void renumberInputs();

  int inputDeviceCount();
  QString inputDeviceName(int n);

  QVector<double> evaluateInputs();
  double          evaluateInput(int ch);

  void updateWaveforms();

  QxrdAcqCommonWPtr acquisition();

signals:
  void detectorCountChanged(int n);
  void outputCountChanged(int n);
  void inputCountChanged(int n);

public:

signals:
  void waveformParametersChanged();
  void waveformsChanged();

public slots:
//  void setManualOutput();
//  void triggerOnce();

public:
  void prepareForAcquisition(QxrdAcquisitionParameterPackWPtr parms);
  void prepareForDarkAcquisition(QxrdDarkAcquisitionParameterPackWPtr parms);
  void finishedAcquisition();
  void acquiredFrameAvailable(int currentPhase);
  void setNIDAQPlugin(QxrdNIDAQWPtr nidaqPlugin);
  QxrdNIDAQWPtr nidaqPlugin() const;

//  QVector<double>  outputTimes();
//  QVector<double>  outputVoltage();

  QxrdAcquisitionParameterPackWPtr parms();

//private:
//  QxrdAcqCommonWPtr acquisition();

private:
  QxrdAcqCommonWPtr                    m_Acquisition;
  QxrdAcquisitionParameterPackWPtr     m_AcquisitionParms;
  QxrdDarkAcquisitionParameterPackWPtr m_DarkAcquisitionParms;
  QxrdNIDAQWPtr                        m_NIDAQPlugin;
  int                                  m_SyncMode;
//  QVector<double>                      m_OutputTimes;
//  QVector<double>                      m_OutputVoltage;

  QVector<QxrdSynchronizedDetectorChannelPtr> m_Detectors;
  QVector<QxrdSynchronizedOutputChannelPtr>   m_Outputs;
  QVector<QxrdSynchronizedInputChannelPtr>    m_Inputs;
};

Q_DECLARE_METATYPE(QxrdSynchronizedAcquisition*)

#endif // QXRDSYNCHRONIZEDACQUISITION_H
