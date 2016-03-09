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

  connect(m_CalibrantTableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &QxrdCalibrantDialog::onLibrarySelectionChanged);
  connect(m_CalibrantTableView, &QTableView::customContextMenuRequested, this, &QxrdCalibrantDialog::calibrantTableContextMenu);
  connect(m_CalibrantDSpacingsView, &QTableView::customContextMenuRequested, this, &QxrdCalibrantDialog::calibrantDSpacingsContextMenu);

  connect(m_CalibrantTableView, &QTableView::clicked,       this, &QxrdCalibrantDialog::onCalibrantClick);
  connect(m_CalibrantTableView, &QTableView::doubleClicked, this, &QxrdCalibrantDialog::onCalibrantDoubleClick);
}

QxrdCalibrantDialog::~QxrdCalibrantDialog()
{
}

void QxrdCalibrantDialog::onLibrarySelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
  if (selected.size() > 0) {
    int row = selected.first().top();

    if (m_CalibrantLibrary) {
      //    m_CalibrantLibrary->printMessage(tr("Row %1 selected").arg(row));

      QxrdCalibrantPtr cal = m_CalibrantLibrary->calibrant(row);

      if (cal) {
        QxrdCenterFinderPtr cf(m_CenterFinder);

        if (cf) {
          m_CalibrantDSpacingsVector = cal->dSpacings(cf->get_Energy());
        } else {
          m_CalibrantDSpacingsVector = cal->dSpacings(15000);
        }

        m_CalibrantDSpacingsModel -> everythingChanged(m_CalibrantDSpacingsVector.count());

        //      m_CalibrantLibrary->printMessage(tr("%1 diffraction peaks").arg(m_CalibrantDSpacingsVector.count()));
      }
    }
  } else {
    m_CalibrantDSpacingsVector.clear();
    m_CalibrantDSpacingsModel -> everythingChanged(0);
  }
}

void QxrdCalibrantDialog::calibrantTableContextMenu(const QPoint &pos)
{
  QModelIndex index  = m_CalibrantTableView->indexAt(pos);
  QMenu      *menu   = new QMenu(this);
  QAction    *copy   = menu->addAction("Copy");
  QAction    *chosen = menu->exec(m_CalibrantTableView->viewport()->mapToGlobal(pos));

  if (chosen == copy) {
    doCopyFromTable(m_CalibrantTableView);
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
