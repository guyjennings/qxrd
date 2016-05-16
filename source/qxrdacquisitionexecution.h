#ifndef QXRDACQUISITIONEXECUTION_H
#define QXRDACQUISITIONEXECUTION_H

#include "qcepobject.h"
#include "qxrdacquisitioninterface-ptr.h"

class QxrdAcquisitionExecution : public QcepObject
{
  Q_OBJECT

public:
  explicit QxrdAcquisitionExecution(QxrdAcquisitionInterfaceWPtr acq);
  virtual ~QxrdAcquisitionExecution();

  void setAcquisition(QxrdAcquisitionInterfaceWPtr acq);

signals:

public slots:
  void doAcquire();
  void doAcquireDark();

private:
  QxrdAcquisitionInterfaceWPtr m_Acquisition;
};

#endif // QXRDACQUISITIONEXECUTION_H
