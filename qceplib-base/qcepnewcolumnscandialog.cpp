#include "qcepnewcolumnscandialog.h"
#include "ui_qcepnewcolumnscandialog.h"
#include "qcepdatasetmodel.h"

static QString     s_ScanName;
static QStringList s_ColumnNames;
static int         s_ScanSize;

QcepNewColumnScanDialog::QcepNewColumnScanDialog(QcepDatasetModelPtr model, const QModelIndex &idx) :
  QDialog(),
  m_Model(model),
  m_Index(idx)
{
  setupUi(this);

  if (m_Model) {
    setWindowTitle(tr("Create new column scan in %1").arg(m_Model->pathName(idx)));
  }

  m_ScanName -> setText(s_ScanName);
  m_ScanSize -> setValue(s_ScanSize);

  foreach (QString col, s_ColumnNames) {
    m_ColumnNames->addItem(col);
  }

  connect(m_AddColumn, &QAbstractButton::clicked, this, &QcepNewColumnScanDialog::addColumn);
  connect(m_DelColumn, &QAbstractButton::clicked, this, &QcepNewColumnScanDialog::delColumn);
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

  s_ScanName = m_ScanName->text();
  s_ScanSize = m_ScanSize->value();

  QStringList res;

  for (int i=0; i<m_ColumnNames->count(); i++) {
    QListWidgetItem *item = m_ColumnNames->item(i);

    if (item) {
      res.append(item->text());
    }
  }

  s_ColumnNames = res;

  if (m_Model) {
    m_Model -> newColumnScan(m_Index, s_ScanName, s_ScanSize, s_ColumnNames);
  }

  QDialog::accept();
}
