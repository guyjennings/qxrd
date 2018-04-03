#include "qxrdprocessorexecution.h"
#include "qxrddebug.h"
#include "qxrdprocessor.h"
#include "qxrdprocessorexecutionthread.h"

QxrdProcessorExecution::QxrdProcessorExecution(QString name)
  : inherited(name),
    m_Processing(this, "processing", 0, "Is processing?"),
    m_Processor()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdProcessorExecution::QxrdProcessorExecution(%p)\n", this);
  }
}

void QxrdProcessorExecution::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_Processor = QxrdProcessor::findProcessor(parent);
}

QxrdProcessorExecution::~QxrdProcessorExecution()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdProcessorExecution::~QxrdProcessorExecution(%p)\n", this);
  }
}

void QxrdProcessorExecution::registerMetaTypes()
{
  qRegisterMetaType<QxrdProcessorExecution*>("QxrdProcessorExecution*");
  qRegisterMetaType<QxrdProcessorExecutionThread*>("QxrdProcessorExecutionThread*");
}
