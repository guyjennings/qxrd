#ifndef QXRDNIDAQSYNCANALOGINPUT_H
#define QXRDNIDAQSYNCANALOGINPUT_H

#include "qcepobject.h"
#include "qcepproperty.h"

class QxrdNIDAQSyncAnalogInput : public QcepObject
{
  Q_OBJECT

public:
  QxrdNIDAQSyncAnalogInput();

private:
  typedef QcepObject inherited;
};

#endif // QXRDNIDAQSYNCANALOGINPUT_H
