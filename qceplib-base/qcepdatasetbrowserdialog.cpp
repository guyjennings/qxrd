#include "qcepdatasetbrowserdialog.h"
#include "ui_qcepdatasetbrowserdialog.h"
#include "qcepdataset.h"
#include "qcepdatasetmodel.h"
#include <QMenu>
#include <stdio.h>
#include "qcepexperiment.h"
#include "qcepdataobjectgraphwindow.h"
#include "qcepdataobjectpropertieswindow.h"
#include "qcepdataobjectspreadsheetwindow.h"
#include "qcepdatacolumnscan.h"
#include <QMessageBox>
#include "qcepimagedata.h"
#include "qcepnewdatagroupdialog.h"
#include "qcepnewcolumnscandialog.h"
#include "qcepnewdataarraydialog.h"
#include "qcepnewdatacolumndialog.h"
#include "qcepnewimagedialog.h"
#include <QFileDialog>
#include "qcepdatasetselectiondialog.h"
#include "qcepdataexportdialog.h"
#include "qcepdataimportdialog.h"

QcepDatasetBrowserDialog::QcepDatasetBrowserDialog(QcepExperimentWPtr expt, QcepDatasetModelPtr ds, QWidget *parent) :
  QDockWidget(parent),
  m_Experiment(expt),
  m_DatasetModel(ds)
{
  setupUi(this);

  if (m_DatasetModel) {
    m_DatasetBrowserView -> setModel(m_DatasetModel.data());

    connect(m_DatasetBrowserView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onCustomContextMenuRequested(QPoint)));
    connect(m_DatasetBrowserView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onDoubleClicked(QModelIndex)));

    m_DatasetBrowserView -> setContextMenuPolicy(Qt::CustomContextMenu);
    m_DatasetBrowserView -> header() -> setSectionResizeMode(QHeaderView::Stretch);

    m_DatasetBrowserView -> setSelectionBehavior(QAbstractItemView::SelectRows);
    m_DatasetBrowserView -> setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_DatasetBrowserView -> setDragEnabled(true);
    m_DatasetBrowserView -> setAcceptDrops(true);
    m_DatasetBrowserView -> viewport() ->setAcceptDrops(true);
    m_DatasetBrowserView -> setDropIndicatorShown(true);
    m_DatasetBrowserView -> setDragDropMode(QAbstractItemView::InternalMove);
    m_DatasetBrowserView -> setDragDropOverwriteMode(false);
  }
}

QcepDatasetBrowserDialog::~QcepDatasetBrowserDialog()
{
}

void QcepDatasetBrowserDialog::onCustomContextMenuRequested(QPoint pt)
{
//  m_Dataset->printMessage(tr("QcepDatasetBrowserDialog::onCustomContextMenuRequested([%1,%2])").arg(pt.x()).arg(pt.y()));

  QModelIndex index = m_DatasetBrowserView->indexAt(pt);

  QcepExperimentPtr expt(m_Experiment);

  if (expt) {
    expt->printMessage(tr("Custom context menu clicked at: %1").arg(m_DatasetModel->indexDescription(index)));
  }

  QModelIndexList indexes = m_DatasetBrowserView
      ->selectionModel()->selectedRows();

  int nSel = indexes.count();

  QcepDataObjectPtr     obj;
  QcepDataGroupPtr      grp;
  QcepDataColumnPtr     col;
  QcepDataColumnScanPtr scn;
  QcepDataObjectPtr     prn;
  QcepDataColumnScanPtr psc;
  QcepImageDataBasePtr  img;

  if (nSel == 1) {
    QModelIndex idx = indexes.value(0);

    obj = m_DatasetModel->item(idx);
    grp = m_DatasetModel->group(idx);
    col = m_DatasetModel->column(idx);
    scn = m_DatasetModel->columnScan(idx);
    img = m_DatasetModel->image(idx);

    if (obj) {
      prn = obj->parentItem();
      psc = qSharedPointerDynamicCast<QcepDataColumnScan>(prn);
    }
  } else if (nSel == 0) {
    obj = m_DatasetModel->item(QModelIndex());
    grp = m_DatasetModel->group(QModelIndex());
  }

  QMenu menu(NULL, NULL);

  QString names;

  for (int i=0; i<nSel; i++) {
    QcepDataObjectPtr o = m_DatasetModel->item(indexes.value(i));
//        static_cast<QcepDataObject*>(indexes.at(i).internalPointer());

    if (i != 0) {
      names += ", ";
    }

    names += ( o ? o -> pathName() : "<NULL>");
  }

  QString name = (obj ? obj->pathName() : "");

  QAction *tt = menu.addAction("Data operations...");

  tt->setDisabled(true);

  QMenu   *nm = menu.addMenu("Create New");

  QAction *ng = nm->addAction(tr("New Group in %1").arg(name));

  QAction *nc = NULL;

  if (psc == NULL) {
    nc = nm->addAction(tr("New Data Column in %1").arg(name));
  } else {
    nc = nm->addAction(tr("Insert Data Column after %1").arg(name));
  }

  QAction *ns = nm->addAction(tr("New Column Scan in %1").arg(name));
  QAction *ni = nm->addAction(tr("New Image in %1").arg(name));
  QAction *na = nm->addAction(tr("New Array in %1").arg(name));

  QMenu   *ops = menu.addMenu(tr("Operations on %1 ...").arg(name));

  QAction *cat = ops->addAction(tr("Concatenate to %1...").arg(name));
  QAction *add = ops->addAction(tr("Add to %1...").arg(name));
  QAction *sub = ops->addAction(tr("Subtract from %1...").arg(name));
  QAction *mul = ops->addAction(tr("Multiply %1 by...").arg(name));
  QAction *div = ops->addAction(tr("Divide %1 by...").arg(name));
  QAction *ntg = ops->addAction(tr("Circular Integrate %1").arg(names));
  QAction *ntgp = ops->addAction(tr("Circular Integrate %1 Parameters...").arg(names));
  QAction *pol = ops->addAction(tr("Polar Transform %1").arg(names));
  QAction *polp = ops->addAction(tr("Polar Transform %1 Parameters...").arg(names));
  QAction *pli = ops->addAction(tr("Polar Integrate %1").arg(names));
  QAction *plip = ops->addAction(tr("Polar Integrate %1 Parameters...").arg(names));

  QAction *rd = menu.addAction(tr("Read Data into %1 ...").arg(name));
  QAction *sv = menu.addAction(tr("Save %1 as ...").arg(name));
  QAction *og = menu.addAction(tr("Open %1 in graph window").arg(names));
  QAction *os = menu.addAction(tr("Open %1 in spreadsheet window").arg(names));
  QAction *op = menu.addAction(tr("Open %1 in properties window").arg(names));
  QAction *dl = menu.addAction(tr("Delete %1").arg(names));

  ng->setEnabled(nSel == 0 || (nSel == 1 && (grp != NULL && scn == NULL)));
  nc->setEnabled(nSel == 0 || (nSel == 1 && (grp != NULL || psc != NULL)));
  ns->setEnabled(nSel == 0 || (nSel == 1 && (grp != NULL && scn == NULL)));
  ni->setEnabled(nSel == 0 || (nSel == 1 && (grp != NULL && scn == NULL)));
  na->setEnabled(nSel == 0 || (nSel == 1 && (grp != NULL && scn == NULL)));

  cat->setEnabled(nSel == 1 && scn);
  add->setEnabled(nSel == 1 && (scn || col || img));
  sub->setEnabled(nSel == 1 && (scn || col || img));
  mul->setEnabled(nSel == 1 && (scn || col || img));
  div->setEnabled(nSel == 1 && (scn || col || img));

  ntg->setEnabled(nSel >= 1);

  rd->setEnabled(nSel == 1);
  sv->setEnabled(nSel == 1);
  og->setEnabled(nSel >= 1);
  os->setEnabled(nSel >= 1);
  op->setEnabled(nSel >= 1);
  dl->setEnabled(nSel >= 1);

  nm->setEnabled(ng->isEnabled()
                 || nc->isEnabled()
                 || ns->isEnabled()
                 || ni->isEnabled()
                 || na->isEnabled());

  rd->setEnabled(grp != NULL);


  QAction *action = menu.exec(QCursor::pos(), tt);

  if (action == ng) {
    newGroup(indexes);
  } else if (action == nc) {
    newDataColumn(indexes);
  } else if (action == ns) {
    newColumnScan(indexes);
  } else if (action == ni) {
    newImage(indexes);
  } else if (action == na) {
    newArray(indexes);
  } else if (action == cat) {
    concatenateData(indexes.value(0));
  } else if (action == add) {
    addData(indexes.value(0));
  } else if (action == sub) {
    subtractData(indexes.value(0));
  } else if (action == mul) {
    multiplyData(indexes.value(0));
  } else if (action == div) {
    divideData(indexes.value(0));
  } else if (action == ntg) {
    integrateData(indexes);
  } else if (action == ntgp) {
    integrateParameters(indexes);
  } else if (action == pol) {
    polarTransformData(indexes);
  } else if (action == polp) {
    polarTransformParameters(indexes);
  } else if (action == pli) {
    polarIntegrateData(indexes);
  } else if (action == plip) {
    polarIntegrateParameters(indexes);
  } else if (action == rd) {
    readData(indexes);
  } else if (action == sv) {
    saveData(indexes);
  } else if (action == og) {
    openGraph(indexes);
  } else if (action == os) {
    openSpreadsheet(indexes);
  } else if (action == op) {
    openProperties(indexes);
  } else if (action == dl) {
    deleteData(indexes);
  }
}

void QcepDatasetBrowserDialog::onDoubleClicked(QModelIndex idx)
{
  QcepExperimentPtr expt(m_Experiment);

  if (expt) {
    expt->printMessage(tr("QcepDatasetBrowserDialog::onDoubleClicked([%1,%2])").arg(idx.row()).arg(idx.column()));
  }
}

void QcepDatasetBrowserDialog::openGraph(const QModelIndexList &idx)
{
  for (int i=0; i<idx.count(); i++) {
    QcepDataObjectPtr obj = m_DatasetModel -> item(idx.value(i));

    QcepExperimentPtr expt(m_Experiment);

    if (obj && expt) {
      QcepDataObjectGraphWindow *gw = new QcepDataObjectGraphWindow(expt, obj);

      gw->show();
    }
  }
}

void QcepDatasetBrowserDialog::openSpreadsheet(const QModelIndexList &idx)
{
  for (int i=0; i<idx.count(); i++) {
    QcepDataObjectPtr obj = m_DatasetModel -> item(idx.value(i));

    QcepExperimentPtr expt(m_Experiment);

    if (obj && expt) {
      QcepDataObjectSpreadsheetWindow *sw = new QcepDataObjectSpreadsheetWindow(expt, obj);

      sw->show();
    }
  }
}

void QcepDatasetBrowserDialog::openProperties(const QModelIndexList &idx)
{
  for (int i=0; i<idx.count(); i++) {
    QcepDataObjectPtr obj = m_DatasetModel -> item(idx.value(i));

    QcepExperimentPtr expt(m_Experiment);

    if (obj && expt) {
      QcepDataObjectPropertiesWindow *pw = new QcepDataObjectPropertiesWindow(expt, obj);

      pw->show();
    }
  }
}

void QcepDatasetBrowserDialog::newGroup(const QModelIndexList &idx)
{
  QcepNewDataGroupDialog dlog(m_DatasetModel, idx.value(0));

  dlog.exec();
}

void QcepDatasetBrowserDialog::newDataColumn(const QModelIndexList &idx)
{
  QcepNewDataColumnDialog dlog(m_DatasetModel, idx.value(0));

  dlog.exec();
}

void QcepDatasetBrowserDialog::newColumnScan(const QModelIndexList &idx)
{
  QcepNewColumnScanDialog dlog(m_DatasetModel, idx.value(0));

  dlog.exec();
}

void QcepDatasetBrowserDialog::newImage(const QModelIndexList &idx)
{
  QcepNewImageDialog dlog(m_DatasetModel, idx.value(0));

  dlog.exec();
}

void QcepDatasetBrowserDialog::newArray(const QModelIndexList &idx)
{
  QcepNewDataArrayDialog dlog(m_DatasetModel, idx.value(0));

  dlog.exec();
}

void QcepDatasetBrowserDialog::readData(const QModelIndexList &idx)
{
  static QString selectedFilter;
  QStringList theFiles = QFileDialog::getOpenFileNames(this,
                                                       "Load data from", "",
                                                       "", &selectedFilter);

  if (theFiles.length() > 0) {
    QcepDataImportDialog dlog(m_DatasetModel, idx);

    dlog.exec();
  }
}

void QcepDatasetBrowserDialog::saveData(const QModelIndexList &idx)
{
  QcepDataObjectPtr obj = m_DatasetModel -> item(idx.value(0));

  static QString selectedFilter;
  QString theFile = QFileDialog::getSaveFileName(this,
                                                 "Save data in", obj->get_FileName(),
                                                 obj->fileFormatFilterString(), &selectedFilter);

  if (theFile.length()) {
    QcepDataExportDialog dlog(m_DatasetModel, idx);

    dlog.exec();
  }
}

void QcepDatasetBrowserDialog::deleteData(const QModelIndexList &idx)
{
  QString name;

  for (int i=0; i<idx.count(); i++) {
    QcepDataObjectPtr obj = m_DatasetModel -> item(idx.value(i));

    if (obj) {
      if (i != 0) {
        name += ", ";
      }

      name += obj->get_Name();
    }
  }

  if (name.length() > 0) {
    if (QMessageBox::question(NULL, "Delete Object(s)?",
                              tr("Do you really want to delete %1").arg(name),
                              QMessageBox::Ok | QMessageBox::No, QMessageBox::No) == QMessageBox::Ok) {

      for (int i=0; i<idx.count(); i++) {
        m_DatasetModel->remove(idx.at(i));
      }
    }
  }
}

void QcepDatasetBrowserDialog::concatenateData(const QModelIndex &idx)
{
  QcepDatasetSelectionDialog dlg("Concatenate to %1", m_DatasetModel, idx);

  if (dlg.exec() == QDialog::Accepted) {
    if (dlg.constantChosen()) {
      m_DatasetModel->concatenateData(idx, dlg.constantValue());
    } else {
      m_DatasetModel->concatenateData(idx, dlg.selectedIndexes());
    }
  }
}

void QcepDatasetBrowserDialog::addData(const QModelIndex &idx)
{
  QcepDatasetSelectionDialog dlg("Add to %1", m_DatasetModel, idx);

  if (dlg.exec() == QDialog::Accepted) {
    if (dlg.constantChosen()) {
      m_DatasetModel->addData(idx, dlg.constantValue());
    } else {
      m_DatasetModel->addData(idx, dlg.selectedIndexes());
    }
  }
}

void QcepDatasetBrowserDialog::subtractData(const QModelIndex &idx)
{
  QcepDatasetSelectionDialog dlg("Subtract from %1", m_DatasetModel, idx);

  if (dlg.exec() == QDialog::Accepted) {
    if (dlg.constantChosen()) {
      m_DatasetModel->subtractData(idx, dlg.constantValue());
    } else {
      m_DatasetModel->subtractData(idx, dlg.selectedIndexes());
    }
  }
}

void QcepDatasetBrowserDialog::multiplyData(const QModelIndex &idx)
{
  QcepDatasetSelectionDialog dlg("Multiply %1 by", m_DatasetModel, idx);

  if (dlg.exec() == QDialog::Accepted) {
    if (dlg.constantChosen()) {
      m_DatasetModel->multiplyData(idx, dlg.constantValue());
    } else {
      m_DatasetModel->multiplyData(idx, dlg.selectedIndexes());
    }
  }
}

void QcepDatasetBrowserDialog::divideData(const QModelIndex &idx)
{
  QcepDatasetSelectionDialog dlg("Divide %1 by", m_DatasetModel, idx);

  if (dlg.exec() == QDialog::Accepted) {
    if (dlg.constantChosen()) {
      m_DatasetModel->divideData(idx, dlg.constantValue());
    } else {
      m_DatasetModel->divideData(idx, dlg.selectedIndexes());
    }
  }
}

void QcepDatasetBrowserDialog::integrateData(const QModelIndexList &idx)
{
  for (int i=0; i<idx.count(); i++) {
    m_DatasetModel->integrate(idx.value(i));
  }
}

void QcepDatasetBrowserDialog::polarTransformData(const QModelIndexList &idx)
{
  for (int i=0; i<idx.count(); i++) {
    m_DatasetModel->polarTransform(idx.value(i));
  }
}

void QcepDatasetBrowserDialog::polarIntegrateData(const QModelIndexList &idx)
{
  for (int i=0; i<idx.count(); i++) {
    m_DatasetModel->polarIntegrate(idx.value(i));
  }
}

void QcepDatasetBrowserDialog::integrateParameters(const QModelIndexList &idx)
{
  if (m_DatasetModel->integrateParameters()) {
    for (int i=0; i<idx.count(); i++) {
      m_DatasetModel->integrate(idx.value(i));
    }
  }
}

void QcepDatasetBrowserDialog::polarTransformParameters(const QModelIndexList &idx)
{
  if (m_DatasetModel->polarTransformParameters()) {
    for (int i=0; i<idx.count(); i++) {
      m_DatasetModel->polarTransform(idx.value(i));
    }
  }
}

void QcepDatasetBrowserDialog::polarIntegrateParameters(const QModelIndexList &idx)
{
  if (m_DatasetModel->polarIntegrateParameters()) {
    for (int i=0; i<idx.count(); i++) {
      m_DatasetModel->polarIntegrate(idx.value(i));
    }
  }
}
