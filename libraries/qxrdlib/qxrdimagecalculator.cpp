#include "qxrdimagecalculator.h"
#include "ui_qxrdimagecalculator.h"

QxrdImageCalculator::QxrdImageCalculator(QxrdProcessorWPtr processor, QWidget *parent) :
    QDockWidget(parent),
    m_Processor(processor)
{
  setupUi(this);
}

QxrdImageCalculator::~QxrdImageCalculator()
{
}

void QxrdImageCalculator::changeEvent(QEvent *e)
{
  QDockWidget::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}
