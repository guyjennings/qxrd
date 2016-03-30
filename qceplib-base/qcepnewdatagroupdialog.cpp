#include "qcepnewdatagroupdialog.h"
#include "ui_qcepnewdatagroupdialog.h"
#include "qcepdatasetmodel.h"

static QString s_GroupName;

QcepNewDataGroupDialog::QcepNewDataGroupDialog(QcepDatasetModelPtr model, const QModelIndex &idx) :
  QDialog(),
  m_Model(model),
  m_Index(idx)
{
  setupUi(this);

  if (m_Model) {
    setWindowTitle(tr("Create new data group in %1").arg(m_Model->pathName(idx)));
  }

  m_GroupName -> setText(s_GroupName);
}

QcepNewDataGroupDialog::~QcepNewDataGroupDialog()
{
}

void QcepNewDataGroupDialog::accept()
{
  s_GroupName = m_GroupName -> text();

  if (m_Model) {
    m_Model -> newGroup(m_Index, s_GroupName);
  }

  QDialog::accept();
}
