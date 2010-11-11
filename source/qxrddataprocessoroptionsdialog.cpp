#include "qxrddataprocessoroptionsdialog.h"
#include "ui_qxrddataprocessoroptionsdialog.h"

QxrdDataProcessorOptionsDialog::QxrdDataProcessorOptionsDialog(QxrdDataProcessor *processor, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QxrdDataProcessorOptionsDialog)
{
    ui->setupUi(this);
}

QxrdDataProcessorOptionsDialog::~QxrdDataProcessorOptionsDialog()
{
    delete ui;
}

void QxrdDataProcessorOptionsDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
