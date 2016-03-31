#include "qcepdataimportdialog.h"

QcepDataImportDialog::QcepDataImportDialog(QcepDatasetModelPtr model, const QModelIndexList &idx) :
  QDialog(),
  m_Model(model),
  m_Indexes(idx)
{
  setupUi(this);
}

QcepDataImportDialog::~QcepDataImportDialog()
{
}

void QcepDataImportDialog::accept()
{
  QDialog::accept();
}
