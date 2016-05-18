#ifndef QXRDACQUISITIONEXTRAINPUTSCHANNEL_H
#define QXRDACQUISITIONEXTRAINPUTSCHANNEL_H

#include "qcepserializableobject.h"
#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdacquisitionextrainputs-ptr.h"
#include "qcepsettingssaver-ptr.h"

class QxrdAcquisitionExtraInputsChannel : public QcepSerializableObject
{
  Q_OBJECT
public:
  QxrdAcquisitionExtraInputsChannel(int chnum);
  Q_INVOKABLE QxrdAcquisitionExtraInputsChannel(QString name);

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

signals:
  void reinitiateNeeded();

public slots:
  QVector<double> readChannel();
  double          evaluateChannel();
  double          sumChannel();
  double          averageChannel();
  double          maximumChannel();
  double          minimumChannel();
  int             startIndex();
  int             endIndex();
  bool            evaluateTrigger();
  bool            evalTrig(int polarity, bool edgeTrig);

private:
  QxrdAcquisitionExtraInputsWPtr extraInputs();

public:
  enum {
    ModeSummed,
    ModeAveraged,
    ModeMaximum,
    ModeMinimum
  };

  enum {
    TriggerModeNone,
    TriggerModeEdgePos,
    TriggerModeEdgeNeg,
    TriggerModeLevelPos,
    TriggerModeLevelNeg
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

  Q_PROPERTY(int triggerMode READ get_TriggerMode WRITE set_TriggerMode)
  QCEP_INTEGER_PROPERTY(TriggerMode)

  Q_PROPERTY(double triggerLevel READ get_TriggerLevel WRITE set_TriggerLevel)
  QCEP_DOUBLE_PROPERTY(TriggerLevel)

  Q_PROPERTY(double triggerHysteresis READ get_TriggerHysteresis WRITE set_TriggerHysteresis)
  QCEP_DOUBLE_PROPERTY(TriggerHysteresis)

  Q_PROPERTY(int physicalChannel READ get_PhysicalChannel WRITE set_PhysicalChannel STORED false)
  QCEP_INTEGER_PROPERTY(PhysicalChannel)

  Q_PROPERTY(double value READ get_Value WRITE set_Value STORED false)
  QCEP_DOUBLE_PROPERTY(Value)

  Q_PROPERTY(bool triggered READ get_Triggered WRITE set_Triggered STORED false)
  QCEP_BOOLEAN_PROPERTY(Triggered)

  Q_PROPERTY(int nLow READ get_NLow WRITE set_NLow STORED false)
  QCEP_INTEGER_PROPERTY(NLow)

  Q_PROPERTY(int nHigh READ get_NHigh WRITE set_NHigh STORED false)
  QCEP_INTEGER_PROPERTY(NHigh)

  Q_PROPERTY(QcepDoubleVector waveform READ get_Waveform WRITE set_Waveform STORED false)
  QCEP_DOUBLE_VECTOR_PROPERTY(Waveform)

private:
  mutable QMutex                 m_Mutex;
  QxrdAcquisitionExtraInputsWPtr m_ExtraInputs;
};

Q_DECLARE_METATYPE(QxrdAcquisitionExtraInputsChannel*)

#endif // QXRDACQUISITIONEXTRAINPUTSCHANNEL_H
