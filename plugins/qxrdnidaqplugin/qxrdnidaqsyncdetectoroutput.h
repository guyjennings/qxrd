#ifndef QXRDNIDAQSYNCDETECTOROUTPUT_H
#define QXRDNIDAQSYNCDETECTOROUTPUT_H

#include "qcepobject.h"
#include "qcepproperty.h"

class QxrdNIDAQSyncDetectorOutput : public QcepObject
{
  Q_OBJECT

public:
  QxrdNIDAQSyncDetectorOutput(int relExposure, double d0, double d1);

private:
  typedef QcepObject inherited;

public:
  Q_PROPERTY(int relExposure READ get_RelExposure WRITE set_RelExposure STORED false)
  QCEP_INTEGER_PROPERTY(RelExposure)

  Q_PROPERTY(double initialDelay READ get_InitialDelay WRITE set_InitialDelay STORED false)
  QCEP_DOUBLE_PROPERTY(InitialDelay)

  Q_PROPERTY(double readoutDelay READ get_ReadoutDelay WRITE set_ReadoutDelay STORED false)
  QCEP_DOUBLE_PROPERTY(ReadoutDelay)
};

#endif // QXRDNIDAQSYNCDETECTOROUTPUT_H
