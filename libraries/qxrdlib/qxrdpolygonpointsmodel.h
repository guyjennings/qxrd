#ifndef QXRDPOLYGONPOINTSMODEL_H
#define QXRDPOLYGONPOINTSMODEL_H

#include "qxrdlib_global.h"
#include <QAbstractTableModel>
#include "qxrdroipolygon-ptr.h"
#include <QPolygonF>

class QItemSelectionModel;

class QXRD_EXPORT QxrdPolygonPointsModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  explicit QxrdPolygonPointsModel(QPolygonF poly, QObject *parent = 0);

  // Basic functionality:
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

  bool setData(const QModelIndex &index, const QVariant &value, int role);

  Qt::ItemFlags flags(const QModelIndex &index) const;

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

#endif // QXRDPOLYGONPOINTSMODEL_H
