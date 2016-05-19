#include "qxrddebug.h"
#include "qxrdacquisitionexecutionthread.h"
#include "qxrdacquisitioninterface.h"
#include "qxrdacquisitionexecution.h"
#include <stdio.h>
#include "qcepmutexlocker.h"

QxrdAcquisitionExecutionThread::QxrdAcquisitionExecutionThread(QxrdAcquisitionInterfaceWPtr acq)
  : QxrdThread(acq),
    m_Acquisition(acq)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisitionExecutionThread::QxrdAcquisitionExecutionThread(%p)\n", this);
  }

  setObjectName("acqExec");
}

QxrdAcquisitionExecutionThread::~QxrdAcquisitionExecutionThread()
{
  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisitionExecutionThread::~QxrdAcquisitionExecutionThread(%p)\n", this);
  }
}

void QxrdAcquisitionExecutionThread::shutdown()
{
  exit();

  wait();
}

void QxrdAcquisitionExecutionThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printf("Acquisition Execution Thread Started\n");
  }

  m_AcquisitionExecution =
      QxrdAcquisitionExecutionPtr(new QxrdAcquisitionExecution(m_Acquisition));

  int rc = exec();

  m_AcquisitionExecution = QxrdAcquisitionExecutionPtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Acquisition Execution Thread terminated with rc %d\n", rc);
  }
}

void QxrdAcquisitionExecutionThread::doAcquire()
{
  if (m_AcquisitionExecution) {
    QMetaObject::invokeMethod(m_AcquisitionExecution.data(), "doAcquire");
  }
}

void QxrdAcquisitionExecutionThread::doAcquireDark()
{
  if (m_AcquisitionExecution) {
    QMetaObject::invokeMethod(m_AcquisitionExecution.data(), "doAcquireDark");
  }
}
