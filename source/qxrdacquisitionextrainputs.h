#ifndef QXRDACQUISITIONEXTRAINPUTS_H
#define QXRDACQUISITIONEXTRAINPUTS_H

#include <QObject>
#include "qcepproperty.h"
#include "qxrdsettingssaver-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrdnidaqplugininterface-ptr.h"
#include "qxrdnidaqplugininterface.h"
#include "qxrdimagedata.h"
#include "qxrdimagedata-ptr.h"
#include "qxrdacquisitionextrainputschannel.h"
#include "qxrdacquisitionextrainputschannel-ptr.h"
#include "qxrdobjectnamer.h"
#include <QSettings>
#include <QMutex>

class QxrdAcquisitionExtraInputs : public QObject
{
  Q_OBJECT
public:
  explicit QxrdAcquisitionExtraInputs(QxrdSettingsSaverWPtr saver,
                                      QxrdExperimentWPtr    doc,
                                      QxrdAcquisitionWPtr   acq);
  
public:
  void setNIDAQPlugin(QxrdNIDAQPluginInterfacePtr nidaqPlugin);
  QxrdNIDAQPluginInterfacePtr nidaqPlugin() const;

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

signals:
  void newDataAvailable();

public slots:
  void prepareForAcquisition(QxrdAcquisitionParameterPack *parms);
  void initialize();
  void reinitialize();
  void acquire();
  void logToImage(QxrdInt16ImageDataPtr img);
  void finish();
  void timerDone();
  void appendChannel(int ch = -1);
  void removeChannel(int ch = -1);

  QcepDoubleList  evaluateChannels();
  double          evaluateChannel(int ch);
  QVector<double> readChannel(int ch);
  QVector<double> readXChannel();
  double          averageChannel(int ch);
  double          sumChannel(int ch);
  QVector<QxrdAcquisitionExtraInputsChannelPtr> channels() const;
  QxrdAcquisitionExtraInputsChannelPtr channel(int chan) const;

private:
  QxrdObjectNamer m_ObjectNamer;

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

private:
  mutable QMutex              m_Mutex;
  QxrdExperimentWPtr          m_Experiment;
  QxrdAcquisitionWPtr         m_Acquisition;
  QxrdSettingsSaverWPtr       m_Saver;
  QVector<QxrdAcquisitionExtraInputsChannelPtr> m_Channels;
  QxrdNIDAQPluginInterfacePtr m_NIDAQPlugin;
  QVector< QVector<double> >  m_ChannelData;
};

#endif // QXRDACQUISITIONEXTRAINPUTS_H
