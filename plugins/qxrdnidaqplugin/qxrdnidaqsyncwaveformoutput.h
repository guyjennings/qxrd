#ifndef QXRDNIDAQSYNCWAVEFORMOUTPUT_H
#define QXRDNIDAQSYNCWAVEFORMOUTPUT_H

#include "qcepobject.h"
#include "qcepproperty.h"

class QxrdNIDAQSyncWaveformOutput : public QcepObject
{
  Q_OBJECT

public:
  QxrdNIDAQSyncWaveformOutput(int style, double v0, double v1);

private:
  typedef QcepObject inherited;

public:
  Q_PROPERTY(int waveformStyle READ get_WaveformStyle WRITE set_WaveformStyle STORED false)
  QCEP_INTEGER_PROPERTY(WaveformStyle)

  Q_PROPERTY(double waveformStartV READ get_WaveformStartV WRITE set_WaveformStartV STORED false)
  QCEP_DOUBLE_PROPERTY(WaveformStartV)

  Q_PROPERTY(double waveformEndV READ get_WaveformEndV WRITE set_WaveformEndV STORED false)
  QCEP_DOUBLE_PROPERTY(WaveformEndV)
};

Q_DECLARE_METATYPE(QxrdNIDAQSyncWaveformOutput*)

#endif // QXRDNIDAQSYNCWAVEFORMOUTPUT_H
