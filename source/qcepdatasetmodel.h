#ifndef QCEPDATASETMODEL_H
#define QCEPDATASETMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include "qcepdataset-ptr.h"
#include "qcepdataobject-ptr.h"

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

  QcepDataObject *indexedObject(const QModelIndex& index) const;

private slots:
  void onDataObjectChanged();

private:
  QString indexDescription(const QModelIndex& index) const;

private:
  QcepDatasetPtr m_Dataset;
};

#endif // QCEPDATASETMODEL_H
