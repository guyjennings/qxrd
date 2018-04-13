#ifndef QCEPPOLYGONPOINTSMODEL_H
#define QCEPPOLYGONPOINTSMODEL_H

#include "qceplib_global.h"
#include <QAbstractTableModel>
#include "qceproipolygon-ptr.h"
#include <QPolygonF>

class QItemSelectionModel;

class QCEP_EXPORT QcepPolygonPointsModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  explicit QcepPolygonPointsModel(QPolygonF poly, QObject *parent = 0);

  // Basic functionality:
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

  bool setData(const QModelIndex &index, const QVariant &value, int role) override;

  Qt::ItemFlags flags(const QModelIndex &index) const override;

  QPointF polygonPoint(int i);

  void addPoint(QItemSelectionModel *sel);
  void delPoint(QItemSelectionModel *sel);

  enum {
    NumCol,
    XCol,
    YCol,
    ColCount
  };

private:
  void setPoly(int n, int col, double v);

private:
  QPolygonF m_Polygon;
};

#endif // QCEPPOLYGONPOINTSMODEL_H
