#include "qxrdcorrectiondialog.h"
#include "ui_qxrdcorrectiondialog.h"
#include "qxrddebug.h"

QxrdCorrectionDialog::QxrdCorrectionDialog(QWidget *parent, QxrdAcquisitionWPtr acq, QxrdDataProcessorWPtr proc) :
    QDockWidget(parent)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdCorrectionDialog::QxrdCorrectionDialog(%p)\n", this);
  }

  setupUi(this);
}

QxrdCorrectionDialog::~QxrdCorrectionDialog()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdCorrectionDialog::~QxrdCorrectionDialog(%p)\n", this);
  }
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
