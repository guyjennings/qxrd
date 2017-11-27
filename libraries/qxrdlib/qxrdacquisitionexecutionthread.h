#ifndef QXRDACQUISITIONEXECUTIONTHREAD_H
#define QXRDACQUISITIONEXECUTIONTHREAD_H

#include "qxrdlib_global.h"
#include "qxrdthread.h"
#include "qxrdacquisition-ptr.h"
#include "qxrdacquisitionexecution-ptr.h"

class QXRD_EXPORT QxrdAcquisitionExecutionThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdAcquisitionExecutionThread(QxrdAcquisitionWPtr acq);
  ~QxrdAcquisitionExecutionThread();

  void shutdown();

signals:

public slots:
  void doAcquire();
  void doAcquireOnce();
  void doAcquireDark();

protected:
  void run();

private:
  QxrdAcquisitionWPtr          m_Acquisition;
  QxrdAcquisitionExecutionPtr  m_AcquisitionExecution;
};

#endif // QXRDACQUISITIONEXECUTIONTHREAD_H
