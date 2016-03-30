#include "qcepnewdatacolumndialog.h"
#include "ui_qcepnewdatacolumndialog.h"
#include "qcepdatasetmodel.h"

QcepNewDataColumnDialog::QcepNewDataColumnDialog(QcepDatasetModelPtr model, const QModelIndex &idx) :
  QDialog(),
  m_Model(model),
  m_Index(idx)
{
  setupUi(this);

  if (m_Model) {
    setWindowTitle(tr("Create new data column in %1").arg(m_Model->pathName(idx)));
  }
}

QcepNewDataColumnDialog::~QcepNewDataColumnDialog()
{
}

void QcepNewDataColumnDialog::accept()
{

  QDialog::accept();
}
