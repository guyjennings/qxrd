#include "qxrdcorrectiondialog.h"
#include "ui_qxrdcorrectiondialog.h"

QxrdCorrectionDialog::QxrdCorrectionDialog(QWidget *parent, QxrdAcquisitionWPtr acq, QxrdDataProcessorWPtr proc) :
    QDockWidget(parent)
{
    setupUi(this);
}

QxrdCorrectionDialog::~QxrdCorrectionDialog()
{
}

void QxrdCorrectionDialog::changeEvent(QEvent *e)
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
