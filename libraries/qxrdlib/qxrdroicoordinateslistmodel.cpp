#include "qxrdroicoordinateslistmodel.h"
#include "qxrdroicoordinates.h"
#include "qcepmutexlocker.h"
#include <stdio.h>
#include "qxrdroishape.h"
#include "qtconcurrentrun.h"
#include "qxrddebug.h"

QxrdROICoordinatesListModel::QxrdROICoordinatesListModel()
  : QAbstractListModel(),
    m_ROICoordinates()
{
}

QxrdROICoordinatesListModel::~QxrdROICoordinatesListModel()
{
}

void QxrdROICoordinatesListModel::readSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  beginResetModel();

  int n = settings->beginReadArray("roi");

  m_ROICoordinates.resize(0);

  for (int i=0; i<n; i++) {
    settings->setArrayIndex(i);

    int roiOuterType = settings->value("roiOuterType", 0).toInt();
    int roiInnerType = settings->value("roiInnerType", 0).toInt();

    QxrdROICoordinatesPtr roi = newROI(roiOuterType, roiInnerType);

    if (roi) {
      roi->readSettings(settings);

      m_ROICoordinates.append(roi);

      connect(roi.data(), &QxrdROICoordinates::roiChanged,
              this, &QxrdROICoordinatesListModel::onROIChanged);
    }
  }

  settings->endArray();

  endResetModel();
}

void QxrdROICoordinatesListModel::writeSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  settings->beginWriteArray("roi");

  for (int i=0; i<m_ROICoordinates.count(); i++) {
    settings->setArrayIndex(i);

    QxrdROICoordinatesPtr roi = m_ROICoordinates.value(i);

    if (roi) {
      roi->writeSettings(settings);
    }
  }

  settings->endArray();
}

QScriptValue QxrdROICoordinatesListModel::toScriptValue(QScriptEngine *engine, const QxrdROICoordinatesListModelPtr &coords)
{
  return engine->newQObject(coords.data());
}

void QxrdROICoordinatesListModel::fromScriptValue(const QScriptValue &obj, QxrdROICoordinatesListModelPtr &coords)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QxrdROICoordinatesListModel *qcoords = qobject_cast<QxrdROICoordinatesListModel*>(qobj);

    if (qcoords) {
      coords = QxrdROICoordinatesListModelPtr(qcoords);
    }
  }
}

int QxrdROICoordinatesListModel::roiCount() const
{
  return m_ROICoordinates.count();
}

int QxrdROICoordinatesListModel::rowCount(const QModelIndex & /*parent*/) const
{
  return m_ROICoordinates.count();
}

int QxrdROICoordinatesListModel::columnCount(const QModelIndex & /*parent*/) const
{
  return ColCount;
}

QVariant QxrdROICoordinatesListModel::data(const QModelIndex &index, int role) const
{
  int row = index.row();
  int col = index.column();

  QxrdROICoordinatesPtr c = roi(row);

  if (c) {
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
      if (col == NumCol) {
        return row;
      } else if (col == SumCol) {
        return c->get_Sum();
      } else if (col == AverageCol) {
        return c->get_Average();
      } else if (col == MinimumCol) {
        return c->get_Minimum();
      } else if (col == MaximumCol) {
        return c->get_Maximum();
      } else if (col == NPixelsCol) {
        return c->get_NPixels();
      } else if (col == BackgroundCol) {
        return c->get_Background();
      } else if (col == XGradientCol) {
        return c->get_XGradient();
      } else if (col == YGradientCol) {
        return c->get_YGradient();
      } else if (col == OuterTypeCol) {
        return c->outerTypeName();
      } else if (col == InnerTypeCol) {
        return c->innerTypeName();
      } else if (col == CenterXCol) {
        return c->get_Center().x();
      } else if (col == CenterYCol) {
        return c->get_Center().y();
      } else if (col == RotationCol) {
        return c->get_Rotation();
//      } else if (col == WidthCol) {
//        return c->get_Coords().width();
//      } else if (col == HeightCol) {
//        return c->get_Coords().height();
//      } else if (col == Width2Col) {
//        return c->get_Width2();
//      } else if (col == Height2Col) {
//        return c->get_Height2();
      }
    }
  }

  return QVariant();
}

QVariant QxrdROICoordinatesListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal) {
    if (role == Qt::DisplayRole) {
      if (section == NumCol) {
        return "#";
      } else if (section == SumCol) {
        return "Sum";
      } else if (section == AverageCol) {
        return "Avg";
      } else if (section == MinimumCol) {
        return "Min";
      } else if (section == MaximumCol) {
        return "Max";
      } else if (section == NPixelsCol) {
        return "NPix";
      } else if (section == BackgroundCol) {
        return "Bkgd";
      } else if (section == XGradientCol) {
        return "XGrad";
      } else if (section == YGradientCol) {
        return "YGrad";
      } else if (section == OuterTypeCol) {
        return "Bkgd Shape";
      } else if (section == InnerTypeCol) {
        return "Peak Shape";
      } else if (section == CenterXCol) {
        return "CenterX";
      } else if (section == CenterYCol) {
        return "CenterY";
      } else if (section == RotationCol) {
        return "Rotation";
//      } else if (section == WidthCol) {
//        return "Width";
//      } else if (section == HeightCol) {
//        return "Height";
//      } else if (section == Width2Col) {
//        return "Wd 2";
//      } else if (section == Height2Col) {
//        return "Ht 2";
      }
    } else if (role == Qt::TextAlignmentRole) {
      return Qt::AlignHCenter;
    }
  }

  return QVariant();
}

Qt::ItemFlags QxrdROICoordinatesListModel::flags(const QModelIndex &index) const
{
//  int row = index.row();
  int col = index.column();

  if (/*col == OuterTypeCol ||
      col == InnerTypeCol || */
      col == CenterXCol ||
      col == CenterYCol ||
      col == RotationCol /* ||
      col == WidthCol ||
      col == HeightCol ||
      col == Width2Col ||
      col == Height2Col*/) {
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
  } else {
    return QAbstractListModel::flags(index);
  }
}

bool QxrdROICoordinatesListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  int row = index.row();
  int col = index.column();

  QxrdROICoordinatesPtr c = roi(row);

  if (c) {
    if (role == Qt::EditRole || role == Qt::DisplayRole) {
      if (col == SumCol) {
        c->set_Sum(value.toDouble());
      } else if (col == AverageCol) {
        c->set_Average(value.toDouble());
      } else if (col == MinimumCol) {
        c->set_Minimum(value.toDouble());
      } else if (col == MaximumCol) {
        c->set_Maximum(value.toDouble());
      } else if (col == NPixelsCol) {
        c->set_NPixels(value.toDouble());
      } else if (col == BackgroundCol) {
        c->set_Background(value.toDouble());
      } else if (col == XGradientCol) {
        c->set_XGradient(value.toDouble());
      } else if (col == YGradientCol) {
        c->set_YGradient(value.toDouble());
      } else if (col == OuterTypeCol) {
        c->selectNamedROIOuterType(value.toString());
      } else if (col == InnerTypeCol) {
        c->selectNamedROIInnerType(value.toString());
      } else if (col == CenterXCol) {
        c->setCenterX(value.toDouble());
      } else if (col == CenterYCol) {
        c->setCenterY(value.toDouble());
      } else if (col == RotationCol) {
        c->setRotation(value.toDouble());
//      } else if (col == WidthCol) {
//        c->setWidth(value.toDouble());
//      } else if (col == HeightCol) {
//        c->setHeight(value.toDouble());
//      } else if (col == Width2Col) {
//        c->setWidth2(value.toDouble());
//      } else if (col == Height2Col) {
//        c->setHeight2(value.toDouble());
      } else {
        return false;
      }

      setRoi(row, c);

      emit dataChanged(index, index);

      return true;
    }
  }
  return false;
}

QxrdROICoordinatesPtr QxrdROICoordinatesListModel::newROI(int roiOuterType, int roiInnerType)
{
  QxrdROICoordinates *coord = new QxrdROICoordinates(roiOuterType, roiInnerType);

  coord->moveToThread(thread());

  return QxrdROICoordinatesPtr(coord);
}

void QxrdROICoordinatesListModel::append(QxrdROICoordinatesPtr coords)
{
  beginInsertRows(QModelIndex(), m_ROICoordinates.count(), m_ROICoordinates.count());

  m_ROICoordinates.append(coords);

  connect(coords.data(), &QxrdROICoordinates::roiChanged,
          this, &QxrdROICoordinatesListModel::onROIChanged);

  endInsertRows();
}

void QxrdROICoordinatesListModel::removeROI(int row)
{
  beginRemoveRows(QModelIndex(), row, row);

  m_ROICoordinates.remove(row);

  endRemoveRows();
}

void QxrdROICoordinatesListModel::moveROIDown(int row)
{
  int nRows = m_ROICoordinates.count();

  if (row >= 0 && row < (nRows-1)) {
    beginMoveRows(QModelIndex(), row+1, row+1, QModelIndex(), row);

    QxrdROICoordinatesPtr p1 = m_ROICoordinates.value(row);
    QxrdROICoordinatesPtr p2 = m_ROICoordinates.value(row+1);

    m_ROICoordinates[row]   = p2;
    m_ROICoordinates[row+1] = p1;

    endMoveRows();
  }
}

void QxrdROICoordinatesListModel::moveROIUp(int row)
{
  int nRows = m_ROICoordinates.count();

  if (row >= 1 && row < nRows) {
    beginMoveRows(QModelIndex(), row, row, QModelIndex(), row-1);

    QxrdROICoordinatesPtr p1 = m_ROICoordinates.value(row-1);
    QxrdROICoordinatesPtr p2 = m_ROICoordinates.value(row);

    m_ROICoordinates[row-1] = p2;
    m_ROICoordinates[row]   = p1;

    endMoveRows();
  }
}

//TODO: implement
void QxrdROICoordinatesListModel::editROI(int /*row*/)
{
}

QxrdROICoordinatesWPtr QxrdROICoordinatesListModel::roi(int row) const
{
  return m_ROICoordinates.value(row);
}

void QxrdROICoordinatesListModel::setRoi(int row, QxrdROICoordinatesPtr c)
{
  if (row >= 0 && row < m_ROICoordinates.count()) {
    m_ROICoordinates[row] = c;

    emit dataChanged(index(row,0), index(row,ColCount-1));
  }
}

void QxrdROICoordinatesListModel::moveROICenter(int i, double x, double y)
{
  QxrdROICoordinatesPtr roi = this->roi(i);

  if (roi) {
    roi->setCenter(QPointF(x,y));

    emit dataChanged(index(i,0), index(i,ColCount-1));
  }
}

void QxrdROICoordinatesListModel::moveROIRelative(int i, double dx, double dy)
{
  QxrdROICoordinatesPtr roi = this->roi(i);

  if (roi) {
    QPointF cen = roi->get_Center() + QPointF(dx, dy);

    roi->setCenter(cen);

    emit dataChanged(index(i,0), index(i,ColCount-1));
  }
}

void QxrdROICoordinatesListModel::setRotation(int i, double r)
{
  QxrdROICoordinatesPtr roi = this->roi(i);

  if (roi) {
    roi->set_Rotation(r);

    emit dataChanged(index(i,0), index(i,ColCount-1));
  }
}

void QxrdROICoordinatesListModel::scaleROI
  (int i, int innerOuter, double kx, double ky)
{
  QxrdROICoordinatesPtr roi = this->roi(i);

  if (roi) {
    roi->scaleROI(innerOuter, kx, ky);

    emit dataChanged(index(i,0), index(i,ColCount-1));
  }
}

void QxrdROICoordinatesListModel::deleteROIPoint(int i,
                                                 int innerOuter,
                                                 int n)
{
  QxrdROICoordinatesPtr roi = this->roi(i);

  if (roi) {
    roi->deleteROIPoint(innerOuter, n);

    emit dataChanged(index(i,0), index(i,ColCount-1));
  }
}

void QxrdROICoordinatesListModel::changeROIPoint(int i,
                                                 int innerOuter,
                                                 int n,
                                                 QPointF pt)
{
  QxrdROICoordinatesPtr roi = this->roi(i);

  if (roi) {
    roi->changeROIPoint(innerOuter, n, pt);

    emit dataChanged(index(i,0), index(i,ColCount-1));
  }
}

void QxrdROICoordinatesListModel::insertROIPoint(int i,
                                                 int innerOuter,
                                                 int n,
                                                 QPointF pt)
{
  QxrdROICoordinatesPtr roi = this->roi(i);

  if (roi) {
    roi->insertROIPoint(innerOuter, n, pt);

    emit dataChanged(index(i,0), index(i,ColCount-1));
  }
}

void QxrdROICoordinatesListModel::onROIChanged()
{
//  printf("QxrdROICoordinatesListModel::onROIChanged()\n");

  QObject *s = sender();

  QxrdROICoordinates *c = qobject_cast<QxrdROICoordinates*>(s);

  if (c) {
    for (int i=0; i<m_ROICoordinates.count(); i++) {
      QxrdROICoordinatesPtr r = m_ROICoordinates.value(i);

      if (r && r.data() == c) {
//        printf("ROI %d changed\n", i);

        emit dataChanged(index(i,0), index(i,columnCount(QModelIndex())-1));
      }
    }
  }
}

void QxrdROICoordinatesListModel::onROIsChanged()
{
  emit dataChanged(index(0,0), index(rowCount(QModelIndex()), columnCount(QModelIndex())-1));
}

void QxrdROICoordinatesListModel::recalculate(QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
  QTime tic;
  tic.start();

  QVector<  QFuture<void> > res;

  for (int i=0; i<m_ROICoordinates.count(); i++) {
    QxrdROICoordinatesPtr r = m_ROICoordinates.value(i);

    if (r) {
      res.append(
            QtConcurrent::run(r.data(),
                              &QxrdROICoordinates::recalculate, img, mask));

      if (qcepDebug(DEBUG_NOPARALLEL)) {
        res.value(i).waitForFinished();
      }
    }
  }

  for (int i=0; i<res.count(); i++) {
    res.value(i).waitForFinished();
  }

  QVector<int> roles;
  roles.append(Qt::DisplayRole);
  roles.append(Qt::EditRole);

  emit dataChanged(index(0,SumCol), index(m_ROICoordinates.count()-1,YGradientCol), roles);

  printf("ROI Calculation in %d msec\n",tic.elapsed());
}

void QxrdROICoordinatesListModel::visualizeBackground(int n, QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
  QxrdROICoordinatesPtr r = m_ROICoordinates.value(n);

  if (r) {
    r->visualizeBackground(img, mask);
  }
}

void QxrdROICoordinatesListModel::visualizePeak(int n, QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
  QxrdROICoordinatesPtr r = m_ROICoordinates.value(n);

  if (r) {
    r->visualizePeak(img, mask);
  }
}
