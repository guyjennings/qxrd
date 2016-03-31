#include "qcepnewdatagroupdialog.h"
#include "ui_qcepnewdatagroupdialog.h"
#include "qcepdatasetmodel.h"
#include "qcepexperiment.h"

QcepNewDataGroupDialog::QcepNewDataGroupDialog(QcepDatasetModelPtr model, const QModelIndex &idx) :
  QDialog(),
  m_Model(model),
  m_Index(idx)
{
  setupUi(this);

  if (m_Model) {
    setWindowTitle(tr("Create new data group in %1").arg(m_Model->pathName(idx)));

    QcepExperimentPtr expt = m_Model -> experiment();

    if (expt) {
      m_GroupName -> setText(expt -> get_NewGroupName());
    }
  }
}

QcepNewDataGroupDialog::~QcepNewDataGroupDialog()
{
}

void QcepNewDataGroupDialog::accept()
{
  if (m_Model) {
    QcepExperimentPtr expt = m_Model -> experiment();

    if (expt) {
      QString newGroupName = m_GroupName -> text();

      expt -> set_NewGroupName(newGroupName);

      m_Model -> newGroup(m_Index, newGroupName);
    }
  }

  QDialog::accept();
}
