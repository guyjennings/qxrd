#include "qcepdatasetbrowserdialog.h"
#include "ui_qcepdatasetbrowserdialog.h"
#include "qcepdataset.h"
#include "qcepdatasetmodel.h"
#include <QMenu>
#include <stdio.h>
#include "qxrdexperiment.h"
#include "qxrddataobjectgraphwindow.h"
#include "qxrddataobjectpropertieswindow.h"
#include "qxrddataobjectspreadsheetwindow.h"
#include "qxrdwindow.h"
#include "qcepdatacolumnscan.h"
#include <QMessageBox>
#include "qcepnewdatagroupdialog.h"
#include "qcepnewcolumnscandialog.h"
#include "qcepnewdataarraydialog.h"
#include "qcepnewdatacolumndialog.h"
#include "qxrdnewimagedialog.h"

QcepDatasetBrowserDialog::QcepDatasetBrowserDialog(QxrdExperimentWPtr expt, QcepDatasetModelPtr ds, QWidget *parent) :
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

  QModelIndex index = m_DatasetBrowserView->indexAt(pt);

  QcepDataObject     *obj = static_cast<QcepDataObject*>(index.internalPointer());
  QcepDataGroup      *grp = qobject_cast<QcepDataGroup*>(obj);
  QcepDataColumnScan *scn = qobject_cast<QcepDataColumnScan*>(obj);
  QcepDataObject     *prn = (obj ? obj->parentItem().data() : NULL);
  QcepDataColumnScan *psc = qobject_cast<QcepDataColumnScan*>(prn);

  QMenu menu(NULL, NULL);

  QString name = (obj ? obj->pathName() : "");

  QMenu   *nm = menu.addMenu("Create New");
  QAction *ng = nm->addAction(tr("New Group in %1").arg(name));

  QAction *nc = NULL;

  if (psc == NULL) {
    nc = nm->addAction(tr("New Data Column in %1").arg(name));
  } else {
    nc = nm->addAction(tr("Insert Data Column after %1").arg(name));
  }
                   ;
  QAction *ns = nm->addAction(tr("New Column Scan in %1").arg(name));
  QAction *ni = nm->addAction(tr("New Image in %1").arg(name));
  QAction *na = nm->addAction(tr("New Array in %1").arg(name));
  QAction *rd = menu.addAction(tr("Read Data into %1 ...").arg(name));
  QAction *sv = menu.addAction(tr("Save %1 as ...").arg(name));
  QAction *og = menu.addAction(tr("Open %1 in graph window").arg(name));
  QAction *os = menu.addAction(tr("Open %1 in spreadsheet window").arg(name));
  QAction *op = menu.addAction(tr("%1 properties").arg(name));
  QAction *dl = menu.addAction(tr("Delete %1").arg(name));

  ng->setEnabled(grp != NULL && scn == NULL);
  nc->setEnabled(grp != NULL || psc != NULL);
  ns->setEnabled(grp != NULL && scn == NULL);
  ni->setEnabled(grp != NULL && scn == NULL);
  na->setEnabled(grp != NULL && scn == NULL);

  nm->setEnabled(ng->isEnabled()
                 || nc->isEnabled()
                 || ns->isEnabled()
                 || ni->isEnabled()
                 || na->isEnabled());

  rd->setEnabled(grp != NULL);


  QAction *action = menu.exec(mapToGlobal(pt));

  if (action == ng) {
    newGroup(index);
  } else if (action == nc) {
    newDataColumn(index);
  } else if (action == ns) {
    newColumnScan(index);
  } else if (action == ni) {
    newImage(index);
  } else if (action == na) {
    newArray(index);
  } else if (action == rd) {
    readData(index);
  } else if (action == sv) {
    saveData(index);
  } else if (action == og) {
    openGraph(index);
  } else if (action == os) {
    openSpreadsheet(index);
  } else if (action == op) {
    openProperties(index);
  } else if (action == dl) {
    deleteData(index);
  }
}

void QcepDatasetBrowserDialog::onDoubleClicked(QModelIndex idx)
{
  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    expt->printMessage(tr("QcepDatasetBrowserDialog::onDoubleClicked([%1,%2])").arg(idx.row()).arg(idx.column()));
  }
}

void QcepDatasetBrowserDialog::openGraph(const QModelIndex &idx)
{
  QcepDataObject *obj = static_cast<QcepDataObject*>(idx.internalPointer());
  QxrdExperimentPtr expt(m_Experiment);

  if (obj && expt) {
    QxrdDataObjectGraphWindow *gw = new QxrdDataObjectGraphWindow(expt,
                                                                  obj->sharedFromThis(),
                                                                  NULL /*expt->window().data()*/);

    gw->show();
  }
}

void QcepDatasetBrowserDialog::openSpreadsheet(const QModelIndex &idx)
{
  QcepDataObject *obj = static_cast<QcepDataObject*>(idx.internalPointer());
  QxrdExperimentPtr expt(m_Experiment);

  if (obj && expt) {
    QxrdDataObjectSpreadsheetWindow *sw = new QxrdDataObjectSpreadsheetWindow(expt,
                                                                              obj->sharedFromThis(),
                                                                              NULL /*expt->window().data()*/);

    sw->show();
  }
}

void QcepDatasetBrowserDialog::openProperties(const QModelIndex &idx)
{
  QcepDataObject *obj = static_cast<QcepDataObject*>(idx.internalPointer());
  QxrdExperimentPtr expt(m_Experiment);

  if (obj && expt) {
    QxrdDataObjectPropertiesWindow *pw = new QxrdDataObjectPropertiesWindow(expt,
                                                                            obj->sharedFromThis(),
                                                                            NULL /*expt->window().data()*/);

    pw->show();
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

  QxrdNewImageDialog *dlog = new QxrdNewImageDialog(obj);

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
}

void QcepDatasetBrowserDialog::deleteData(const QModelIndex &idx)
{
  QcepDataObject *obj = static_cast<QcepDataObject*>(idx.internalPointer());
}
