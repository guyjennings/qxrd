#ifndef QXRDACQUISITIONEXECUTIONTHREAD_H
#define QXRDACQUISITIONEXECUTIONTHREAD_H

#include "qxrdlib_global.h"
#include "qxrdthread.h"
#include "qxrdacquisitioninterface-ptr.h"
#include "qxrdacquisitionexecution-ptr.h"

class QXRD_EXPORT QxrdAcquisitionExecutionThread : public QxrdThread
{
public:
  QxrdAcquisitionExecutionThread(QxrdAcquisitionInterfaceWPtr acq);
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
  QxrdAcquisitionInterfaceWPtr m_Acquisition;
  QxrdAcquisitionExecutionPtr  m_AcquisitionExecution;
};

#endif // QXRDACQUISITIONEXECUTIONTHREAD_H
