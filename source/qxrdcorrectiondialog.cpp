#include "qxrdcorrectiondialog.h"
#include "ui_qxrdcorrectiondialog.h"

QxrdCorrectionDialog::QxrdCorrectionDialog(QWidget *parent, QxrdAcquisition *acq, QxrdDataProcessor *proc) :
    QDockWidget(parent)
{
    setupUi(this);
}

QxrdCorrectionDialog::~QxrdCorrectionDialog()
{
}

void QxrdCorrectionDialog::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
