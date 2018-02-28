#ifndef QXRDNIDAQSYNCWAVEFORMOUTPUT_H
#define QXRDNIDAQSYNCWAVEFORMOUTPUT_H

#include "qcepobject.h"
#include "qcepproperty.h"

class QxrdNIDAQSyncWaveformOutput : public QcepObject
{
  Q_OBJECT

public:
  QxrdNIDAQSyncWaveformOutput();

private:
  typedef QcepObject inherited;
};

#endif // QXRDNIDAQSYNCWAVEFORMOUTPUT_H
