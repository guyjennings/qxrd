#ifndef QCEPDATASETMODEL_H
#define QCEPDATASETMODEL_H

#include <QAbstractItemModel>
#include "qcepdataset-ptr.h"
#include "qcepdataset.h"
#include "qcepdataobject-ptr.h"
#include "qcepdataobject.h"
#include "qcepdatagroup-ptr.h"
#include "qcepdatagroup.h"
#include "qcepdataarray-ptr.h"
#include "qcepdataarray.h"
#include "qcepdatacolumn-ptr.h"
#include "qcepdatacolumn.h"
#include "qcepdatacolumnscan-ptr.h"
#include "qcepdatacolumnscan.h"
#include "qxrdimagedata-ptr.h"
#include "qxrdimagedata.h"

class QcepDatasetModel : public QAbstractItemModel
{
  Q_OBJECT

public:
  QcepDatasetModel(QcepDatasetPtr ds);

public:
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &index) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  Qt::ItemFlags flags(const QModelIndex &index) const;

  QcepDataObjectPtr indexedObject(const QModelIndex& index) const;

  QStringList mimeTypes() const;
  QMimeData  *mimeData(const QModelIndexList &indexes) const;

public slots:
  QcepDataObjectPtr      item(const QModelIndex &index);
  QcepDataObjectPtr      item(QString path);

  QcepDataGroupPtr       group(const QModelIndex &index);
  QcepDataGroupPtr       group(QString path);
  QcepDataGroupPtr       newGroup(QString path);

  QcepDataArrayPtr       array(const QModelIndex &index);
  QcepDataArrayPtr       array(QString path);
  QcepDataArrayPtr       newArray(QString path, QVector<int> dims);

  QcepDataColumnPtr      column(const QModelIndex &index);
  QcepDataColumnPtr      column(QString path);
  QcepDataColumnPtr      newColumn(QString path, int nRows);

  QcepDataColumnScanPtr  columnScan(const QModelIndex &index);
  QcepDataColumnScanPtr  columnScan(QString path);
  QcepDataColumnScanPtr  newColumnScan(QString path, int nRows, QStringList cols);

  QxrdDoubleImageDataPtr image(const QModelIndex &index);
  QxrdDoubleImageDataPtr image(QString path);
  QxrdDoubleImageDataPtr newImage(QString path);

  void                   append(const QModelIndex &index, QcepDataObjectPtr obj);
  void                   append(QString path, QcepDataObjectPtr obj);
  void                   remove(const QModelIndex &index);
  void                   remove(QString path);

private slots:
  void onDataObjectChanged();

private:
  QString indexDescription(const QModelIndex& index) const;

private:
  QcepDatasetPtr m_Dataset;
};

#endif // QCEPDATASETMODEL_H
