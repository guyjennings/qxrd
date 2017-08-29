#ifndef QXRDACQUISITIONEXECUTION_H
#define QXRDACQUISITIONEXECUTION_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qxrdacquisitioninterface-ptr.h"

class QXRD_EXPORT QxrdAcquisitionExecution : public QcepObject
{
  Q_OBJECT

public:
  explicit QxrdAcquisitionExecution(QxrdAcquisitionInterfaceWPtr acq);
  virtual ~QxrdAcquisitionExecution();

  void setAcquisition(QxrdAcquisitionInterfaceWPtr acq);

signals:

public slots:
  void doAcquire();
  void doAcquireOnce();
  void doAcquireDark();

private:
  QxrdAcquisitionInterfaceWPtr m_Acquisition;
};

#endif // QXRDACQUISITIONEXECUTION_H
