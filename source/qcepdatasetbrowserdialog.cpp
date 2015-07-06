#include "qcepdatasetbrowserdialog.h"
#include "ui_qcepdatasetbrowserdialog.h"
#include "qcepdataset.h"
#include "qcepdatasetmodel.h"
#include <QMenu>
#include <stdio.h>

QcepDatasetBrowserDialog::QcepDatasetBrowserDialog(QcepDatasetPtr ds, QWidget *parent) :
  QDockWidget(parent),
  m_Dataset(ds)
{
  setupUi(this);

  if (m_Dataset) {
    m_DatasetModel = QcepDatasetModelPtr(
          new QcepDatasetModel(m_Dataset));

    m_DatasetBrowserView -> setModel(m_DatasetModel.data());

    connect(m_DatasetBrowserView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onCustomContextMenuRequested(QPoint)));
    connect(m_DatasetBrowserView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onDoubleClicked(QModelIndex)));

    m_DatasetBrowserView -> setContextMenuPolicy(Qt::CustomContextMenu);
  }
}

QcepDatasetBrowserDialog::~QcepDatasetBrowserDialog()
{
}

void QcepDatasetBrowserDialog::onCustomContextMenuRequested(QPoint pt)
{
//  m_Dataset->printMessage(tr("QcepDatasetBrowserDialog::onCustomContextMenuRequested([%1,%2])").arg(pt.x()).arg(pt.y()));

  QModelIndex index = m_DatasetBrowserView->indexAt(pt);

  QcepDataObject *obj = static_cast<QcepDataObject*>(index.internalPointer());

  QMenu menu(NULL, NULL);

  QString name = (obj ? obj->pathName() : "");

  QAction *a1 = menu.addAction(tr("Open %1 in graph window").arg(name));
  QAction *a2 = menu.addAction(tr("Open %1 in spreadsheet window").arg(name));
  QAction *a3 = menu.addAction(tr("%1 properties").arg(name));

  QAction *action = menu.exec(mapToGlobal(pt));

  if (action == a1) {
    openGraph(index);
  } else if (action == a2) {
    openSpreadsheet(index);
  } else if (action == a3) {
    openProperties(index);
  }
}

void QcepDatasetBrowserDialog::onDoubleClicked(QModelIndex idx)
{
  m_Dataset->printMessage(tr("QcepDatasetBrowserDialog::onDoubleClicked([%1,%2])").arg(idx.row()).arg(idx.column()));
}

void QcepDatasetBrowserDialog::openGraph(const QModelIndex &idx)
{
  QcepDataObject *obj = static_cast<QcepDataObject*>(idx.internalPointer());

  if (obj) {
    printf("openGraph(%s)\n", qPrintable(obj->pathName()));
  }
}

void QcepDatasetBrowserDialog::openSpreadsheet(const QModelIndex &idx)
{
  QcepDataObject *obj = static_cast<QcepDataObject*>(idx.internalPointer());

  if (obj) {
    printf("openSpreadsheet(%s)\n", qPrintable(obj->pathName()));
  }
}

void QcepDatasetBrowserDialog::openProperties(const QModelIndex &idx)
{
  QcepDataObject *obj = static_cast<QcepDataObject*>(idx.internalPointer());

  if (obj) {
    printf("openProperties(%s)\n", qPrintable(obj->pathName()));
  }
}
