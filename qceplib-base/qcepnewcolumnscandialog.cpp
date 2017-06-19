#include "qcepnewcolumnscandialog.h"
#include "ui_qcepnewcolumnscandialog.h"
#include "qcepdatasetmodel.h"
#include "qcepexperiment.h"

QcepNewColumnScanDialog::QcepNewColumnScanDialog(QcepDatasetModelPtr model, const QModelIndex &idx) :
  QDialog(),
  m_Model(model),
  m_Index(idx)
{
  setupUi(this);

  if (m_Model) {
    setWindowTitle(tr("Create new column scan in %1").arg(m_Model->pathName(idx)));

    QcepExperimentPtr expt = m_Model -> experiment();

    if (expt) {
      m_ScanName -> setText(expt->get_NewScanName());
      m_ScanSize -> setValue(expt->get_NewScanSize());

      QStringList cols = expt->get_NewScanColumns();

      foreach (QString col, cols) {
        m_ColumnNames->addItem(col);
      }
    }

    connect(m_AddColumn, &QAbstractButton::clicked, this, &QcepNewColumnScanDialog::addColumn);
    connect(m_DelColumn, &QAbstractButton::clicked, this, &QcepNewColumnScanDialog::delColumn);
  }
}

QcepNewColumnScanDialog::~QcepNewColumnScanDialog()
{
}

void QcepNewColumnScanDialog::addColumn()
{
  int row = m_ColumnNames->currentRow();

  m_ColumnNames->insertItem(row, "Col");
}

void QcepNewColumnScanDialog::delColumn()
{
  int row = m_ColumnNames->currentRow();

  QListWidgetItem *item = m_ColumnNames->takeItem(row);

  delete item;
}

void QcepNewColumnScanDialog::accept()
{
  if (m_Model) {
    QcepExperimentPtr expt = m_Model -> experiment();

    if (expt) {
      QString     newScanName = m_ScanName->text();
      int         newScanSize = m_ScanSize->value();
      QStringList newScanColumns;

      for (int i=0; i<m_ColumnNames->count(); i++) {
        QListWidgetItem *item = m_ColumnNames->item(i);

        if (item) {
          newScanColumns.append(item->text());
        }
      }

      expt -> set_NewScanName(newScanName);
      expt -> set_NewScanSize(newScanSize);
      expt -> set_NewScanColumns(newScanColumns);

      m_Model -> newColumnScan(m_Index, newScanName, newScanColumns, newScanSize);
    }
  }

  QDialog::accept();
}
