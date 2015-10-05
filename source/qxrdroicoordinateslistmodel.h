#ifndef QXRDROICOORDINATESLISTMODEL_H
#define QXRDROICOORDINATESLISTMODEL_H

#include <QAbstractListModel>
#include "qxrdroicoordinateslist-ptr.h"
#include "qxrdroicoordinates-ptr.h"

class QxrdROICoordinatesListModel : public QAbstractListModel
{
public:
  QxrdROICoordinatesListModel(QxrdROICoordinatesListWPtr coords);

  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;

  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  Qt::ItemFlags flags(const QModelIndex &index) const;

  bool setData(const QModelIndex &index, const QVariant &value, int role);

  void append(QxrdROICoordinatesPtr coords);
  void removeROI(int row);

  void moveROIDown(int row);
  void moveROIUp(int row);

  void editROI(int row);

  void moveROICenter(int row, double x, double y);

  QxrdROICoordinatesPtr roi(int row) const;
  void setRoi(int row, QxrdROICoordinatesPtr c);

  enum {
    NumCol,
    ValueCol,
    TypeCol,
    CenterXCol,
    CenterYCol,
    WidthCol,
    HeightCol,
    ColCount
  };

signals:

public slots:

private:
  QxrdROICoordinatesListWPtr m_ROICoordinates;
};

#endif // QXRDROICOORDINATESLISTMODEL_H
