#include "qcepdatasetselectiondialog.h"
#include "ui_qcepdatasetselectiondialog.h"
#include "qcepdatasetmodel.h"

int    QcepDatasetSelectionDialog::m_ConstantSelected = false;
double QcepDatasetSelectionDialog::m_ConstantValue = 0;

QcepDatasetSelectionDialog::QcepDatasetSelectionDialog(QString title, QcepDatasetModelPtr ds, const QModelIndex &idx) :
  inherited(NULL),
  m_Title(title),
  m_DatasetModel(ds),
  m_Destination(ds->indexedObject(idx))
{
  setupUi(this);

  m_ItemsSelectionWidget->setModel(m_DatasetModel.data());

  m_ConstantValueWidget->setValue(m_ConstantValue);

  if (m_ConstantSelected) {
    m_ConstantSelectedButton->setChecked(true);
  } else {
    m_ItemsSelectedButton->setChecked(true);
  }

  if (m_Destination) {
    QString s = "Select data to " + m_Title;

    setWindowTitle(s.arg(m_Destination->pathName()));
  }
}

QcepDatasetSelectionDialog::~QcepDatasetSelectionDialog()
{
}

void QcepDatasetSelectionDialog::accept()
{
  m_ConstantSelected = m_ConstantSelectedButton->isChecked();
  m_ConstantValue    = m_ConstantValueWidget->value();
  m_SelectedData     = m_ItemsSelectionWidget->selectionModel()->selectedRows();

  QDialog::accept();
}

bool QcepDatasetSelectionDialog::constantChosen()
{
  return m_ConstantSelected;
}

double QcepDatasetSelectionDialog::constantValue()
{
  return m_ConstantValue;
}

QModelIndexList QcepDatasetSelectionDialog::selectedIndexes()
{
  return m_SelectedData;
}
