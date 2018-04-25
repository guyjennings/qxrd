#include "qxrddebug.h"
#include "qxrdacquisitionexecutionthread.h"
#include "qxrdacquisition.h"
#include "qxrdacquisitionexecution.h"
#include <stdio.h>
#include "qcepmutexlocker.h"

QxrdAcquisitionExecutionThread::QxrdAcquisitionExecutionThread(QString name)
  : inherited(name),
    m_Acquisition(),
    m_AcquisitionExecution()
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
//  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisitionExecutionThread::~QxrdAcquisitionExecutionThread(%p)\n", this);
  }
}

void QxrdAcquisitionExecutionThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printf("Acquisition Execution Thread Started\n");
  }

  m_AcquisitionExecution =
      QxrdAcquisitionExecutionPtr(
        NEWPTR(QxrdAcquisitionExecution("acquisitionExecution")));

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
#if QT_VERSION >= QT_VERSION_CHECK(5,10,0)
    INVOKE_CHECK(
          QMetaObject::invokeMethod(m_AcquisitionExecution.data(),
                                    &QxrdAcquisitionExecution::doAcquire,
                                    Qt::QueuedConnection));
#else
    INVOKE_CHECK(
          QMetaObject::invokeMethod(m_AcquisitionExecution.data(),
                                    "doAcquire",
                                    Qt::QueuedConnection));
#endif
  }
}

void QxrdAcquisitionExecutionThread::doAcquireOnce()
{
  if (m_AcquisitionExecution) {
#if QT_VERSION >= QT_VERSION_CHECK(5,10,0)
    INVOKE_CHECK(
          QMetaObject::invokeMethod(m_AcquisitionExecution.data(),
                                    &QxrdAcquisitionExecution::doAcquireOnce,
                                    Qt::QueuedConnection));
#else
    INVOKE_CHECK(
          QMetaObject::invokeMethod(m_AcquisitionExecution.data(),
                                    "doAcquireOnce",
                                    Qt::QueuedConnection));
#endif
  }
}

void QxrdAcquisitionExecutionThread::doAcquireDark()
{
  if (m_AcquisitionExecution) {
#if QT_VERSION >= QT_VERSION_CHECK(5,10,0)
    INVOKE_CHECK(
          QMetaObject::invokeMethod(m_AcquisitionExecution.data(),
                                    &QxrdAcquisitionExecution::doAcquireDark,
                                    Qt::QueuedConnection));
#else
    INVOKE_CHECK(
          QMetaObject::invokeMethod(m_AcquisitionExecution.data(),
                                    "doAcquireDark",
                                    Qt::QueuedConnection));
#endif
  }
}

void QxrdAcquisitionExecutionThread::doAcquireIdle()
{
  if (m_AcquisitionExecution) {
#if QT_VERSION >= QT_VERSION_CHECK(5,10,0)
    INVOKE_CHECK(
          QMetaObject::invokeMethod(m_AcquisitionExecution.data(),
                                    &QxrdAcquisitionExecution::doAcquireIdle,
                                    Qt::QueuedConnection));
#else
    INVOKE_CHECK(
          QMetaObject::invokeMethod(m_AcquisitionExecution.data(),
                                    "doAcquireIdle",
                                    Qt::QueuedConnection));
#endif
  }
}
