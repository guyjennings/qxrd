#include "qcepnewdataarraydialog.h"
#include "ui_qcepnewdataarraydialog.h"
#include "qcepdatasetmodel.h"

QcepNewDataArrayDialog::QcepNewDataArrayDialog(QcepDatasetModelPtr model, const QModelIndex &idx) :
  QDialog(),
  m_Model(model),
  m_Index(idx)
{
  setupUi(this);

  if (m_Model) {
    setWindowTitle(tr("Create new data array in %1").arg(m_Model->pathName(idx)));
  }
}

QcepNewDataArrayDialog::~QcepNewDataArrayDialog()
{
}

void QcepNewDataArrayDialog::accept()
{

  QDialog::accept();
}
