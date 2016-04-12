#include "qcepdatasetmodel.h"
#include "qcepdataobject.h"
#include "qcepdataobject-ptr.h"
#include "qcepexperiment.h"
#include "qcepdataset.h"
#include "qcepdebug.h"
#include "qcepdataprocessorbase.h"
#include "qcepallocator.h"
#include <QMimeData>
#include <stdio.h>
#include <QFileInfo>
#include <QDir>

QcepDatasetModel::QcepDatasetModel(QcepExperimentWPtr expt, QcepDataProcessorBaseWPtr proc, QcepDatasetWPtr ds) :
  m_Experiment(expt),
  m_Processor(proc),
  m_Dataset(ds)
{
  if (m_Dataset == NULL) {
    m_Dataset = QcepAllocator::newDataset("model");
  }

  connect(m_Dataset.data(), SIGNAL(dataObjectChanged()), this, SLOT(onDataObjectChanged()));
}

void QcepDatasetModel::printMessage(QString msg, QDateTime dt) const
{
  QcepExperimentPtr expt(m_Experiment);

  if (expt) {
    expt->printMessage(msg, dt);
  }
}

QcepExperimentWPtr QcepDatasetModel::experiment()
{
  return m_Experiment;
}

QcepDataProcessorBaseWPtr QcepDatasetModel::processor()
{
  return m_Processor;
}

QcepDataObjectPtr QcepDatasetModel::indexedObject(const QModelIndex &index) const
{
  QcepDataObjectPtr res;

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printMessage(tr("QcepDatasetModel::indexedObject(%1)").arg(indexDescription(index)));
  }

  if (index.isValid()) {
    QcepDataObject *obj = static_cast<QcepDataObject*>(index.internalPointer());

    if (obj) {
      res = obj->sharedFromThis();

      if (!res) {
        printMessage("QcepDatasetModel::indexedObject returns NULL\n");
      }
    }
  } else {
    res = m_Dataset;
  }

  if (qcepDebug(DEBUG_DATABROWSER)) {
    if (res) {
      printMessage(tr("QcepDatasetModel::indexedObject(%1) = %2")
                   .arg(indexDescription(index))
                   .arg(res->get_Name()));
    } else {
      printMessage(tr("QcepDatasetModel::indexedObject(%1) = NULL")
                   .arg(indexDescription(index)));
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

QString QcepDatasetModel::pathName(const QModelIndex &index)
{
  QcepDataObjectPtr obj = item(index);

  if (obj) {
    return obj->pathName();
  } else {
    return "NULL";
  }
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

  return 3 /*res*/;
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
      if (role == Qt::DisplayRole || role == Qt::EditRole) {
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

bool QcepDatasetModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (qcepDebug(DEBUG_DATABROWSER)) {
    printMessage(tr("QcepDatasetModel::setData(%1,%2,%3)")
                 .arg(indexDescription(index)).arg(value.toString()).arg(role));
  }

  if (index.isValid() && role == Qt::EditRole) {
    QcepDataObjectPtr obj = indexedObject(index);

    if (obj && index.column() == 0) {
      obj->set_Name(value.toString());


      emit dataChanged(index, index);

      return true;
    }
  }

  return false;
}

Qt::ItemFlags QcepDatasetModel::flags(const QModelIndex &index) const
{
  Qt::ItemFlags flags = Qt::NoItemFlags;

  if (index.isValid()) {
    flags |= Qt::ItemIsEnabled;
    flags |= Qt::ItemIsSelectable;

    QcepDataObjectPtr obj = indexedObject(index);

    if (obj) {
      flags |= Qt::ItemIsDragEnabled;

      if (index.column() == 0) {
        flags |= Qt::ItemIsEditable;
      }

      QcepDataGroupPtr grp = qSharedPointerDynamicCast<QcepDataGroup>(obj);

      if (grp) { // Is a container...
        flags |= Qt::ItemIsDropEnabled;
      }
    }
  } else {
    flags |= Qt::ItemIsDropEnabled;
  }

  return flags;
}

Qt::DropActions QcepDatasetModel::supportedDropActions() const
{
  return Qt::CopyAction | Qt::MoveAction;
}

QStringList QcepDatasetModel::mimeTypes() const
{
  QStringList types;

  types << QcepDataObject::mimeType();
  types << "application/vnd.text.list";
  types << QAbstractItemModel::mimeTypes();

  return types;
}

QMimeData *QcepDatasetModel::mimeData(const QModelIndexList &indexes) const
{
  QMimeData *mimeData = QAbstractItemModel::mimeData(indexes);

  QString    textData;
  QByteArray objectData;
  QDataStream dataStream(&objectData, QIODevice::WriteOnly);

  foreach (const QModelIndex &index, indexes) {
    if (qcepDebug(DEBUG_DRAGDROP) || qcepDebug(DEBUG_DATABROWSER)) {
      printMessage(tr("Mime data for %1").arg(indexDescription(index)));
    }

    if (index.isValid()) {
      if (index.column() != 0) {
        textData += "\t";
      }
      textData += data(index, Qt::DisplayRole).toString();
      textData += "\n";

      if (index.column() == 0) {
        QcepDataObjectPtr obj = indexedObject(index);

        quint64 objn = (quint64) obj.data();

        dataStream << objn
                   << index.row()
                   << index.column()
                   /*<< data(index, Qt::DisplayRole).toString()*/;
      }
    }
  }

//  printMessage(tr("QcepDatasetModel::mimeData = %1").arg((QString) objectData));

  mimeData->setData(QcepDataObject::mimeType(), objectData);
  mimeData->setText(textData);

  return mimeData;
}

bool QcepDatasetModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
  bool res = false;

  if (qcepDebug(DEBUG_DRAGDROP) || qcepDebug(DEBUG_DATABROWSER)) {
    printMessage(tr("dropMimeData into row:%1 col:%2 parent:%3 action %4")
                 .arg(row).arg(column).arg(indexDescription(parent)).arg(action));
  }

  if (data -> hasFormat(QcepDataObject::mimeType())) {
    QByteArray encodedData = data->data(QcepDataObject::mimeType());
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QHash<qint64, QMap<int,QHash<int,QString> > > newItems;

    QcepDataObjectPtr        parentobj = item(parent);

    QList<int>               rows;
    QList<QcepDataObjectPtr> objs;
    int                      nsameparent = 0;

    while (!stream.atEnd()) {
      quint64 id;
      int srcrow;
      int srccolumn;
      stream >> id >> srcrow >> srccolumn;

      QcepDataObject *obj = (QcepDataObject*) id;

      if (obj) {
        if (qcepDebug(DEBUG_DRAGDROP) || qcepDebug(DEBUG_DATABROWSER)) {
          printMessage(tr("dropMimeData: id:%1 row:%2 col:%3 path:%4")
                       .arg(id).arg(srcrow).arg(srccolumn).arg(obj->pathName()));
        }

        QcepDataObjectPtr objp = obj->sharedFromThis();

        if (objp) {
          rows.append(srcrow);
          objs.append(objp);

          if (objp->parentItem() == parentobj) {
            if (qcepDebug(DEBUG_DRAGDROP) || qcepDebug(DEBUG_DATABROWSER)) {
              printMessage(tr("item from same parent as destination FromRow: %1, ToRow: %2").arg(srcrow).arg(row));
            }

            if (srcrow < row) {
              nsameparent++;
            }
          }
        }
      }
    }

    emit layoutAboutToBeChanged();

    for (int i=0; i<objs.count(); i++) {
      remove(objs[i]);
//      removeRows(rows[i], 1, index(objs[i]->parentItem()));
    }

    if (row >= 0) {
      for (int i=0; i<objs.count(); i++) {
        insert(row+i-nsameparent, parent, objs[i]);
      }
    } else {
      for (int i=0; i<objs.count(); i++) {
        append(parent, objs[i]);
      }
    }

    emit layoutChanged();

    res = true;
//  } else {
//    return /*false;*/ QAbstractItemModel::dropMimeData(data, action, row, column, parent);
  }

  return res;
}

QString QcepDatasetModel::indexDescription(const QModelIndex &index) const
{
  if (index.isValid()) {
    QcepDataObjectPtr obj = indexedObject(index);
    QcepDataObjectPtr par = indexedObject(parent(index));

    if (obj) {
      if (par) {
        return tr("\"%1\" : Row %2, Col %3 of \"%4\"").arg(obj->get_Name()).arg(index.row()).arg(index.column()).arg(par->get_Name());
      } else {
        return tr("\"%1\" : Row %2, Col %3 of \"/\"").arg(obj->get_Name()).arg(index.row()).arg(index.column());
      }
    }
  }

  return tr("(%1,%2,null)").arg(index.row()).arg(index.column());
}

bool QcepDatasetModel::insertRows(int row, int count, const QModelIndex &parent)
{
  printMessage(tr("QcepDatasetModel::insertRows(row:%1, count:%2, parent:%3)")
               .arg(row).arg(count).arg(indexDescription(parent)));

  return QAbstractItemModel::insertRows(row, count, parent);
}

bool QcepDatasetModel::moveRows
  (const QModelIndex &sourceParent, int sourceRow, int count,
   const QModelIndex &destinationParent, int destinationChild)
{
  printMessage(tr("QAbstractItemModel::moveRows(sourceParent:%1, sourceRow:%2, count:%3,\n"
         "                             destinationParent:%4, destinationChild:%5)")
         .arg(indexDescription(sourceParent)).arg(sourceRow).arg(count)
         .arg(indexDescription(destinationParent)).arg(destinationChild));

  return QAbstractItemModel::moveRows(sourceParent, sourceRow, count,
                                      destinationParent, destinationChild);
}

bool QcepDatasetModel::removeRows(int row, int count, const QModelIndex &parent)
{
  bool res = false;
  QcepDataGroupPtr grp = group(parent);

  if (grp) {

    beginRemoveRows(parent, row, row+count+1);

    for (int i=0; i<count; i++) {
      grp->remove(row);
    }

    endRemoveRows();

    res = true;
  }

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printMessage(tr("QcepDatasetModel::removeRows(row:%1, count:%2, parent:%3)")
                 .arg(row).arg(count).arg(indexDescription(parent)));
  }

  return res;
}

bool QcepDatasetModel::insert(int row, const QModelIndex &parent, QcepDataObjectPtr obj)
{
  bool res = false;

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printMessage(tr("QcepDatasetModel::insertAfter row:%1 parent:%2 object:%3")
                 .arg(row).arg(indexDescription(parent)).arg(obj->pathName()));
  }

  QcepDataGroupPtr grp = group(parent);

  if (grp) {
    beginInsertRows(parent, row, row);

    grp->insert(row, obj);

    endInsertRows();

    res = true;
  }

  return res;
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
            QcepAllocator::newGroup(objectName(path));

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

QcepDataGroupPtr       QcepDatasetModel::newGroup(const QModelIndex &index, QString name)
{
  QcepDataGroupPtr res;
  QcepDataGroupPtr grp = group(index);
  QcepDatasetPtr   ds(m_Dataset);

  if (grp && ds) {
    QcepDataGroupPtr ng = QcepAllocator::newGroup(name);

    if (ng) {
      beginInsertRows(index, grp->rowCount(), grp->rowCount()+1);
      grp->append(ng);
      endInsertRows();

      res = ng;
    }
  }

  return res;
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
      printMessage(tr("%1 exists but is not an array").arg(path));
    } else if (arr) {
      printMessage(tr("Array %1 already exists").arg(path));
    } else {
      QcepDataArrayPtr arr =
          QcepAllocator::newArray(objectName(path), dims, QcepAllocator::NullIfNotAvailable);

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

QcepDataArrayPtr       QcepDatasetModel::newArray(const QModelIndex &index, QString name, QVector<int> dims)
{
  QcepDataArrayPtr res;
  QcepDataGroupPtr grp = group(index);
  QcepDatasetPtr   ds(m_Dataset);

  if (grp && ds) {
    QcepDataArrayPtr na =
        QcepAllocator::newArray(name, dims, QcepAllocator::NullIfNotAvailable);

    if (na) {
      beginInsertRows(index, grp->rowCount(), grp->rowCount()+1);
      grp->append(na);
      endInsertRows();

      res = na;
    }
  }

  return res;
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
      printMessage(tr("%1 exists but is not a data column").arg(path));
    } else if (col) {
      printMessage(tr("Column %1 already exists").arg(path));
    } else {
      QcepDataColumnPtr col
          = QcepAllocator::newColumn(objectName(path), nRows, QcepAllocator::NullIfNotAvailable);

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

QcepDataColumnPtr       QcepDatasetModel::newColumn(const QModelIndex &index, QString name, int nRows)
{
  QcepDataColumnPtr res;
  QcepDataGroupPtr grp = group(index);
  QcepDatasetPtr   ds(m_Dataset);

  if (grp && ds) {
    QcepDataColumnPtr nc =
        QcepAllocator::newColumn(name, nRows, QcepAllocator::NullIfNotAvailable);

    if (nc) {
      beginInsertRows(index, grp->rowCount(), grp->rowCount()+1);
      grp->append(nc);
      endInsertRows();

      res = nc;
    }
  }

  return res;
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

QcepDataColumnScanPtr  QcepDatasetModel::newColumnScan(QString path, QStringList cols, int nRows)
{
  QcepDataGroupPtr sgr = newGroup(groupName(path));
  QcepDatasetPtr   ds(m_Dataset);

  if (sgr && ds) {
    QcepDataObjectPtr     ptr  = item(path);
    QcepDataColumnScanPtr scan = columnScan(path);

    if (ptr && scan == NULL) {
      printMessage(tr("%1 exists but is not a data column scan").arg(path));
    } else if (scan) {
      printMessage(tr("Column Scan %1 already exists").arg(path));
    } else {
      QcepDataColumnScanPtr scan = QcepAllocator::newColumnScan(objectName(path), cols, nRows, QcepAllocator::NullIfNotAvailable);

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

QcepDataColumnScanPtr       QcepDatasetModel::newColumnScan(const QModelIndex &index,
                                                            QString name,
                                                            QStringList cols,
                                                            int nRows)
{
  QcepDataColumnScanPtr res;
  QcepDataGroupPtr grp = group(index);
  QcepDatasetPtr   ds(m_Dataset);

  if (grp && ds) {
    QcepDataColumnScanPtr ns =
        QcepAllocator::newColumnScan(name, cols, nRows, QcepAllocator::NullIfNotAvailable);

    if (ns) {
      beginInsertRows(index, grp->rowCount(), grp->rowCount()+1);
      grp->append(ns);
      endInsertRows();

      res = ns;
    }
  }

  return res;
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
      printMessage(tr("%1 exists but is not an image").arg(path));
    } else if (img) {
      printMessage(tr("Image %1 already exists").arg(path));
    } else {
      QcepDoubleImageDataPtr img
          = QcepAllocator::newDoubleImage(objectName(path), width, height, QcepAllocator::NullIfNotAvailable);

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

QcepDoubleImageDataPtr QcepDatasetModel::newImage(const QModelIndex &index, QString name, int width, int height)
{
  QcepDoubleImageDataPtr res;
  QcepDataGroupPtr grp = group(index);
  QcepDatasetPtr   ds(m_Dataset);

  if (grp && ds) {
    QcepDoubleImageDataPtr ni =
        QcepAllocator::newDoubleImage(name, width, height, QcepAllocator::NullIfNotAvailable);

    if (ni) {
      beginInsertRows(index, grp->rowCount(), grp->rowCount()+1);
      grp->append(ni);
      endInsertRows();

      res = ni;
    }
  }

  return res;
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
      printMessage(tr("%1 exists but is not an integrated dataset").arg(path));
    } else if (dat) {
      printMessage(tr("Integrated dataset %1 already exists").arg(path));
    } else {
      QcepIntegratedDataPtr dat =
          QcepAllocator::newIntegratedData(objectName(path), sz, QcepAllocator::NullIfNotAvailable);

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

QcepIntegratedDataPtr QcepDatasetModel::newIntegratedData(const QModelIndex &index, QString name, int sz)
{
  QcepIntegratedDataPtr res;
  QcepDataGroupPtr grp = group(index);
  QcepDatasetPtr   ds(m_Dataset);

  if (grp && ds) {
    QcepIntegratedDataPtr ni =
        QcepAllocator::newIntegratedData(name, sz, QcepAllocator::NullIfNotAvailable);

    if (ni) {
      beginInsertRows(index, grp->rowCount(), grp->rowCount()+1);
      grp->append(ni);
      endInsertRows();

      res = ni;
    }
  }

  return res;
}

void QcepDatasetModel::insertGroup(int atRow, QString name)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    beginInsertRows(QModelIndex(), atRow, atRow+1);

    ds->insert(atRow, QcepAllocator::newGroup(name));

    endInsertRows();
  }
}

QModelIndex QcepDatasetModel::append(const QModelIndex &idx, QcepDataObjectPtr obj)
{
  QModelIndex       res;
  QcepDatasetPtr    ds = m_Dataset;
  QcepDataGroupPtr  gr = group(idx);

  if (ds) {
    if (gr==NULL) {
      QcepDatasetPtr dset(m_Dataset);

      if (dset) {
        int rc = rowCount(idx);
        beginInsertRows(idx, rc, rc+1);
        dset->append(obj);
        endInsertRows();

        res = index(obj);
      }
    } else if (gr==NULL) {
      ds->printMessage("Containing object is not a container");
    } else {
      int rc = gr->rowCount();
      beginInsertRows(index(gr), rc, rc+1);
      gr->append(obj);
      endInsertRows();

      res = index(obj);
    }
  }

  if (item(res) != obj) {
    printMessage("result of append is wrong");
  }

  return res;
}

QModelIndex QcepDatasetModel::append(QString path, QcepDataObjectPtr obj)
{
  QModelIndex       res;
  QcepDataGroupPtr sgr = newGroup(groupName(path));
  QcepDatasetPtr   ds(m_Dataset);

  if (sgr && ds) {
    QcepDataObjectPtr ptr = item(path);

    if (ptr) {
      ds->printMessage(tr("%1 exists already").arg(path));
    } else {
      int rc = sgr->rowCount();
      beginInsertRows(index(sgr), rc, rc+1);
      sgr->append(obj);
      endInsertRows();

      res = index(obj);
    }
  }

  if (item(res) != obj) {
    printMessage("result of append is wrong");
  }

  return res;
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
