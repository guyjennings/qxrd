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
  }
}

QcepDatasetBrowserDialog::~QcepDatasetBrowserDialog()
{
}

void QcepDatasetBrowserDialog::onCustomContextMenuRequested(QPoint pt)
{
//  m_Dataset->printMessage(tr("QcepDatasetBrowserDialog::onCustomContextMenuRequested([%1,%2])").arg(pt.x()).arg(pt.y()));

//  QModelIndex index = m_DatasetBrowserView->indexAt(pt);

  QModelIndexList indexes = m_DatasetBrowserView
      ->selectionModel()->selectedRows();

  int nSel = indexes.count();

  QcepDataObject     *obj = NULL;
  QcepDataGroup      *grp = NULL;
  QcepDataColumn     *col = NULL;
  QcepDataColumnScan *scn = NULL;
  QcepDataObject     *prn = NULL;
  QcepDataColumnScan *psc = NULL;
  QcepImageDataBase  *img = NULL;

  if (nSel == 1) {
    obj = static_cast<QcepDataObject*>(indexes.first().internalPointer());
    grp = qobject_cast<QcepDataGroup*>(obj);
    col = qobject_cast<QcepDataColumn*>(obj);
    scn = qobject_cast<QcepDataColumnScan*>(obj);
    prn = (obj ? obj->parentItem().data() : NULL);
    psc = qobject_cast<QcepDataColumnScan*>(prn);
    img = qobject_cast<QcepImageDataBase*>(obj);
  }

  QMenu menu(NULL, NULL);

  QString names;

  for (int i=0; i<nSel; i++) {
    QcepDataObject *o =
        static_cast<QcepDataObject*>(indexes.at(i).internalPointer());

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

  ng->setEnabled(nSel == 1 && (grp != NULL && scn == NULL));
  nc->setEnabled(nSel == 1 && (grp != NULL || psc != NULL));
  ns->setEnabled(nSel == 1 && (grp != NULL && scn == NULL));
  ni->setEnabled(nSel == 1 && (grp != NULL && scn == NULL));
  na->setEnabled(nSel == 1 && (grp != NULL && scn == NULL));

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
    newGroup(indexes.first());
  } else if (action == nc) {
    newDataColumn(indexes.first());
  } else if (action == ns) {
    newColumnScan(indexes.first());
  } else if (action == ni) {
    newImage(indexes.first());
  } else if (action == na) {
    newArray(indexes.first());
  } else if (action == cat) {
    concatenateData(indexes.first());
  } else if (action == add) {
    addData(indexes.first());
  } else if (action == sub) {
    subtractData(indexes.first());
  } else if (action == mul) {
    multiplyData(indexes.first());
  } else if (action == div) {
    divideData(indexes.first());
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
    readData(indexes.first());
  } else if (action == sv) {
    saveData(indexes.first());
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
    QcepDataObject *obj = static_cast<QcepDataObject*>(idx.at(i).internalPointer());

    QcepExperimentPtr expt(m_Experiment);

    if (obj && expt) {
      QcepDataObjectGraphWindow *gw = new QcepDataObjectGraphWindow(expt, obj->sharedFromThis());

      gw->show();
    }
  }
}

void QcepDatasetBrowserDialog::openSpreadsheet(const QModelIndexList &idx)
{
  for (int i=0; i<idx.count(); i++) {
    QcepDataObject *obj = static_cast<QcepDataObject*>(idx.at(i).internalPointer());

    QcepExperimentPtr expt(m_Experiment);

    if (obj && expt) {
      QcepDataObjectSpreadsheetWindow *sw = new QcepDataObjectSpreadsheetWindow(expt, obj->sharedFromThis());

      sw->show();
    }
  }
}

void QcepDatasetBrowserDialog::openProperties(const QModelIndexList &idx)
{
  for (int i=0; i<idx.count(); i++) {
    QcepDataObject *obj = static_cast<QcepDataObject*>(idx.at(i).internalPointer());

    QcepExperimentPtr expt(m_Experiment);

    if (obj && expt) {
      QcepDataObjectPropertiesWindow *pw = new QcepDataObjectPropertiesWindow(expt, obj->sharedFromThis());

      pw->show();
    }
  }
}

void QcepDatasetBrowserDialog::newGroup(const QModelIndex &idx)
{
  QcepDataObject *obj = static_cast<QcepDataObject*>(idx.internalPointer());

  QcepNewDataGroupDialog *dlog = new QcepNewDataGroupDialog(obj);

  if (dlog->exec()) {
  }

  delete dlog;
}

void QcepDatasetBrowserDialog::newDataColumn(const QModelIndex &idx)
{
  QcepDataObject *obj = static_cast<QcepDataObject*>(idx.internalPointer());

  QcepNewDataColumnDialog *dlog = new QcepNewDataColumnDialog(obj);

  if (dlog->exec()) {
  }

  delete dlog;
}

void QcepDatasetBrowserDialog::newColumnScan(const QModelIndex &idx)
{
  QcepDataObject *obj = static_cast<QcepDataObject*>(idx.internalPointer());

  QcepNewColumnScanDialog *dlog = new QcepNewColumnScanDialog(obj);

  if (dlog->exec()) {
  }

  delete dlog;
}

void QcepDatasetBrowserDialog::newImage(const QModelIndex &idx)
{
  QcepDataObject *obj = static_cast<QcepDataObject*>(idx.internalPointer());

  QcepNewImageDialog *dlog = new QcepNewImageDialog(obj);

  if (dlog->exec()) {
  }

  delete dlog;
}

void QcepDatasetBrowserDialog::newArray(const QModelIndex &idx)
{
  QcepDataObject *obj = static_cast<QcepDataObject*>(idx.internalPointer());

  QcepNewDataArrayDialog *dlog = new QcepNewDataArrayDialog(obj);

  if (dlog->exec()) {
  }

  delete dlog;
}

void QcepDatasetBrowserDialog::readData(const QModelIndex &idx)
{
  QcepDataObject *obj = static_cast<QcepDataObject*>(idx.internalPointer());
}

void QcepDatasetBrowserDialog::saveData(const QModelIndex &idx)
{
  QcepDataObject *obj = static_cast<QcepDataObject*>(idx.internalPointer());

  if (obj) {
    static QString selectedFilter;
    QString theFile = QFileDialog::getSaveFileName(this,
                                                   "Save data in", obj->get_FileName(),
                                                   obj->fileFormatFilterString(), &selectedFilter);

    if (theFile.length()) {
      obj -> saveData(theFile, selectedFilter, QcepDataObject::CanOverwrite);
    }
  }
}

void QcepDatasetBrowserDialog::deleteData(const QModelIndexList &idx)
{
  QString name;

  for (int i=0; i<idx.count(); i++) {
    QcepDataObject *obj = static_cast<QcepDataObject*>(idx.at(i).internalPointer());

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
    m_DatasetModel->integrate(idx.at(i));
  }
}

void QcepDatasetBrowserDialog::polarTransformData(const QModelIndexList &idx)
{
  for (int i=0; i<idx.count(); i++) {
    m_DatasetModel->polarTransform(idx.at(i));
  }
}

void QcepDatasetBrowserDialog::polarIntegrateData(const QModelIndexList &idx)
{
  for (int i=0; i<idx.count(); i++) {
    m_DatasetModel->polarIntegrate(idx.at(i));
  }
}

void QcepDatasetBrowserDialog::integrateParameters(const QModelIndexList &idx)
{
  if (m_DatasetModel->integrateParameters()) {
    for (int i=0; i<idx.count(); i++) {
      m_DatasetModel->integrate(idx.at(i));
    }
  }
}

void QcepDatasetBrowserDialog::polarTransformParameters(const QModelIndexList &idx)
{
  if (m_DatasetModel->polarTransformParameters()) {
    for (int i=0; i<idx.count(); i++) {
      m_DatasetModel->polarTransform(idx.at(i));
    }
  }
}

void QcepDatasetBrowserDialog::polarIntegrateParameters(const QModelIndexList &idx)
{
  if (m_DatasetModel->polarIntegrateParameters()) {
    for (int i=0; i<idx.count(); i++) {
      m_DatasetModel->polarIntegrate(idx.at(i));
    }
  }
}
