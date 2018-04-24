#include "qxrdcalibrantwindow.h"
#include "qxrdexperiment.h"
#include "qxrdcalibrantpropertiesdialog.h"
#include "qcepcalibrantlibrary.h"
#include "qcepcalibrantlibrarymodel.h"
#include "qcepcalibrantdspacings.h"
#include "qcepcalibrantdspacingsmodel.h"
#include "qcepcalibrant.h"
#include "qcepcenterfinder.h"
#include <QClipboard>
#include <QThread>

QxrdCalibrantWindow::QxrdCalibrantWindow(QString name) :
  inherited(name)
{
}

void QxrdCalibrantWindow::initialize(QcepObjectWPtr parent)
{
  GUI_THREAD_CHECK;

  setupUi(this);

  inherited::initialize(parent);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);

  QxrdExperimentPtr exp(QxrdExperiment::findExperiment(m_Parent));

  if (exp) {
    m_CenterFinder            = exp->centerFinder();

    m_CalibrantLibrary        = exp->calibrantLibrary();
    m_CalibrantLibraryModel   = exp->calibrantLibraryModel();

    m_CalibrantDSpacings      = exp->calibrantDSpacings();
    m_CalibrantDSpacingsModel = exp->calibrantDSpacingsModel();
  } else {
    printMessage("No experiment available for calibrant window");
  }

  QcepCalibrantLibraryModelPtr   lib(m_CalibrantLibraryModel);
  QcepCalibrantDSpacingsModelPtr dsp(m_CalibrantDSpacingsModel);

  if (lib) {
    m_CalibrantTableView     -> setModel(lib.data());
  }

  if (dsp) {
    m_CalibrantDSpacingsView -> setModel(dsp.data());
  }

  m_CalibrantTableView->horizontalHeader()->setStretchLastSection(true);
  m_CalibrantDSpacingsView->horizontalHeader()->setStretchLastSection(true);

  m_CalibrantTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_CalibrantTableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_CalibrantDSpacingsView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_CalibrantDSpacingsView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

  CONNECT_CHECK(connect(m_CalibrantTableView, &QTableView::customContextMenuRequested, this, &QxrdCalibrantWindow::calibrantTableContextMenu));
  CONNECT_CHECK(connect(m_CalibrantDSpacingsView, &QTableView::customContextMenuRequested, this, &QxrdCalibrantWindow::calibrantDSpacingsContextMenu));

  CONNECT_CHECK(connect(m_CalibrantTableView, &QTableView::clicked,       this, &QxrdCalibrantWindow::onCalibrantClick));
  CONNECT_CHECK(connect(m_CalibrantTableView, &QTableView::doubleClicked, this, &QxrdCalibrantWindow::onCalibrantDoubleClick));

  if (lib) {
    CONNECT_CHECK(
          connect(lib.data(), &QAbstractItemModel::dataChanged,
                  this, &QxrdCalibrantWindow::onCalibrantChanged));
  }

  QcepCenterFinderPtr cfp(m_CenterFinder);

  if (cfp) {
    CONNECT_CHECK(
          connect(cfp->prop_Energy(), &QcepDoubleProperty::valueChanged,
                  this, &QxrdCalibrantWindow::onCalibrantChanged));
  } else {
    printMessage("No center finder for calibrant window");
  }

  onCalibrantChanged();
}

QxrdCalibrantWindow::~QxrdCalibrantWindow()
{
}

void QxrdCalibrantWindow::changeEvent(QEvent *e)
{
  inherited::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}

void QxrdCalibrantWindow::onCalibrantChanged()
{
  QcepCalibrantLibraryPtr lib(m_CalibrantLibrary);
  QcepCalibrantDSpacingsPtr dsp(m_CalibrantDSpacings);
  QcepCenterFinderPtr cf(m_CenterFinder);
  QcepCalibrantDSpacingsModelPtr model(m_CalibrantDSpacingsModel);

  if (lib && dsp && cf && model) {
    double energy = cf->get_Energy();

    dsp->clear();

    for (int i=0; i<lib->count(); i++) {
      QcepCalibrantPtr cal = lib->calibrant(i);

      if (cal && cal->get_IsUsed()) {
        dsp->merge(cal->dSpacings(energy));
      }
    }

    model -> everythingChanged(dsp->count());
  }
}

void QxrdCalibrantWindow::calibrantTableContextMenu(const QPoint &pos)
{
  QModelIndex index  = m_CalibrantTableView->indexAt(pos);
  QcepCalibrantLibraryPtr lib(m_CalibrantLibrary);
  QcepCalibrantPtr cal;

  if (index.isValid() && lib) {
    cal = lib->calibrant(index.row());
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

void QxrdCalibrantWindow::calibrantDSpacingsContextMenu(const QPoint &pos)
{
//  QModelIndex index  = m_CalibrantDSpacingsView->indexAt(pos);
  QMenu      *menu   = new QMenu(this);
  QAction    *copy   = menu->addAction("Copy");
  QAction    *chosen = menu->exec(m_CalibrantDSpacingsView->viewport()->mapToGlobal(pos));

  if (chosen == copy) {
    doCopyFromTable(m_CalibrantDSpacingsView);
  }
}

void QxrdCalibrantWindow::doCopyFromTable(QTableView *table)
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

void QxrdCalibrantWindow::onCalibrantClick(const QModelIndex & /*item*/)
{
//  printf("Single click [%d,%d]\n", item.column(), item.row());
}

void QxrdCalibrantWindow::onCalibrantDoubleClick(const QModelIndex &item)
{
//  printf("Double click [%d,%d]\n", item.column(), item.row());

  if (item.column() == 0) {
    QcepCalibrantLibraryModelPtr lib(m_CalibrantLibraryModel);

    if (lib) {
      lib->toggleIsUsed(item.row());
    }
  }
}

void QxrdCalibrantWindow::doDuplicateCalibrant(int n)
{
  printf("Duplicate Calibrant %d\n", n);
}

void QxrdCalibrantWindow::doDeleteCalibrant(int n)
{
  printf("Delete Calibrant %d\n", n);
}

void QxrdCalibrantWindow::doCalibrantProperties(int n)
{
  QcepCalibrantLibraryPtr lib(m_CalibrantLibrary);
  QcepCalibrantLibraryModelPtr mdl(m_CalibrantLibraryModel);

  if (lib && mdl) {
    QcepCalibrantPtr cal = lib->calibrant(n);

    QxrdCalibrantPropertiesDialog dlg(this, cal);

    if (dlg.exec() == QDialog::Accepted) {
      mdl->calibrantChanged(n);
    }
  }
}
