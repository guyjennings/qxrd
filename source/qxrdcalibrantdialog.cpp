#include "qxrdcalibrantdialog.h"
#include "ui_qxrdcalibrantdialog.h"
#include "qxrdcalibrantlibrarymodel.h"
#include "qxrdcalibrantdspacingsmodel.h"
#include "qxrdcalibrantlibrary.h"
#include "qxrdcalibrant.h"
#include "qxrdcenterfinder.h"
#include <QMenu>
#include <QAction>
#include <QClipboard>
#include "qxrdcalibrantpropertiesdialog.h"

QxrdCalibrantDialog::QxrdCalibrantDialog(QxrdCalibrantLibraryPtr cal, QxrdCenterFinderWPtr cf, QWidget *parent) :
  QDockWidget(parent),
  m_CalibrantLibrary(cal),
  m_CenterFinder(cf)
{
  setupUi(this);

  if (m_CalibrantLibrary) {
    m_CalibrantLibraryModel = QxrdCalibrantLibraryModelPtr(
          new QxrdCalibrantLibraryModel(m_CalibrantLibrary));

    m_CalibrantDSpacingsModel = QxrdCalibrantDSpacingsModelPtr(
          new QxrdCalibrantDSpacingsModel(m_CalibrantLibrary, &m_CalibrantDSpacingsVector));

    m_CalibrantTableView -> setModel(m_CalibrantLibraryModel.data());
    m_CalibrantDSpacingsView -> setModel(m_CalibrantDSpacingsModel.data());

    m_CalibrantTableView->horizontalHeader()->setStretchLastSection(true);
    m_CalibrantDSpacingsView->horizontalHeader()->setStretchLastSection(true);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  m_CalibrantTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_CalibrantTableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_CalibrantDSpacingsView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_CalibrantDSpacingsView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
  m_CalibrantTableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
  m_CalibrantTableView->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
  m_CalibrantDSpacingsView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
  m_CalibrantDSpacingsView->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#endif
  }

  connect(m_CalibrantTableView, &QTableView::customContextMenuRequested, this, &QxrdCalibrantDialog::calibrantTableContextMenu);
  connect(m_CalibrantDSpacingsView, &QTableView::customContextMenuRequested, this, &QxrdCalibrantDialog::calibrantDSpacingsContextMenu);

  connect(m_CalibrantTableView, &QTableView::clicked,       this, &QxrdCalibrantDialog::onCalibrantClick);
  connect(m_CalibrantTableView, &QTableView::doubleClicked, this, &QxrdCalibrantDialog::onCalibrantDoubleClick);

  connect(m_CalibrantLibraryModel.data(), &QAbstractItemModel::dataChanged,
          this, &QxrdCalibrantDialog::onCalibrantChanged);

  QxrdCenterFinderPtr cen(m_CenterFinder);

  if (cen) {
    connect(cen->prop_Energy(), &QcepDoubleProperty::valueChanged,
            this, &QxrdCalibrantDialog::onCalibrantChanged);
  }

  onCalibrantChanged();
}

QxrdCalibrantDialog::~QxrdCalibrantDialog()
{
}


void QxrdCalibrantDialog::onCalibrantChanged()
{
  if (m_CalibrantLibrary) {
    QxrdCenterFinderPtr cf(m_CenterFinder);

    double energy = 15000;
    if (cf) {
      energy = cf->get_Energy();
    }

    m_CalibrantDSpacingsVector.clear();

    for (int i=0; i<m_CalibrantLibrary->count(); i++) {
      QxrdCalibrantPtr cal = m_CalibrantLibrary->calibrant(i);

      if (cal && cal->get_IsUsed()) {
        m_CalibrantDSpacingsVector.merge(cal->dSpacings(energy));
      }
    }

    m_CalibrantDSpacingsModel -> everythingChanged(m_CalibrantDSpacingsVector.count());
  }
}

void QxrdCalibrantDialog::calibrantTableContextMenu(const QPoint &pos)
{
  QModelIndex index  = m_CalibrantTableView->indexAt(pos);
  QxrdCalibrantPtr cal;

  if (index.isValid()) {
    cal = m_CalibrantLibrary->calibrant(index.row());
  }

  QMenu      *menu   = new QMenu(this);
  QAction    *copy   = menu->addAction("Copy");
  QAction    *dupe   = NULL;
  QAction    *props  = NULL;
  QAction    *del    = NULL;

  if (cal) {
    dupe  = menu->addAction(tr("Duplicate Calibrant %1").arg(cal->get_Name()));
    props = menu->addAction(tr("Calibrant %1 Properties...").arg(cal->get_Name()));

    if (!cal->isLocked()) {
      del = menu->addAction(tr("Delete Calibrant %1...").arg(cal->get_Name()));
    }
  }

  QAction    *chosen = menu->exec(m_CalibrantTableView->viewport()->mapToGlobal(pos));

  if (chosen) {
    if (chosen == copy) {
      doCopyFromTable(m_CalibrantTableView);
    } else if (chosen == dupe) {
      doDuplicateCalibrant(index.row());
    } else if (chosen == props) {
      doCalibrantProperties(index.row());
    } else if (chosen == del) {
      doDeleteCalibrant(index.row());
    }
  }
}

void QxrdCalibrantDialog::calibrantDSpacingsContextMenu(const QPoint &pos)
{
  QModelIndex index  = m_CalibrantDSpacingsView->indexAt(pos);
  QMenu      *menu   = new QMenu(this);
  QAction    *copy   = menu->addAction("Copy");
  QAction    *chosen = menu->exec(m_CalibrantDSpacingsView->viewport()->mapToGlobal(pos));

  if (chosen == copy) {
    doCopyFromTable(m_CalibrantDSpacingsView);
  }
}

void QxrdCalibrantDialog::doCopyFromTable(QTableView *table)
{
  QAbstractItemModel *model     = table->model();
  QItemSelectionModel *selected = table->selectionModel();

  int nRows = model->rowCount();
  int nCols = model->columnCount();

  int firstRowSel = -1, lastRowSel = -1;
  int firstColSel = -1, lastColSel = -1;

  for (int row = 0; row<nRows; row++) {
    for (int col = 0; col<nCols; col++) {
      if (selected->isSelected(model->index(row, col))) {
        if (firstRowSel == -1 || firstRowSel > row) {
          firstRowSel = row;
        }

        if (firstColSel == -1 || firstColSel > col) {
          firstColSel = col;
        }

        if (lastRowSel == -1 || lastRowSel < row) {
          lastRowSel = row;
        }

        if (lastColSel == -1 || lastColSel < col) {
          lastColSel = col;
        }
      }
    }
  }

  if (firstRowSel >= 0 && lastRowSel >= 0 && firstColSel >= 0 && lastColSel >= 0) {
    QString result;

    for (int row = firstRowSel; row <= lastRowSel; row++) {
      for (int col = firstColSel; col <= lastColSel; col++) {
        if (col != firstColSel) {
          if (selected->columnIntersectsSelection(col,QModelIndex())) {
            result.append("\t");
          }
        }

        if (selected->isSelected(model->index(row,col))) {
          result.append(model->data(model->index(row,col)).toString());
        }
      }

      result.append("\n");
    }

    QClipboard *clip = QApplication::clipboard();

    clip->setText(result);
  } else {
    printf("No cells selected\n");
  }
}

void QxrdCalibrantDialog::onCalibrantClick(const QModelIndex &item)
{
//  printf("Single click [%d,%d]\n", item.column(), item.row());
}

void QxrdCalibrantDialog::onCalibrantDoubleClick(const QModelIndex &item)
{
//  printf("Double click [%d,%d]\n", item.column(), item.row());

  if (item.column() == 0) {
    if (m_CalibrantLibraryModel) {
      m_CalibrantLibraryModel->toggleIsUsed(item.row());
    }
  }
}

void QxrdCalibrantDialog::doDuplicateCalibrant(int n)
{
  printf("Duplicate Calibrant %d\n", n);
}

void QxrdCalibrantDialog::doDeleteCalibrant(int n)
{
  printf("Delete Calibrant %d\n", n);
}

void QxrdCalibrantDialog::doCalibrantProperties(int n)
{
  if (m_CalibrantLibrary && m_CalibrantLibraryModel) {
    QxrdCalibrantPtr cal = m_CalibrantLibrary->calibrant(n);

    QxrdCalibrantPropertiesDialog dlg(this, cal);

    if (dlg.exec() == QDialog::Accepted) {
      m_CalibrantLibraryModel->calibrantChanged(n);
    }
  }
}
