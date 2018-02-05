#include "qxrdprocessorstep.h"
#include "qxrdprocessor.h"

QxrdProcessorStep::QxrdProcessorStep(QString name) :
  QcepObject(name)
{
}

QxrdProcessorStep::~QxrdProcessorStep()
{
}

void QxrdProcessorStep::initialize(QObjectWPtr p)
{
  inherited::initialize(p);

  m_Processor = QxrdProcessor::findProcessor(p);
}
