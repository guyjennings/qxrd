#include "qcepdatasetmodel.h"
#include "qcepdataobject.h"
#include "qcepdataobject-ptr.h"
#include "qcepexperiment.h"
#include "qcepdataset.h"
#include "qcepdebug.h"
#include "qcepdataprocessorbase.h"
#include <QMimeData>
#include <stdio.h>
#include <QFileInfo>
#include <QDir>

QcepDatasetModel::QcepDatasetModel(QcepExperimentWPtr expt, QcepDataProcessorBaseWPtr proc, QcepDatasetWPtr ds) :
  m_Experiment(expt),
  m_Processor(proc),
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
      res = obj->sharedFromThis();

      if (!res) {
        printf("QcepDatasetModel::indexedObject returns NULL\n");
      }
    }
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

QModelIndex QcepDatasetModel::index(const QcepDataObjectPtr &obj) const
{
  if (obj == NULL || obj == m_Dataset) {
    return QModelIndex();
  } else {
    QcepDataGroupPtr parent = obj->parentItem();

    if (parent) {
      int n = obj->indexInParent();

      if (n >= 0) {
        return createIndex(n, 0, obj.data());
      }
    }

    return QModelIndex();
  }
}

QModelIndex QcepDatasetModel::parent(const QModelIndex &index) const
{
  QModelIndex res = QModelIndex();

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QcepDatasetModel::parent(%s)\n", qPrintable(indexDescription(index)));
  }

  QcepDataObjectPtr childItem = indexedObject(index);

  if (childItem) {
//    if (qcepDebug(DEBUG_DATABROWSER)) {
//      printf("of object %s\n", qPrintable(childItem->get_Name()));
//    }

    QcepDataObjectPtr parentItem = childItem->parentItem();

    if (parentItem) {
      if (parentItem != m_Dataset.data()) {
        res = createIndex(parentItem->indexInParent(), 0, parentItem.data());
      }
    }
  }

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QcepDatasetModel::parent(%s)", qPrintable(indexDescription(index)));
    printf(" = (%s)\n", qPrintable(indexDescription(res)));
  }

  return res;
}

int QcepDatasetModel::rowCount(const QModelIndex &parent) const
{
  int res = 0;

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QcepDatasetModel::rowCount(%s)\n", qPrintable(indexDescription(parent)));
  }

  if (parent.column() <= 0) {
    QcepDataObjectPtr parentItem = indexedObject(parent);

    if (parentItem) {
      int nrows = parentItem->childCount();

      res = nrows;
    } else {
      QcepDatasetPtr ds(m_Dataset);

      if (ds) {
        res = ds->childCount();
      }
    }
  }

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QcepDatasetModel::rowCount(%s)", qPrintable(indexDescription(parent)));
    printf(" = %d\n", res);
  }

  return res;
}

int QcepDatasetModel::columnCount(const QModelIndex &parent) const
{
  int res = 0;

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QcepDatasetModel::columnCount(%s)\n", qPrintable(indexDescription(parent)));
  }

  QcepDataObjectPtr parentItem = indexedObject(parent);

  if (parentItem) {
    res = 3;
  } else {
    res = 3;
  }

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QcepDatasetModel::columnCount(%s)", qPrintable(indexDescription(parent)));
    printf(" = %d\n", res);
  }

  return res;
}

QVariant QcepDatasetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation==Qt::Horizontal) {
    if (role==Qt::DisplayRole) {
      switch (section) {
      case 0:
        return "Name";

      case 1:
        return "Type";

      case 2:
        return "Description";
      }
    }
  }

  return QVariant();
}

QVariant QcepDatasetModel::data(const QModelIndex &index, int role) const
{
  QVariant res = QVariant();

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QcepDatasetModel::data(%s,%d)\n", qPrintable(indexDescription(index)), role);
  }

  if (!index.isValid()) {
    res = QVariant();
  }  else {

    QcepDataObjectPtr object = indexedObject(index);

    if (object) {
      if (role == Qt::DisplayRole) {
        res = object->columnData(index.column());
      } else if (role == Qt::ToolTipRole) {
        res = object->pathName() + "\n" +
            object->columnData(1).toString() + "\n" +
            object->columnData(2).toString();
      }
    }
  }

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QcepDatasetModel::data(%s,%d)", qPrintable(indexDescription(index)), role);
    printf(" = %s\n", qPrintable(res.toString()));
  }

  return res;
}

Qt::ItemFlags QcepDatasetModel::flags(const QModelIndex &index) const
{
  Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

  if (index.isValid()) {
    return defaultFlags | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
  } else {
    return defaultFlags | Qt::ItemIsDropEnabled;
  }
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
      QcepDatasetPtr ds(m_Dataset);

      if (ds) {
        return tr("(%1,%2,\"%3\")")
            .arg(index.row()).arg(index.column())
            .arg(ds->get_Name());
      } else {
        return "";
      }
    }
  }

  return tr("(%1,%2,null)").arg(index.row()).arg(index.column());
}

bool QcepDatasetModel::insertRows(int row, int count, const QModelIndex &parent)
{
  printf("QcepDatasetModel::insertRows(row:%d, count:%d, parent:%s)\n", row, count, qPrintable(indexDescription(parent)));

  return QAbstractItemModel::insertRows(row, count, parent);
}

bool QcepDatasetModel::moveRows
  (const QModelIndex &sourceParent, int sourceRow, int count,
   const QModelIndex &destinationParent, int destinationChild)
{
  printf("QAbstractItemModel::moveRows(sourceParent:%s, sourceRow:%d, count:%d,\n"
         "                             destinationParent:%s, destinationChild:%d)\n",
         qPrintable(indexDescription(sourceParent)), sourceRow, count,
         qPrintable(indexDescription(destinationParent)), destinationChild);

  return QAbstractItemModel::moveRows(sourceParent, sourceRow, count,
                                      destinationParent, destinationChild);
}

bool QcepDatasetModel::removeRows(int row, int count, const QModelIndex &parent)
{
  printf("QcepDatasetModel::removeRows(row:%d, count:%d, parent:%s)\n", row, count, qPrintable(indexDescription(parent)));

  return QAbstractItemModel::removeRows(row, count, parent);
}

void QcepDatasetModel::onDataObjectChanged()
{
//  beginResetModel();
//  endResetModel();

  emit dataChanged(index(0,0), index(rowCount(), columnCount()));
}

QString QcepDatasetModel::groupName(QString path)
{
  QFileInfo info(path);

  if (info.isAbsolute()) {
    return info.dir().absolutePath();
  } else {
    return info.dir().path();
  }
}

QString QcepDatasetModel::objectName(QString path)
{
  QFileInfo info(path);

  return info.fileName();
}

QcepDataObjectPtr      QcepDatasetModel::item(const QModelIndex &index)
{
  return indexedObject(index);
}

QcepDataObjectPtr      QcepDatasetModel::item(QString path)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->item(path);
  } else {
    return QcepDataObjectPtr();
  }
}

QcepDataObjectPtr      QcepDatasetModel::item(int n)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->item(n);
  } else {
    return QcepDataObjectPtr();
  }
}

QcepDataGroupPtr       QcepDatasetModel::group(const QModelIndex &index)
{
  return qSharedPointerDynamicCast<QcepDataGroup>(indexedObject(index));
}

QcepDataGroupPtr       QcepDatasetModel::group(QString path)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->group(path);
  } else {
    return QcepDataGroupPtr();
  }
}

QcepDataGroupPtr       QcepDatasetModel::group(int n)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->group(n);
  } else {
    return QcepDataGroupPtr();
  }
}

QcepDataGroupPtr       QcepDatasetModel::newGroup(QString path)
{
  QcepDataGroupPtr gr = group(path);

  if (gr) {
    return gr;
  } else {
    QcepDataObjectPtr obj = item(path);
    QcepDatasetPtr    ds(m_Dataset);

    if (obj) {
      if (ds) {
        ds->printMessage(tr("Item %1 exists and is not a data group").arg(path));
      }
    } else {
      QcepDataGroupPtr sgr = newGroup(groupName(path));

      if (sgr && ds) {
        QcepDataGroupPtr par = sgr->parentItem();

        QcepDataGroupPtr ng =
            QcepDataGroupPtr(new QcepDataGroup(ds->saver(), objectName(path), sgr.data()));

        if (ng) {
          beginInsertRows(index(sgr), sgr->rowCount(), sgr->rowCount()+1);

          sgr->append(ng);

          endInsertRows();

          return ng;
        }
      }
    }
  }

  return QcepDataGroupPtr();
}

QcepDataArrayPtr       QcepDatasetModel::array(const QModelIndex &index)
{
  return qSharedPointerDynamicCast<QcepDataArray>(indexedObject(index));
}

QcepDataArrayPtr       QcepDatasetModel::array(QString path)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->array(path);
  } else {
    return QcepDataArrayPtr();
  }
}

QcepDataArrayPtr       QcepDatasetModel::array(int n)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->array(n);
  } else {
    return QcepDataArrayPtr();
  }
}

QcepDataArrayPtr       QcepDatasetModel::newArray(QString path, QVector<int> dims)
{
  QcepDataGroupPtr sgr = newGroup(groupName(path));
  QcepDatasetPtr   ds(m_Dataset);

  if (sgr && ds) {
    QcepDataObjectPtr ptr = item(path);
    QcepDataArrayPtr  arr = array(path);

    if (ptr && arr == NULL) {
      ds->printMessage(tr("%1 exists but is not an array").arg(path));
    } else if (arr) {
      ds->printMessage(tr("Array %1 already exists").arg(path));
    } else {
      QcepDataArrayPtr arr(new QcepDataArray(ds->saver(), objectName(path), dims, sgr.data()));

      if (arr) {
        beginInsertRows(index(sgr), sgr->rowCount(), sgr->rowCount()+1);
        sgr -> append(arr);
        endInsertRows();

        return arr;
      }
    }
  }

  return QcepDataArrayPtr();
}

QcepDataColumnPtr      QcepDatasetModel::column(const QModelIndex &index)
{
  return qSharedPointerDynamicCast<QcepDataColumn>(indexedObject(index));
}

QcepDataColumnPtr      QcepDatasetModel::column(QString path)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->column(path);
  } else {
    return QcepDataColumnPtr();
  }
}

QcepDataColumnPtr      QcepDatasetModel::column(int n)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->column(n);
  } else {
    return QcepDataColumnPtr();
  }
}

QcepDataColumnPtr      QcepDatasetModel::newColumn(QString path, int nRows)
{
  QcepDataGroupPtr sgr = newGroup(groupName(path));
  QcepDatasetPtr   ds(m_Dataset);

  if (sgr && ds) {
    QcepDataObjectPtr ptr = item(path);
    QcepDataColumnPtr col = column(path);

    if (ptr && col == NULL) {
      ds->printMessage(tr("%1 exists but is not a data column").arg(path));
    } else if (col) {
      ds->printMessage(tr("Column %1 already exists").arg(path));
    } else {
      QcepDataColumnPtr col(new QcepDataColumn(ds->saver(), objectName(path), nRows, sgr.data()));

      if (col) {
        beginInsertRows(index(sgr), sgr->rowCount(), sgr->rowCount()+1);
        sgr -> append(col);
        endInsertRows();

        return col;
      }
    }
  }

  return QcepDataColumnPtr();
}

QcepDataColumnScanPtr  QcepDatasetModel::columnScan(const QModelIndex &index)
{
  return qSharedPointerDynamicCast<QcepDataColumnScan>(indexedObject(index));
}

QcepDataColumnScanPtr  QcepDatasetModel::columnScan(QString path)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->columnScan(path);
  } else {
    return QcepDataColumnScanPtr();
  }
}

QcepDataColumnScanPtr  QcepDatasetModel::columnScan(int n)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->columnScan(n);
  } else {
    return QcepDataColumnScanPtr();
  }
}

QcepDataColumnScanPtr  QcepDatasetModel::newColumnScan(QString path, int nRows, QStringList cols)
{
  QcepDataGroupPtr sgr = newGroup(groupName(path));
  QcepDatasetPtr   ds(m_Dataset);

  if (sgr && ds) {
    QcepDataObjectPtr     ptr  = item(path);
    QcepDataColumnScanPtr scan = columnScan(path);

    if (ptr && scan == NULL) {
      ds->printMessage(tr("%1 exists but is not a data column scan").arg(path));
    } else if (scan) {
      ds->printMessage(tr("Column Scan %1 already exists").arg(path));
    } else {
      QcepDataColumnScanPtr scan(QcepDataColumnScan::newDataColumnScan(ds->saver(), objectName(path), cols, nRows, sgr.data()));

      if (scan) {
        beginInsertRows(index(sgr), sgr->rowCount(), sgr->rowCount()+1);
        sgr -> append(scan);
        endInsertRows();

        return scan;
      }
    }
  }

  return QcepDataColumnScanPtr();
}

QcepDoubleImageDataPtr QcepDatasetModel::image(const QModelIndex &index)
{
  return qSharedPointerDynamicCast<QcepDoubleImageData>(indexedObject(index));
}

QcepDoubleImageDataPtr QcepDatasetModel::image(QString path)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->image(path);
  } else {
    return QcepDoubleImageDataPtr();
  }
}

QcepDoubleImageDataPtr QcepDatasetModel::image(int n)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->image(n);
  } else {
    return QcepDoubleImageDataPtr();
  }
}

QcepDoubleImageDataPtr QcepDatasetModel::newImage(QString path, int width, int height)
{
  QcepDataGroupPtr sgr = newGroup(groupName(path));
  QcepDatasetPtr   ds(m_Dataset);

  if (sgr && ds) {
    QcepDataObjectPtr      ptr = item(path);
    QcepDoubleImageDataPtr img = image(path);

    if (ptr && img == NULL) {
      ds->printMessage(tr("%1 exists but is not an image").arg(path));
    } else if (img) {
      ds->printMessage(tr("Image %1 already exists").arg(path));
    } else {
      QcepDoubleImageDataPtr img(QcepDoubleImageData::newImage(ds->saver(), objectName(path), width, height, sgr.data()));

      if (img) {
        beginInsertRows(index(sgr), sgr->rowCount(), sgr->rowCount()+1);
        sgr -> append(img);
        endInsertRows();

        return img;
      }
    }
  }

  return QcepDoubleImageDataPtr();
}

QcepIntegratedDataPtr QcepDatasetModel::integratedData(const QModelIndex &index)
{
  return qSharedPointerDynamicCast<QcepIntegratedData>(indexedObject(index));
}

QcepIntegratedDataPtr QcepDatasetModel::integratedData(QString path)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->integratedData(path);
  } else {
    return QcepIntegratedDataPtr();
  }
}

QcepIntegratedDataPtr QcepDatasetModel::integratedData(int n)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->integratedData(n);
  } else {
    return QcepIntegratedDataPtr();
  }
}

QcepIntegratedDataPtr QcepDatasetModel::newIntegratedData(QString path, int sz)
{
  QcepDataGroupPtr sgr = newGroup(groupName(path));
  QcepDatasetPtr   ds(m_Dataset);

  if (sgr && ds) {
    QcepDataObjectPtr     ptr  = item(path);
    QcepIntegratedDataPtr dat  = integratedData(path);

    if (ptr && dat == NULL) {
      ds->printMessage(tr("%1 exists but is not an integrated dataset").arg(path));
    } else if (dat) {
      ds->printMessage(tr("Integrated dataset %1 already exists").arg(path));
    } else {
      QcepIntegratedDataPtr dat(QcepIntegratedData::newIntegratedData(ds->saver(), objectName(path), sz, sgr.data()));

      if (dat) {
        beginInsertRows(index(sgr), sgr->rowCount(), sgr->rowCount()+1);
        sgr -> append(dat);
        endInsertRows();

        return dat;
      }
    }
  }

  return QcepIntegratedDataPtr();
}

void QcepDatasetModel::insertGroup(int atRow, QString name)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    beginInsertRows(QModelIndex(), atRow, atRow+1);

    ds->insert(atRow, QcepDataGroupPtr(new QcepDataGroup(ds->saver(), name, ds.data())));

    endInsertRows();
  }
}

void QcepDatasetModel::append(const QModelIndex &idx, QcepDataObjectPtr obj)
{
  QcepDatasetPtr    ds = m_Dataset;
  QcepDataObjectPtr ob = item(idx);
  QcepDataGroupPtr  gr = group(idx);

  if (ds) {
    if (ob==NULL) {
      QcepDatasetPtr dset(m_Dataset);

      if (dset) {
        beginInsertRows(idx, rowCount(idx), rowCount(idx)+1);
        dset->append(obj);
        endInsertRows();
      }
    } else if (gr==NULL) {
      ds->printMessage("Containing object is not a container");
    } else {
      beginInsertRows(index(gr), gr->rowCount(), gr->rowCount()+1);
      gr->append(obj);
      endInsertRows();
    }
  }
}

void QcepDatasetModel::append(QString path, QcepDataObjectPtr obj)
{
  QcepDataGroupPtr sgr = newGroup(groupName(path));
  QcepDatasetPtr   ds(m_Dataset);

  if (sgr && ds) {
    QcepDataObjectPtr ptr = item(path);

    if (ptr) {
      ds->printMessage(tr("%1 exists already").arg(path));
    } else {
      beginInsertRows(index(sgr), sgr->rowCount(), sgr->rowCount()+1);
      sgr->append(obj);
      endInsertRows();
    }
  }
}

void QcepDatasetModel::remove(QcepDataObjectPtr obj)
{
  if (obj) {
    QcepDataGroupPtr  par = obj->parentItem();
    int n = obj->indexInParent();

    if (par && n >= 0 && n < par->childCount()) {
      beginRemoveRows(index(par), n, n);
      par->remove(n);
      endRemoveRows();
    }
  }
}

void QcepDatasetModel::remove(const QModelIndex &index)
{
  QcepDataObjectPtr obj = indexedObject(index);

  remove(obj);
}

void QcepDatasetModel::remove(QString path)
{
  QcepDataObjectPtr obj = item(path);

  remove(obj);
}

void QcepDatasetModel::concatenateData(const QModelIndex &dest, const QModelIndexList &src)
{
  for (int i=0; i<src.count(); i++) {
    concatenateData(dest, src.at(i));
  }
}

void QcepDatasetModel::concatenateData(const QModelIndex &dest, const QModelIndex &src)
{
  QcepDataColumnPtr     col  = column(dest);

  if (col) {
    col->concat(column(src));
  } else {
    QcepDataColumnScanPtr scan = columnScan(dest);

    if (scan) {
      scan->concat(columnScan(src));
    }
  }
}

void QcepDatasetModel::concatenateData(const QModelIndex &dest, double val)
{
  QcepDataColumnPtr     col  = column(dest);

  if (col) {
    col->concat(val);
//  } else {
//    QcepDataColumnScanPtr scan = columnScan(dest);

//    if (scan) {
//      scan->concat(val);
//    }
  }
}

void QcepDatasetModel::addData(const QModelIndex &dest, const QModelIndexList &src)
{
  for (int i=0; i<src.count(); i++) {
    addData(dest, src.at(i));
  }
}

void QcepDatasetModel::addData(const QModelIndex &dest, const QModelIndex &src)
{
  QcepDataColumnPtr     col  = column(dest);

  if (col) {
    col->add(column(src));
  } else {
    QcepDoubleImageDataPtr img = image(dest);

    if (img) {
      img->add(image(src));
    }
  }
}

void QcepDatasetModel::addData(const QModelIndex &dest, double val)
{
  QcepDataColumnPtr     col  = column(dest);

  if (col) {
    col->add(val);
  } else {
    QcepDoubleImageDataPtr img = image(dest);

    if (img) {
      img->add(val);
    }
  }
}

void QcepDatasetModel::subtractData(const QModelIndex &dest, const QModelIndexList &src)
{
  for (int i=0; i<src.count(); i++) {
    subtractData(dest, src.at(i));
  }
}

void QcepDatasetModel::subtractData(const QModelIndex &dest, const QModelIndex &src)
{
  QcepDataColumnPtr     col  = column(dest);

  if (col) {
    col->subtract(column(src));
  } else {
    QcepDoubleImageDataPtr img = image(dest);

    if (img) {
      img->subtract(image(src));
    }
  }
}

void QcepDatasetModel::subtractData(const QModelIndex &dest, double val)
{
  QcepDataColumnPtr     col  = column(dest);

  if (col) {
    col->subtract(val);
  } else {
    QcepDoubleImageDataPtr img = image(dest);

    if (img) {
      img->subtract(val);
    }
  }
}

void QcepDatasetModel::multiplyData(const QModelIndex &dest, const QModelIndexList &src)
{
  for (int i=0; i<src.count(); i++) {
    multiplyData(dest, src.at(i));
  }
}

void QcepDatasetModel::multiplyData(const QModelIndex &dest, const QModelIndex &src)
{
  QcepDataColumnPtr     col  = column(dest);

  if (col) {
    col->multiply(column(src));
  } else {
    QcepDoubleImageDataPtr img = image(dest);

    if (img) {
      img->multiply(image(src));
    }
  }
}

void QcepDatasetModel::multiplyData(const QModelIndex &dest, double val)
{
  QcepDataColumnPtr     col  = column(dest);

  if (col) {
    col->multiply(val);
  } else {
    QcepDoubleImageDataPtr img = image(dest);

    if (img) {
      img->multiply(val);
    }
  }
}

void QcepDatasetModel::divideData(const QModelIndex &dest, const QModelIndexList &src)
{
  for (int i=0; i<src.count(); i++) {
    divideData(dest, src.at(i));
  }
}

void QcepDatasetModel::divideData(const QModelIndex &dest, const QModelIndex &src)
{
  QcepDataColumnPtr     col  = column(dest);

  if (col) {
    col->divide(column(src));
  } else {
    QcepDoubleImageDataPtr img = image(dest);

    if (img) {
      img->divide(image(src));
    }
  }
}

void QcepDatasetModel::divideData(const QModelIndex &dest, double val)
{
  QcepDataColumnPtr     col  = column(dest);

  if (col) {
    col->divide(val);
  } else {
    QcepDoubleImageDataPtr img = image(dest);

    if (img) {
      img->divide(val);
    }
  }
}

void QcepDatasetModel::integrate(const QModelIndex &src)
{
  QcepDoubleImageDataPtr   img = image(src);
  QcepDataProcessorBasePtr proc(m_Processor);

  if (img && proc) {
    QcepDataObjectPtr res = proc->integrate(img);

    if (res) {
      res->setNameAndSuffix(img->get_Name(), "integ");

      append(parent(src), res);
    }
  }
}

void QcepDatasetModel::polarTransform(const QModelIndex &src)
{
  QcepDoubleImageDataPtr   img = image(src);
  QcepDataProcessorBasePtr proc(m_Processor);

  if (img && proc) {
    QcepDataObjectPtr res = proc->polarTransform(img);

    if (res) {
      res->setNameAndSuffix(img->get_Name(), "xform");

      append(parent(src), res);
    }
  }
}

void QcepDatasetModel::polarIntegrate(const QModelIndex &src)
{
  QcepDoubleImageDataPtr   img = image(src);
  QcepDataProcessorBasePtr proc(m_Processor);

  if (img && proc) {
    QcepDataObjectPtr res = proc->polarIntegrate(img);

    if (res) {
      res->setNameAndSuffix(img->get_Name(), "pinteg");

      append(parent(src), res);
    }
  }
}

bool QcepDatasetModel::integrateParameters()
{
  bool res = false;
  QcepDataProcessorBasePtr proc(m_Processor);

  if (proc) {
    res = proc->integrateParameters();
  }

  return res;
}

bool QcepDatasetModel::polarTransformParameters()
{
  bool res = false;
  QcepDataProcessorBasePtr proc(m_Processor);

  if (proc) {
    res = proc->polarTransformParameters();
  }

  return res;
}

bool QcepDatasetModel::polarIntegrateParameters()
{
  bool res = false;
  QcepDataProcessorBasePtr proc(m_Processor);

  if (proc) {
    res = proc->polarIntegrateParameters();
  }

  return res;
}
