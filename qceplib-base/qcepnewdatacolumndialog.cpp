#include "qcepnewdatacolumndialog.h"
#include "ui_qcepnewdatacolumndialog.h"
#include "qcepdatasetmodel.h"
#include "qcepexperiment.h"

QcepNewDataColumnDialog::QcepNewDataColumnDialog(QcepDatasetModelPtr model, const QModelIndex &idx) :
  QDialog(),
  m_Model(model),
  m_Index(idx)
{
  setupUi(this);

  if (m_Model) {
    setWindowTitle(tr("Create new data column in %1").arg(m_Model->pathName(idx)));
    QcepExperimentPtr expt = m_Model -> experiment();

    if (expt) {

      m_ColumnName -> setText(expt->get_NewColumnName());
      m_ColumnSize -> setValue(expt->get_NewColumnSize());
    }
  }
}

QcepNewDataColumnDialog::~QcepNewDataColumnDialog()
{
}

void QcepNewDataColumnDialog::accept()
{
  if (m_Model) {
    QcepExperimentPtr expt = m_Model -> experiment();

    if (expt) {
      QString newColumnName = m_ColumnName -> text();
      int     newColumnSize = m_ColumnSize -> value();

      expt -> set_NewColumnName(newColumnName);
      expt -> set_NewColumnSize(newColumnSize);

      m_Model -> newColumn(m_Index, newColumnName, newColumnSize);
    }
  }

  QDialog::accept();
}
