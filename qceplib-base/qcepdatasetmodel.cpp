#include "qcepdatasetmodel.h"
#include "qcepdataobject.h"
#include "qcepdataobject-ptr.h"
#include "qcepdataset.h"
#include "qcepdebug.h"
#include <QMimeData>
#include <stdio.h>

QcepDatasetModel::QcepDatasetModel(QcepDatasetPtr ds) :
  m_Dataset(ds)
{
  connect(ds.data(), SIGNAL(dataObjectChanged()), this, SLOT(onDataObjectChanged()));
}

QcepDataObjectPtr QcepDatasetModel::indexedObject(const QModelIndex &index) const
{
  QcepDataObjectPtr res;

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QcepDatasetModel::indexedObject(%s)\n", qPrintable(indexDescription(index)));
  }

  if (index.isValid()) {
    QcepDataObject *obj = static_cast<QcepDataObject*>(index.internalPointer());

    if (obj) {
//      if (qcepDebug(DEBUG_DATABROWSER)) {
//        printf("of object %s\n", qPrintable(obj->get_Name()));
//      }

      res = obj->sharedFromThis();
    }
//  } else {
//    res = NULL;
  }

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QcepDatasetModel::indexedObject(%s)", qPrintable(indexDescription(index)));
    if (res) {
      printf(" = %s\n", qPrintable(res->get_Name()));
    } else {
      printf(" = null\n");
    }
  }

  return res;
}

QModelIndex QcepDatasetModel::index(int row, int column, const QModelIndex &parent) const
{
  QModelIndex res = QModelIndex();

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QcepDatasetModel::index(%d,%d,%s)\n",
           row, column, qPrintable(indexDescription(parent)));
  }

//  if (hasIndex(row, column, parent)) {
  QcepDataObjectPtr parentItem = indexedObject(parent);

  if (!parentItem) {
    parentItem = m_Dataset;
  }

  if (parentItem) {
    QcepDataObjectPtr childItem = parentItem->item(row);

    if (childItem) {
      res = createIndex(row, column, childItem.data());
    }
  }
  //  }

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QcepDatasetModel::index(%d,%d,%s) = %s\n", row, column,
           qPrintable(indexDescription(parent)),
           qPrintable(indexDescription(res)));
  }

  return res;
}

QModelIndex QcepDatasetModel::parent(const QModelIndex &index) const
{
  QModelIndex res = QModelIndex();

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QseDatasetModel::parent(%s)\n", qPrintable(indexDescription(index)));
  }

  QcepDataObjectPtr childItem = indexedObject(index);

  if (childItem) {
//    if (qcepDebug(DEBUG_DATABROWSER)) {
//      printf("of object %s\n", qPrintable(childItem->get_Name()));
//    }

    QcepDataObjectPtr parentItem = childItem->parentItem();

    if (parentItem) {
      if (parentItem != m_Dataset) {
        res = createIndex(parentItem->indexInParent(), 0, parentItem.data());
      }
    }
  }

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QseDatasetModel::parent(%s)", qPrintable(indexDescription(index)));
    printf(" = (%s)\n", qPrintable(indexDescription(res)));
  }

  return res;
}

int QcepDatasetModel::rowCount(const QModelIndex &parent) const
{
  int res = 0;

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QseDatasetModel::rowCount(%s)\n", qPrintable(indexDescription(parent)));
  }

  if (parent.column() <= 0) {
    QcepDataObjectPtr parentItem = indexedObject(parent);

    if (parentItem) {
      int nrows = parentItem->count();

      res = nrows;
    } else {
      res = m_Dataset->count();
    }
  }

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QseDatasetModel::rowCount(%s)", qPrintable(indexDescription(parent)));
    printf(" = %d\n", res);
  }

  return res;
}

int QcepDatasetModel::columnCount(const QModelIndex &parent) const
{
  int res = 0;

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QseDatasetModel::columnCount(%s)\n", qPrintable(indexDescription(parent)));
  }

  QcepDataObjectPtr parentItem = indexedObject(parent);

  if (parentItem) {
    int ncols = parentItem->columnCount();

    res = ncols;
  } else {
    res = m_Dataset->columnCount();
  }

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QseDatasetModel::columnCount(%s)", qPrintable(indexDescription(parent)));
    printf(" = %d\n", res);
  }

  return res;
}

QVariant QcepDatasetModel::data(const QModelIndex &index, int role) const
{
  QVariant res = QVariant();

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QseDatasetModel::data(%s,%d)\n", qPrintable(indexDescription(index)), role);
  }

  if (!index.isValid()) {
    res = QVariant();
  }  else if (role == Qt::DisplayRole) {

    QcepDataObjectPtr object = indexedObject(index);

    if (object) {
      res = object->columnData(index.column());
    }
  }

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QseDatasetModel::data(%s,%d)", qPrintable(indexDescription(index)), role);
    printf(" = %s\n", qPrintable(res.toString()));
  }

  return res;
}

Qt::ItemFlags QcepDatasetModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
      return Qt::ItemIsEnabled;

  return QAbstractItemModel::flags(index) | Qt::ItemIsDragEnabled;
}

QStringList QcepDatasetModel::mimeTypes() const
{
//  if (qcepDebug(DEBUG_DRAGDROP)) {
    printf("QcepDatasetModel::mimeTypes\n");
//  }

  QStringList types;
  types << "application/vnd.text.list";
//  types << "text/plain";
  return types;
}

QMimeData  *QcepDatasetModel::mimeData(const QModelIndexList &indexes) const
{
  QMimeData *mimeData = new QMimeData();
  QString textData;

  foreach (const QModelIndex &index, indexes) {
      if (index.isValid()) {
          QString text = data(index, Qt::DisplayRole).toString();
          textData += text;
      }
  }

  if (qcepDebug(DEBUG_DRAGDROP)) {
    printf("QcepDatasetModel::mimeData = %s\n", qPrintable(textData));
  }

  mimeData->setText(textData);
//  mimeData->setData("text/plain", encodedData);
  return mimeData;
}

QString QcepDatasetModel::indexDescription(const QModelIndex &index) const
{
  if (index.isValid()) {
    QcepDataObject* obj = static_cast<QcepDataObject*>(index.internalPointer());

    if (obj) {
      return tr("(%1,%2,\"%3\")")
          .arg(index.row()).arg(index.column())
          .arg(obj->get_Name());
    } else {
      return tr("(%1,%2,\"%3\")")
          .arg(index.row()).arg(index.column())
          .arg(m_Dataset->get_Name());
    }
  }

  return tr("(%1,%2,null)").arg(index.row()).arg(index.column());
}

void QcepDatasetModel::onDataObjectChanged()
{
  beginResetModel();
  endResetModel();
}
