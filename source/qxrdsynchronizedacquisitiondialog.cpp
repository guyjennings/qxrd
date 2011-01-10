#include "qxrdsynchronizedacquisitiondialog.h"
#include "ui_qxrdsynchronizedacquisitiondialog.h"

QxrdSynchronizedAcquisitionDialog::QxrdSynchronizedAcquisitionDialog(QWidget *parent, QxrdAcquisition *acq) :
    QDockWidget(parent),
    ui(new Ui::QxrdSynchronizedAcquisitionDialog)
{
    ui->setupUi(this);
}

QxrdSynchronizedAcquisitionDialog::~QxrdSynchronizedAcquisitionDialog()
{
    delete ui;
}

void QxrdSynchronizedAcquisitionDialog::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
