#ifndef QCEPSCATTERPLOTGRAPHMODEL_H
#define QCEPSCATTERPLOTGRAPHMODEL_H

#include <QAbstractTableModel>
#include "qcepdataobject-ptr.h"

class QcepScatterPlotGraphModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  QcepScatterPlotGraphModel(QcepDataObjectWPtr object);

  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  QVariant data(const QModelIndex &index, int role) const;

  void toggle(const QModelIndex &index);

  bool getX(int row) const;
  bool getY(int row) const;
  bool getY2(int row) const;

  void toggleX(int row);
  void toggleY(int row);
  void toggleY2(int row);

  int checkState(bool val) const;

signals:

private:
  QcepDataObjectWPtr m_Object;
  QVector<bool> m_X, m_Y, m_Y2;
};

#endif // QCEPSCATTERPLOTGRAPHMODEL_H
