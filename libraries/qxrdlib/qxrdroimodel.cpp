#include "qxrdroimodel.h"
#include "qxrdroivector.h"
#include "qxrdroi.h"
#include "qcepmutexlocker.h"
#include <stdio.h>
#include "qxrdroishape.h"
#include "qtconcurrentrun.h"
#include "qxrddebug.h"

QxrdROIModel::QxrdROIModel(QxrdROIVectorWPtr rois)
  : QAbstractListModel(),
    m_ROIs(rois)
{
}

QxrdROIModel::~QxrdROIModel()
{
}

void QxrdROIModel::initialize(QObjectWPtr parent)
{
}

//QScriptValue QxrdROIModel::toScriptValue(QScriptEngine *engine, const QxrdROIModelPtr &coords)
//{
//  return engine->newQObject(coords.data());
//}

//void QxrdROIModel::fromScriptValue(const QScriptValue &obj, QxrdROIModelPtr &coords)
//{
//  QObject *qobj = obj.toQObject();

//  if (qobj) {
//    QxrdROIModel *qcoords = qobject_cast<QxrdROIModel*>(qobj);

//    if (qcoords) {
//      coords = QxrdROIModelPtr(qcoords);
//    }
//  }
//}

int QxrdROIModel::roiCount() const
{
  int res = 0;

  QxrdROIVectorPtr rois(m_ROIs);

  if (rois) {
    res = rois -> count();
  }

  return res;
}

int QxrdROIModel::rowCount(const QModelIndex & /*parent*/) const
{
  return roiCount();
}

int QxrdROIModel::columnCount(const QModelIndex & /*parent*/) const
{
  return ColCount;
}

QVariant QxrdROIModel::data(const QModelIndex &index, int role) const
{
  int row = index.row();
  int col = index.column();

  QxrdROIPtr c = roi(row);

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

QVariant QxrdROIModel::headerData(int section, Qt::Orientation orientation, int role) const
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

Qt::ItemFlags QxrdROIModel::flags(const QModelIndex &index) const
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

bool QxrdROIModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  int row = index.row();
  int col = index.column();

  QxrdROIPtr c = roi(row);

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

QxrdROIPtr QxrdROIModel::newROI(int roiOuterType, int roiInnerType)
{
  QxrdROI *coord = new QxrdROI(roiOuterType, roiInnerType);

  coord->moveToThread(thread());

  return QxrdROIPtr(coord);
}

void QxrdROIModel::append(QxrdROIPtr coords)
{
  QxrdROIVectorPtr rois(m_ROIs);

  if (rois) {
    beginInsertRows(QModelIndex(), rois -> count(), rois -> count());

    rois -> append(coords);

    connect(coords.data(), &QxrdROI::roiChanged,
            this, &QxrdROIModel::onROIChanged);

    endInsertRows();
  }
}

void QxrdROIModel::removeROI(int row)
{
  QxrdROIVectorPtr rois(m_ROIs);

  if (rois) {
    beginRemoveRows(QModelIndex(), row, row);

    rois -> remove(row);

    endRemoveRows();
  }
}

void QxrdROIModel::moveROIDown(int row)
{
  QxrdROIVectorPtr rois(m_ROIs);

  if (rois) {
    int nRows = rois -> count();

    if (row >= 0 && row < (nRows-1)) {
      beginMoveRows(QModelIndex(), row+1, row+1, QModelIndex(), row);

      rois -> exchange(row, row+1);

      endMoveRows();
    }
  }
}

void QxrdROIModel::moveROIUp(int row)
{
  QxrdROIVectorPtr rois(m_ROIs);

  if (rois) {
    int nRows = rois -> count();

    if (row >= 1 && row < nRows) {
      beginMoveRows(QModelIndex(), row, row, QModelIndex(), row-1);

      rois -> exchange(row-1, row);

      endMoveRows();
    }
  }
}

//TODO: implement
void QxrdROIModel::editROI(int /*row*/)
{
}

QxrdROIWPtr QxrdROIModel::roi(int row) const
{
  QxrdROIWPtr res;

  QxrdROIVectorPtr rois(m_ROIs);

  if (rois) {
    res = rois -> roi(row);
  }

  return res;
}

void QxrdROIModel::setRoi(int row, QxrdROIPtr c)
{
  QxrdROIVectorPtr rois(m_ROIs);

  if (rois) {
    if (row >= 0 && row < rois -> count()) {
      rois -> setRoi(row, c);

      emit dataChanged(index(row,0), index(row,ColCount-1));
    }
  }
}

void QxrdROIModel::moveROICenter(int i, double x, double y)
{
  QxrdROIPtr roi = this->roi(i);

  if (roi) {
    roi->setCenter(QPointF(x,y));

    emit dataChanged(index(i,0), index(i,ColCount-1));
  }
}

void QxrdROIModel::moveROIRelative(int i, double dx, double dy)
{
  QxrdROIPtr roi = this->roi(i);

  if (roi) {
    QPointF cen = roi->get_Center() + QPointF(dx, dy);

    roi->setCenter(cen);

    emit dataChanged(index(i,0), index(i,ColCount-1));
  }
}

void QxrdROIModel::setRotation(int i, double r)
{
  QxrdROIPtr roi = this->roi(i);

  if (roi) {
    roi->set_Rotation(r);

    emit dataChanged(index(i,0), index(i,ColCount-1));
  }
}

void QxrdROIModel::scaleROI
  (int i, int innerOuter, double kx, double ky)
{
  QxrdROIPtr roi = this->roi(i);

  if (roi) {
    roi->scaleROI(innerOuter, kx, ky);

    emit dataChanged(index(i,0), index(i,ColCount-1));
  }
}

void QxrdROIModel::deleteROIPoint(int i,
                                                 int innerOuter,
                                                 int n)
{
  QxrdROIPtr roi = this->roi(i);

  if (roi) {
    roi->deleteROIPoint(innerOuter, n);

    emit dataChanged(index(i,0), index(i,ColCount-1));
  }
}

void QxrdROIModel::changeROIPoint(int i,
                                                 int innerOuter,
                                                 int n,
                                                 QPointF pt)
{
  QxrdROIPtr roi = this->roi(i);

  if (roi) {
    roi->changeROIPoint(innerOuter, n, pt);

    emit dataChanged(index(i,0), index(i,ColCount-1));
  }
}

void QxrdROIModel::insertROIPoint(int i,
                                                 int innerOuter,
                                                 int n,
                                                 QPointF pt)
{
  QxrdROIPtr roi = this->roi(i);

  if (roi) {
    roi->insertROIPoint(innerOuter, n, pt);

    emit dataChanged(index(i,0), index(i,ColCount-1));
  }
}

void QxrdROIModel::onROIChanged()
{
  //  printf("QxrdROIModel::onROIChanged()\n");

  QxrdROIVectorPtr rois(m_ROIs);

  if (rois) {
    QObject *s = sender();

    QxrdROI *c = qobject_cast<QxrdROI*>(s);

    if (c) {
      for (int i=0; i< rois->count(); i++) {
        QxrdROIPtr r = rois->roi(i);

        if (r && r.data() == c) {
          //        printf("ROI %d changed\n", i);

          emit dataChanged(index(i,0), index(i,columnCount(QModelIndex())-1));
        }
      }
    }
  }
}

void QxrdROIModel::onROIsChanged()
{
  emit dataChanged(index(0,0), index(rowCount(QModelIndex()), columnCount(QModelIndex())-1));
}

void QxrdROIModel::recalculate(QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
  QxrdROIVectorPtr rois(m_ROIs);

  if (rois) {
    QTime tic;
    tic.start();

    QVector<  QFuture<void> > res;

    for (int i=0; i<rois->count(); i++) {
      QxrdROIPtr r = rois->roi(i);

      if (r) {
        res.append(
              QtConcurrent::run(r.data(),
                                &QxrdROI::recalculate, img, mask));

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

    emit dataChanged(index(0,SumCol), index(rois->count()-1,YGradientCol), roles);

    printf("ROI Calculation in %d msec\n",tic.elapsed());
  }
}

void QxrdROIModel::visualizeBackground(int n, QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
  QxrdROIPtr r = roi(n);

  if (r) {
    r->visualizeBackground(img, mask);
  }
}

void QxrdROIModel::visualizePeak(int n, QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
  QxrdROIPtr r = roi(n);

  if (r) {
    r->visualizePeak(img, mask);
  }
}
