#ifndef QXRDACQUISITIONEXECUTION_H
#define QXRDACQUISITIONEXECUTION_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qxrdacquisition-ptr.h"

class QXRD_EXPORT QxrdAcquisitionExecution : public QcepObject
{
  Q_OBJECT

public:
  explicit QxrdAcquisitionExecution(QxrdAcquisitionWPtr acq);
  virtual ~QxrdAcquisitionExecution();

  void setAcquisition(QxrdAcquisitionWPtr acq);

signals:

public slots:
  void doAcquire();
  void doAcquireOnce();
  void doAcquireDark();

private:
  QxrdAcquisitionPtr m_Acquisition;
};

#endif // QXRDACQUISITIONEXECUTION_H
