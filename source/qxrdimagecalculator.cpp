#include "qxrdimagecalculator.h"
#include "ui_qxrdimagecalculator.h"

QxrdImageCalculator::QxrdImageCalculator(QxrdDataProcessorPtr processor, QWidget *parent) :
    QDockWidget(parent),
    m_ui(new Ui::QxrdImageCalculator),
    m_Processor(processor),
    SOURCE_IDENT("$id$")
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

/******************************************************************
*
*  $Log: qxrdimagecalculator.cpp,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:39  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.2  2010/06/07 20:17:42  jennings
*  Added more cvs log and ident stuff - removed qxrdspecserver.cpp & qxrdspecserver.h
*
*
*
*******************************************************************/
