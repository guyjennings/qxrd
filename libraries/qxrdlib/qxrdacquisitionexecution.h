#ifndef QXRDACQUISITIONEXECUTION_H
#define QXRDACQUISITIONEXECUTION_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qxrdacqcommon-ptr.h"

class QXRD_EXPORT QxrdAcquisitionExecution : public QcepObject
{
  Q_OBJECT

public:
  explicit QxrdAcquisitionExecution(QxrdAcqCommonWPtr acq);
  virtual ~QxrdAcquisitionExecution();

  void setAcquisition(QxrdAcqCommonWPtr acq);

signals:

public slots:
  void doAcquire();
  void doAcquireOnce();
  void doAcquireDark();

private:
  QxrdAcqCommonWPtr m_Acquisition;
};

#endif // QXRDACQUISITIONEXECUTION_H
