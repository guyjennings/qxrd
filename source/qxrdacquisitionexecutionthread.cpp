#include "qxrddebug.h"
#include "qxrdacquisitionexecutionthread.h"
#include "qxrdacquisitioninterface.h"
#include "qxrdacquisitionexecution.h"
#include <stdio.h>

QxrdAcquisitionExecutionThread::QxrdAcquisitionExecutionThread(QxrdAcquisitionInterfaceWPtr acq)
  : QxrdThread(),
    m_Acquisition(acq)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisitionThread::QxrdAcquisitionThread(%p)\n", this);
  }
}

QxrdAcquisitionExecutionThread::~QxrdAcquisitionExecutionThread()
{
  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisitionThread::~QxrdAcquisitionThread(%p)\n", this);
  }
}

void QxrdAcquisitionExecutionThread::shutdown()
{
  exit();

  wait();
}

void QxrdAcquisitionExecutionThread::run()
{
  m_AcquisitionExecution =
      QxrdAcquisitionExecutionPtr(new QxrdAcquisitionExecution(m_Acquisition));

  int rc = exec();

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
