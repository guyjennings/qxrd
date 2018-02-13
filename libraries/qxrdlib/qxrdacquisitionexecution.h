#ifndef QXRDACQUISITIONEXECUTION_H
#define QXRDACQUISITIONEXECUTION_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qxrdacqcommon-ptr.h"
#include "qtimer-ptr.h"

class QXRD_EXPORT QxrdAcquisitionExecution : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  explicit QxrdAcquisitionExecution(QxrdAcqCommonWPtr acq);
  virtual ~QxrdAcquisitionExecution();
  void initialize(QcepObjectWPtr parent);
  void setAcquisition(QxrdAcqCommonWPtr acq);

signals:

public slots:
  void doAcquire();
  void doAcquireOnce();
  void doAcquireDark();
  void doAcquireIdle();

private:
  void stopIdling();
  void startIdling();

public:
  Q_PROPERTY(int acquiring READ get_Acquiring WRITE set_Acquiring STORED false)
  QCEP_INTEGER_PROPERTY(Acquiring)

private:
  QxrdAcqCommonWPtr m_Acquisition;
  QTimerPtr         m_IdleTimer;
};

#endif // QXRDACQUISITIONEXECUTION_H
