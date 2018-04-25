#include "qxrddebug.h"
#include "qxrdprocessorexecutionthread.h"
#include "qxrdprocessor.h"
#include "qxrdprocessorexecution.h"

QxrdProcessorExecutionThread::QxrdProcessorExecutionThread(QString name)
  : inherited(name),
    m_Processor(),
    m_ProcessorExecution()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdProcessorExecutionThread::QxrdProcessorExecutionThread(%p)\n", this);
  }
}

void QxrdProcessorExecutionThread::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_Processor = QxrdProcessor::findProcessor(parent);
}

QxrdProcessorExecutionThread::~QxrdProcessorExecutionThread()
{
//  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdProcessorExecutionThread::~QxrdProcessorExecutionThread(%p)\n", this);
  }
}

void QxrdProcessorExecutionThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printMessage("Processor Execution Thread Started");
  }

  m_ProcessorExecution =
      QxrdProcessorExecutionPtr(
        NEWPTR(QxrdProcessorExecution("processorExecution")));

  m_ProcessorExecution -> initialize(sharedFromThis());

  int rc = exec();

  m_ProcessorExecution = QxrdProcessorExecutionPtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printMessage(tr("Processor Execution Thread terminated with rc %1").arg(rc));
  }
}

void QxrdProcessorExecutionThread::processAcquiredImage(QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow)
{
  QxrdProcessorExecution* exec = m_ProcessorExecution.data();

#if QT_VERSION >= QT_VERSION_CHECK(5,10,0)
  INVOKE_CHECK(
        QMetaObject::invokeMethod(exec,
                                  [=]() { exec->processAcquiredImage(image, overflow);}))
#else
  INVOKE_CHECK(
        QMetaObject::invokeMethod(exec,
                                  "processAcquiredImage",
                                  Q_ARG(QcepUInt32ImageDataPtr, image),
                                  Q_ARG(QcepMaskDataPtr, overflow)
                                  ))
#endif
}

void QxrdProcessorExecutionThread::processDarkImage(QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow)
{
  QxrdProcessorExecution* exec = m_ProcessorExecution.data();

#if QT_VERSION >= QT_VERSION_CHECK(5,10,0)
  INVOKE_CHECK(
        QMetaObject::invokeMethod(exec,
                                  [=]() { exec->processDarkImage(image, overflow);}))
#else
  INVOKE_CHECK(
        QMetaObject::invokeMethod(exec,
                                  "processDarkImage",
                                  Q_ARG(QcepDoubleImageDataPtr, image),
                                  Q_ARG(QcepMaskDataPtr, overflow)
                                  ))
#endif
}

void QxrdProcessorExecutionThread::processIdleImage(QcepImageDataBasePtr image)
{
  QxrdProcessorExecution* exec = m_ProcessorExecution.data();

#if QT_VERSION >= QT_VERSION_CHECK(5,10,0)
  INVOKE_CHECK(
        QMetaObject::invokeMethod(exec,
                                  [=]() { exec->processIdleImage(image);}))
#else
  INVOKE_CHECK(
        QMetaObject::invokeMethod(exec,
                                  "processIdleImage",
                                  Q_ARG(QcepImageDataBasePtr, image)
                                  ))
#endif
}
