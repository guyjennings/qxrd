#ifndef QXRDSYNCHRONIZEDINPUTCHANNEL_H
#define QXRDSYNCHRONIZEDINPUTCHANNEL_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qxrdnidaq-ptr.h"
#include "qxrdacqcommon-ptr.h"
#include "qxrdsynchronizedacquisition-ptr.h"

class QXRD_EXPORT QxrdSynchronizedInputChannel : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  Q_INVOKABLE QxrdSynchronizedInputChannel(QString name);
  void initialize(QcepObjectWPtr parent);

  QString channelName();
  static int     channelModeCount();
  static QString channelMode(int n);

  double evaluateInput();

  void prepareForInput();

  enum {
    ModeSummed,
    ModeAveraged,
    ModeMaximum,
    ModeMinimum,
    ModeCount
  };

  enum {
    TriggerModeNone,
    TriggerModeEdgePos,
    TriggerModeEdgeNeg,
    TriggerModeLevelPos,
    TriggerModeLevelNeg,
    TriggerModeCount
  };

private:
  QxrdAcqCommonWPtr               m_AcqCommon;
  QxrdSynchronizedAcquisitionWPtr m_SynchronizedAcquisition;
  QxrdNIDAQWPtr                   m_NIDAQ;

public:
  Q_PROPERTY(int channelNumber READ get_ChannelNumber WRITE set_ChannelNumber)
  QCEP_INTEGER_PROPERTY(ChannelNumber)

  Q_PROPERTY(QString signalName READ get_SignalName WRITE set_SignalName)
  QCEP_STRING_PROPERTY(SignalName)

  Q_PROPERTY(int mode READ get_Mode WRITE set_Mode)
  QCEP_INTEGER_PROPERTY(Mode)

  Q_PROPERTY(bool saveWave READ get_SaveWave WRITE set_SaveWave)
  QCEP_BOOLEAN_PROPERTY(SaveWave)

  Q_PROPERTY(double min READ get_Min WRITE set_Min)
  QCEP_DOUBLE_PROPERTY(Min)

  Q_PROPERTY(double max READ get_Max WRITE set_Max)
  QCEP_DOUBLE_PROPERTY(Max)

  Q_PROPERTY(double sampleRate READ get_SampleRate WRITE set_SampleRate)
  QCEP_DOUBLE_PROPERTY(SampleRate)

  Q_PROPERTY(int nSamples READ get_NSamples WRITE set_NSamples STORED false)
  QCEP_INTEGER_PROPERTY(NSamples)

  Q_PROPERTY(double actualSampleRate READ get_ActualSampleRate WRITE set_ActualSampleRate STORED false)
  QCEP_DOUBLE_PROPERTY(ActualSampleRate)

  Q_PROPERTY(QcepDoubleVector timeValues READ get_TimeValues WRITE set_TimeValues STORED false)
  QCEP_DOUBLE_VECTOR_PROPERTY(TimeValues)

  Q_PROPERTY(QcepDoubleVector waveform READ get_Waveform WRITE set_Waveform STORED false)
  QCEP_DOUBLE_VECTOR_PROPERTY(Waveform)
};

Q_DECLARE_METATYPE(QxrdSynchronizedInputChannel*)

#endif // QXRDSYNCHRONIZEDINPUTCHANNEL_H
