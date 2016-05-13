#ifndef QXRDACQUISITIONEXECUTION_H
#define QXRDACQUISITIONEXECUTION_H

#include "qcepobject.h"
#include "qxrdacquisitioninterface-ptr.h"

class QxrdAcquisitionExecution : public QcepObject
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdAcquisitionExecution(QxrdAcquisitionInterfaceWPtr acq);
  virtual ~QxrdAcquisitionExecution();

signals:

public slots:
  void doAcquire();
  void doAcquireDark();

private:
  QxrdAcquisitionInterfaceWPtr m_Acquisition;
};

Q_DECLARE_METATYPE(QxrdAcquisitionExecution*)

#endif // QXRDACQUISITIONEXECUTION_H
