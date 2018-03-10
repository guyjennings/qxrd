#include "qxrddebug.h"
#include "qxrdacquisitionexecutionthread.h"
#include "qxrdacquisition.h"
#include "qxrdacquisitionexecution.h"
#include <stdio.h>
#include "qcepmutexlocker.h"

QxrdAcquisitionExecutionThread::QxrdAcquisitionExecutionThread(QString name)
  : inherited(name),
    m_Acquisition()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisitionExecutionThread::QxrdAcquisitionExecutionThread(%p)\n", this);
  }
}

void QxrdAcquisitionExecutionThread::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_Acquisition = QxrdAcquisition::findAcquisition(parent);
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
      QxrdAcquisitionExecutionPtr(
        new QxrdAcquisitionExecution("acquisitionExecution"));

  m_AcquisitionExecution -> initialize(sharedFromThis());

  int rc = exec();

  m_AcquisitionExecution = QxrdAcquisitionExecutionPtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Acquisition Execution Thread terminated with rc %d\n", rc);
  }
}

void QxrdAcquisitionExecutionThread::doAcquire()
{
  if (m_AcquisitionExecution) {
    INVOKE_CHECK(
          QMetaObject::invokeMethod(m_AcquisitionExecution.data(), "doAcquire"));
  }
}

void QxrdAcquisitionExecutionThread::doAcquireOnce()
{
  if (m_AcquisitionExecution) {
    INVOKE_CHECK(
          QMetaObject::invokeMethod(m_AcquisitionExecution.data(), "doAcquireOnce"));
  }
}

void QxrdAcquisitionExecutionThread::doAcquireDark()
{
  if (m_AcquisitionExecution) {
    INVOKE_CHECK(
          QMetaObject::invokeMethod(m_AcquisitionExecution.data(), "doAcquireDark"));
  }
}

void QxrdAcquisitionExecutionThread::doAcquireIdle()
{
  if (m_AcquisitionExecution) {
    INVOKE_CHECK(
          QMetaObject::invokeMethod(m_AcquisitionExecution.data(), "doAcquireIdle"));
  }
}
