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
#ifndef QT_NO_DEBUG
  printf("Shutdown acquisition execution thread\n");
#endif

  shutdown();

#ifndef QT_NO_DEBUG
  printf("Deleting acquisition execution thread\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisitionExecutionThread::~QxrdAcquisitionExecutionThread(%p)\n", this);
  }
}

void QxrdAcquisitionExecutionThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printf("Acquisition Execution Thread Started\n");
  }

  {
    QxrdAcquisitionExecutionPtr acqexec =
        QxrdAcquisitionExecutionPtr(
          NEWPTR(QxrdAcquisitionExecution("acquisitionExecution")));

    if (acqexec) {
      acqexec -> initialize(sharedFromThis());
    }

    m_AcquisitionExecution = acqexec;
  }

  int rc = exec();

  m_AcquisitionExecution = QxrdAcquisitionExecutionPtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Acquisition Execution Thread terminated with rc %d\n", rc);
  }
}

void QxrdAcquisitionExecutionThread::haltAcquisitionExecution()
{
  if (m_AcquisitionExecution) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    INVOKE_CHECK(
          QMetaObject::invokeMethod(m_AcquisitionExecution.data(),
                                    &QxrdAcquisitionExecution::haltAcquisitionExecution,
                                    Qt::BlockingQueuedConnection))
#else
    INVOKE_CHECK(
          QMetaObject::invokeMethod(m_AcquisitionExecution.data(),
                                    "haltAcquisitionExecution",
                                    Qt::BlockingQueuedConnection))
#endif
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
