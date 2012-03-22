#ifndef QXRDACQUISITIONEXTRAINPUTSCHANNEL_H
#define QXRDACQUISITIONEXTRAINPUTSCHANNEL_H

#include <QObject>
#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdacquisitionextrainputs-ptr.h"

class QxrdAcquisitionExtraInputsChannel : public QObject
{
  Q_OBJECT
public:
  explicit QxrdAcquisitionExtraInputsChannel(int chnum, QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr doc, QxrdAcquisitionExtraInputsWPtr xtra);
  
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

signals:
  void reinitializeNeeded();

public slots:
  QVector<double> readChannel();
  double          evaluateChannel();
  double          sumChannel();
  double          averageChannel();
  double          maximumChannel();
  double          minimumChannel();

public:
  enum {
    ModeSummed,
    ModeAveraged,
    ModeMaximum,
    ModeMinimum
  };

  Q_PROPERTY(int channelNumber READ get_ChannelNumber WRITE set_ChannelNumber STORED false)
  QCEP_INTEGER_PROPERTY(ChannelNumber)

  Q_PROPERTY(bool enabled READ get_Enabled WRITE set_Enabled)
  QCEP_BOOLEAN_PROPERTY(Enabled)

  Q_PROPERTY(bool plotted READ get_Plotted WRITE set_Plotted)
  QCEP_BOOLEAN_PROPERTY(Plotted)

  Q_PROPERTY(QString channelName READ get_ChannelName WRITE set_ChannelName)
  QCEP_STRING_PROPERTY(ChannelName)

//  Q_PROPERTY(int kind READ get_Kind WRITE set_Kind)
//  QCEP_INTEGER_PROPERTY(Kind)

  Q_PROPERTY(int mode READ get_Mode WRITE set_Mode)
  QCEP_INTEGER_PROPERTY(Mode)

  Q_PROPERTY(bool saveWave READ get_SaveWave WRITE set_SaveWave)
  QCEP_BOOLEAN_PROPERTY(SaveWave)

  Q_PROPERTY(double min READ get_Min WRITE set_Min)
  QCEP_DOUBLE_PROPERTY(Min)

  Q_PROPERTY(double max READ get_Max WRITE set_Max)
  QCEP_DOUBLE_PROPERTY(Max)

  Q_PROPERTY(double start READ get_Start WRITE set_Start)
  QCEP_DOUBLE_PROPERTY(Start)

  Q_PROPERTY(double end READ get_End WRITE set_End)
  QCEP_DOUBLE_PROPERTY(End)

  Q_PROPERTY(int physicalChannel READ get_PhysicalChannel WRITE set_PhysicalChannel STORED false)
  QCEP_INTEGER_PROPERTY(PhysicalChannel)

  Q_PROPERTY(double value READ get_Value WRITE set_Value STORED false)
  QCEP_DOUBLE_PROPERTY(Value)

  Q_PROPERTY(QcepDoubleVector waveform READ get_Waveform WRITE set_Waveform STORED false)
  QCEP_DOUBLE_VECTOR_PROPERTY(Waveform)

private:
  mutable QMutex                 m_Mutex;
  QxrdExperimentWPtr             m_Experiment;
  QxrdAcquisitionExtraInputsWPtr m_ExtraInputs;
};

#endif // QXRDACQUISITIONEXTRAINPUTSCHANNEL_H
