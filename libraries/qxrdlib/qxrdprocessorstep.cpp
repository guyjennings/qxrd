#include "qxrdprocessorstep.h"

QxrdProcessorStep::QxrdProcessorStep(QString name) :
  QcepObject(name)
{
}

QxrdProcessorStep::~QxrdProcessorStep()
{
}

void QxrdProcessorStep::initialize(QxrdProcessorWPtr proc)
{
  m_Processor = proc;
}
