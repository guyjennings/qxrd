#ifndef QXRDSYNCHRONIZEDOUTPUTCHANNEL_H
#define QXRDSYNCHRONIZEDOUTPUTCHANNEL_H

#include "qxrdlib_global.h"
#include "qcepobject.h"

class QXRD_EXPORT QxrdSynchronizedOutputChannel : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  Q_INVOKABLE QxrdSynchronizedOutputChannel(QString name);

  QString channelName();
  QString channelMode();

public:
  Q_PROPERTY(int channelNumber READ get_ChannelNumber WRITE set_ChannelNumber)
  QCEP_INTEGER_PROPERTY(ChannelNumber)

  Q_PROPERTY(QString signalName READ get_SignalName WRITE set_SignalName)
  QCEP_STRING_PROPERTY(SignalName)

  Q_PROPERTY(int mode READ get_Mode WRITE set_Mode)
  QCEP_INTEGER_PROPERTY(Mode)

  Q_PROPERTY(double startV READ get_StartV WRITE set_StartV)
  QCEP_DOUBLE_PROPERTY(StartV)

  Q_PROPERTY(double endV READ get_EndV WRITE set_EndV)
  QCEP_DOUBLE_PROPERTY(EndV)

  Q_PROPERTY(QcepDoubleVector waveform READ get_Waveform WRITE set_Waveform STORED false)
  QCEP_DOUBLE_VECTOR_PROPERTY(Waveform)
};

Q_DECLARE_METATYPE(QxrdSynchronizedOutputChannel*)

#endif // QXRDSYNCHRONIZEDOUTPUTCHANNEL_H
