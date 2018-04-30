#ifndef QXRDACQUISITIONEXECUTIONTHREAD_H
#define QXRDACQUISITIONEXECUTIONTHREAD_H

#include "qxrdlib_global.h"
#include "qcepthread.h"
#include "qxrdacqcommon-ptr.h"
#include "qxrdacquisitionexecution-ptr.h"

class QXRD_EXPORT QxrdAcquisitionExecutionThread : public QcepThread
{
  Q_OBJECT

private:
  typedef QcepThread inherited;

public:
  Q_INVOKABLE QxrdAcquisitionExecutionThread(QString name);
  ~QxrdAcquisitionExecutionThread();

  void initialize(QcepObjectWPtr parent);
  void haltAcquisitionExecution();

signals:

public slots:
  void doAcquire();
  void doAcquireOnce();
  void doAcquireDark();
  void doAcquireIdle();

protected:
  void run();

private:
  QxrdAcqCommonWPtr            m_Acquisition;
  QxrdAcquisitionExecutionPtr  m_AcquisitionExecution;
};

Q_DECLARE_METATYPE(QxrdAcquisitionExecutionThread*)

#endif // QXRDACQUISITIONEXECUTIONTHREAD_H
