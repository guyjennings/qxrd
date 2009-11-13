#include "qxrdimagecalculator.h"
#include "ui_qxrdimagecalculator.h"

QxrdImageCalculator::QxrdImageCalculator(QxrdDataProcessor *processor, QWidget *parent) :
    QDockWidget(parent),
    m_ui(new Ui::QxrdImageCalculator),
    m_Processor(processor)
{
    m_ui->setupUi(this);
}

QxrdImageCalculator::~QxrdImageCalculator()
{
    delete m_ui;
}

void QxrdImageCalculator::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
