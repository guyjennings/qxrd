#include "qcepdataexportdialog.h"

QcepDataExportDialog::QcepDataExportDialog(QcepDatasetModelPtr model, const QModelIndexList &idx) :
  QDialog(),
  m_Model(model),
  m_Indexes(idx)
{
  setupUi(this);
}

QcepDataExportDialog::~QcepDataExportDialog()
{
}

void QcepDataExportDialog::accept()
{
  QDialog::accept();
}
