#include "qxrdacquisitionextrainputsdialog.h"
#include "ui_qxrdacquisitionextrainputsdialog.h"

QxrdAcquisitionExtraInputsDialog::QxrdAcquisitionExtraInputsDialog(QWidget *parent, QxrdAcquisitionWPtr acq) :
  QDockWidget(parent)
{
  setupUi(this);
}

QxrdAcquisitionExtraInputsDialog::~QxrdAcquisitionExtraInputsDialog()
{
}
