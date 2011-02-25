#include "qxrdsynchronizedacquisitiondialog.h"
#include "ui_qxrdsynchronizedacquisitiondialog.h"

QxrdSynchronizedAcquisitionDialog::QxrdSynchronizedAcquisitionDialog(QWidget *parent, QxrdAcquisition *acq) :
    QDockWidget(parent)
{
    setupUi(this);
}

QxrdSynchronizedAcquisitionDialog::~QxrdSynchronizedAcquisitionDialog()
{
}

void QxrdSynchronizedAcquisitionDialog::changeEvent(QEvent *e)
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
