#include "qcepscatterplotgraphmodel.h"
#include "qcepdatagroup-ptr.h"
#include "qcepdatagroup.h"
#include "qcepintegrateddata-ptr.h"
#include "qcepintegrateddata.h"

QcepScatterPlotGraphModel::QcepScatterPlotGraphModel(QcepDataObjectWPtr object) :
  m_Object(object)
{
  m_X.resize(1000);
  m_Y.resize(1000);
  m_Y2.resize(1000);
}

int QcepScatterPlotGraphModel::rowCount(
    const QModelIndex &parent) const
{
  int res = 0;

  QcepDataObjectPtr p(m_Object);

  if (p) {
    QcepDataGroupPtr dg = qSharedPointerDynamicCast<QcepDataGroup>(p);

    if (dg) {
      res = dg->childCount();
    } else {
      QcepIntegratedDataPtr integ = qSharedPointerDynamicCast<QcepIntegratedData>(p);

      if (integ) {
        res = 2;
      }
    }
  }

  return res;
}

int QcepScatterPlotGraphModel::columnCount(
    const QModelIndex &parent) const
{
  return 4;
}

QVariant QcepScatterPlotGraphModel::headerData(
    int section, Qt::Orientation orientation, int role) const
{
  QVariant res;

  if (orientation==Qt::Horizontal && role==Qt::DisplayRole) {
    switch (section) {
    case 0:
      res = "Column";
      break;

    case 1:
      res = "X?";
      break;

    case 2:
      res = "Y?";
      break;

    case 3:
      res = "Y2?";
      break;
    }
  }

  return res;
}

QVariant QcepScatterPlotGraphModel::data(
    const QModelIndex &index, int role) const
{
  int r = index.row();
  int c = index.column();

  QVariant res;

  QcepDataObjectPtr p(m_Object);

  if (p) {
    QcepDataGroupPtr dg = qSharedPointerDynamicCast<QcepDataGroup>(p);

    if (dg) {
      QcepDataObjectPtr o = dg->item(r);

      if (o) {
        if (role == Qt::DisplayRole && c == 0) {
          res = o->objectName();
        } else if (role == Qt::CheckStateRole) {
          if (c == 1) {
            res = checkState(getX(r));
          } else if (c == 2) {
            res = checkState(getY(r));
          } else if (c == 3) {
            res = checkState(getY2(r));
          }
        }
      }
    } else {
      QcepIntegratedDataPtr integ = qSharedPointerDynamicCast<QcepIntegratedData>(p);

      if (integ) {
        if (role == Qt::DisplayRole && c == 0) {
            switch(r) {
            case 0:
              res = integ->get_XUnitsLabel();
              break;

            case 1:
              res = "Intensity";
              break;
            }
        } else if (role == Qt::CheckStateRole) {
          if (c == 1) {
            res = checkState(getX(r));
          } else if (c == 2) {
            res = checkState(getY(r));
          } else if (c == 3) {
            res = checkState(getY2(r));
          }
        }
      }
    }
  }

  return res;
}

void QcepScatterPlotGraphModel::toggle(const QModelIndex &index)
{
  int r = index.row();
  int c = index.column();
  int nr = rowCount(QModelIndex());
  int nc = columnCount(QModelIndex());

  if (r >= 0 && r < nr) {
    if (c == 1) { // 'X' column:
      toggleX(r);
    } else if (c == 2) { // 'Y' column
      toggleY(r);
    } else if (c == 3) { // 'Y2' column
      toggleY2(r);
    }
  }
}

bool QcepScatterPlotGraphModel::getX(int row) const
{
  return m_X.value(row);
}

bool QcepScatterPlotGraphModel::getY(int row) const
{
  return m_Y.value(row);
}

bool QcepScatterPlotGraphModel::getY2(int row) const
{
  return m_Y2.value(row);
}

void QcepScatterPlotGraphModel::toggleX(int row)
{
  int nr = rowCount(QModelIndex());

  if (row >= 0 && row < nr) {
    for (int i=0; i<nr; i++) {
      m_X[i] = false;
    }

    m_X[row] = true;

    emit dataChanged(index(0,1), index(nr,1));
  }
}

void QcepScatterPlotGraphModel::toggleY(int row)
{
  int nr = rowCount(QModelIndex());

  if (row >= 0 && row < nr) {
    m_Y[row] = !m_Y[row];

    emit dataChanged(index(row,2), index(row,2));
  }
}

void QcepScatterPlotGraphModel::toggleY2(int row)
{
  int nr = rowCount(QModelIndex());

  if (row >= 0 && row < nr) {
    m_Y2[row] = !m_Y2[row];

    emit dataChanged(index(row,3), index(row,3));
  }
}

int QcepScatterPlotGraphModel::checkState(bool val) const
{
  if (val) {
    return Qt::Checked;
  } else {
    return Qt::Unchecked;
  }
}
