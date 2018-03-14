#ifndef QXRDSYNCHRONIZEDOUTPUTCHANNEL_H
#define QXRDSYNCHRONIZEDOUTPUTCHANNEL_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qxrdnidaq-ptr.h"

class QXRD_EXPORT QxrdSynchronizedOutputChannel : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  Q_INVOKABLE QxrdSynchronizedOutputChannel(QString name);
  void initialize(QcepObjectWPtr parent);

  QString channelName();
  static int channelModeCount();
  static QString channelMode(int n);

  enum {
    ModeNone,
    ModeStepped,
    ModeContinuous,
    ModeCount
  };

  enum {
    WaveformSquare,
    WaveformSine,
    WaveformTriangle,
    WaveformRamp,
    WaveformBipolarTriangle,
    WaveformCount
  };

  static int waveformModeCount();
  static QString waveformMode(int n);

private:
  QxrdNIDAQWPtr m_NIDAQ;

public:
  Q_PROPERTY(int channelNumber READ get_ChannelNumber WRITE set_ChannelNumber)
  QCEP_INTEGER_PROPERTY(ChannelNumber)

  Q_PROPERTY(QString signalName READ get_SignalName WRITE set_SignalName)
  QCEP_STRING_PROPERTY(SignalName)

  Q_PROPERTY(int mode READ get_Mode WRITE set_Mode)
  QCEP_INTEGER_PROPERTY(Mode)

  Q_PROPERTY(int waveformMode READ get_WaveformMode WRITE set_WaveformMode)
  QCEP_INTEGER_PROPERTY(WaveformMode)

  Q_PROPERTY(double startV READ get_StartV WRITE set_StartV)
  QCEP_DOUBLE_PROPERTY(StartV)

  Q_PROPERTY(double endV READ get_EndV WRITE set_EndV)
  QCEP_DOUBLE_PROPERTY(EndV)

  Q_PROPERTY(double symmetry READ get_Symmetry WRITE set_Symmetry)
  QCEP_DOUBLE_PROPERTY(Symmetry)

  Q_PROPERTY(double phaseShift READ get_PhaseShift WRITE set_PhaseShift)
  QCEP_DOUBLE_PROPERTY(PhaseShift)

  Q_PROPERTY(double sampleRate READ get_SampleRate WRITE set_SampleRate)
  QCEP_DOUBLE_PROPERTY(SampleRate)

  Q_PROPERTY(QcepDoubleVector waveform READ get_Waveform WRITE set_Waveform STORED false)
  QCEP_DOUBLE_VECTOR_PROPERTY(Waveform)
};

Q_DECLARE_METATYPE(QxrdSynchronizedOutputChannel*)

#endif // QXRDSYNCHRONIZEDOUTPUTCHANNEL_H
