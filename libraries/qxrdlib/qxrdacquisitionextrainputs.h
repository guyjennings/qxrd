#ifndef QXRDACQUISITIONEXTRAINPUTS_H
#define QXRDACQUISITIONEXTRAINPUTS_H

#include "qxrdlib_global.h"
#include "qcepserializableobject.h"
#include "qcepproperty.h"
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

class QXRD_EXPORT QxrdAcquisitionExtraInputs : public QcepSerializableObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdAcquisitionExtraInputs(QString name);
  static QxrdAcquisitionExtraInputsPtr newAcquisitionExtraInputs();
  virtual ~QxrdAcquisitionExtraInputs();

public:
  void addChildPtr(QcepObjectPtr child);
  void removeChildPtr(QcepObjectPtr child);

  void setNIDAQPlugin(QxrdNIDAQPluginInterfacePtr nidaqPlugin);
  QxrdNIDAQPluginInterfacePtr nidaqPlugin() const;

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

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

private:
  QxrdAcquisitionWPtr acquisition();
  
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
  QVector<QxrdAcquisitionExtraInputsChannelPtr> m_Channels;
  QxrdNIDAQPluginInterfacePtr m_NIDAQPlugin;
  QVector< QVector<double> >  m_ChannelData;
};

Q_DECLARE_METATYPE(QxrdAcquisitionExtraInputs*)

#endif // QXRDACQUISITIONEXTRAINPUTS_H
