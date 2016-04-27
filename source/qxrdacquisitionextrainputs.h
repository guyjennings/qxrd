#ifndef QXRDACQUISITIONEXTRAINPUTS_H
#define QXRDACQUISITIONEXTRAINPUTS_H

#include "qcepobject.h"
#include "qcepproperty.h"
#include "qcepsettingssaver-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrdnidaqplugininterface-ptr.h"
#include "qxrdnidaqplugininterface.h"
#include "qcepimagedata.h"
#include "qcepimagedata-ptr.h"
#include "qxrdacquisitionextrainputschannel.h"
#include "qxrdacquisitionextrainputschannel-ptr.h"
#include <QSettings>
#include <QMutex>
#include "qxrdacquisitionparameterpack-ptr.h"

class QxrdAcquisitionExtraInputs : public QcepObject
{
  Q_OBJECT
public:
  explicit QxrdAcquisitionExtraInputs(QxrdExperimentWPtr    doc,
                                      QxrdAcquisitionWPtr   acq);
  void initialize();
  virtual ~QxrdAcquisitionExtraInputs();

public:
  void setNIDAQPlugin(QxrdNIDAQPluginInterfacePtr nidaqPlugin);
  QxrdNIDAQPluginInterfacePtr nidaqPlugin() const;

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;

signals:
  void newDataAvailable();
  void channelCountChanged();

public slots:
  void prepareForAcquisition(QxrdAcquisitionParameterPackWPtr parms);
  void initiate();
  void reinitiate();
  void acquire();
  void logToImage(QcepUInt16ImageDataPtr img);
  void finish();
  void timerDone();
  void appendChannel(int ch = -1);
  void removeChannel(int ch = -1);

  QVector<double> evaluateChannels();
  double          evaluateChannel(int ch);
  QVector<double> readChannel(int ch);
  QVector<double> readXChannel();
  double          averageChannel(int ch);
  double          sumChannel(int ch);
  QVector<QxrdAcquisitionExtraInputsChannelPtr> channels() const;
  QxrdAcquisitionExtraInputsChannelPtr channel(int chan) const;

public:
  Q_PROPERTY(bool enabled READ get_Enabled WRITE set_Enabled STORED false)
  QCEP_BOOLEAN_PROPERTY(Enabled)

  Q_PROPERTY(bool skipping READ get_Skipping WRITE set_Skipping STORED false)
  QCEP_BOOLEAN_PROPERTY(Skipping)

  Q_PROPERTY(double sampleRate READ get_SampleRate WRITE set_SampleRate)
  QCEP_DOUBLE_PROPERTY(SampleRate)

  Q_PROPERTY(double acquireDelay READ get_AcquireDelay WRITE set_AcquireDelay)
  QCEP_DOUBLE_PROPERTY(AcquireDelay)

  Q_PROPERTY(double exposureTime READ get_ExposureTime WRITE set_ExposureTime STORED false)
  QCEP_DOUBLE_PROPERTY(ExposureTime)

  Q_PROPERTY(QString deviceName READ get_DeviceName WRITE set_DeviceName)
  QCEP_STRING_PROPERTY(DeviceName)

private:
  mutable QMutex              m_Mutex;
  QxrdExperimentWPtr          m_Experiment;
  QxrdAcquisitionWPtr         m_Acquisition;
  QVector<QxrdAcquisitionExtraInputsChannelPtr> m_Channels;
  QxrdNIDAQPluginInterfacePtr m_NIDAQPlugin;
  QVector< QVector<double> >  m_ChannelData;
};

#endif // QXRDACQUISITIONEXTRAINPUTS_H
