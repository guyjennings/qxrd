#include "qxrdroicoordinates.h"
#include "qxrdexperiment.h"
#include <QtMath>
#include <QMatrix4x4>
#include "qxrdroishape.h"
#include "qcepmutexlocker.h"

QxrdROICoordinates::QxrdROICoordinates(int                   roiOuterType,
                                       int                   roiInnerType)
  : QcepSerializableObject("coords"),
    m_RoiOuterType(this, "roiOuterType", roiOuterType, "ROI Outer Type"),
    m_RoiInnerType(this, "roiInnerType", roiInnerType, "ROI Inner Type"),
    m_RoiOuterTypeName(this, "roiOuterTypeName", QxrdROIShape::roiTypeName(roiOuterType), "ROI Outer Type Name"),
    m_RoiInnerTypeName(this, "roiInnerTypeName", QxrdROIShape::roiTypeName(roiInnerType), "ROI Inner Type Name"),
    m_Center(this, "center", QPointF(0, 0), "ROI Center"),
    m_Rotation(this, "rotation", 0, "ROI Rotation"),
    m_Sum(this, "sum", 0, "ROI Pixel Sum"),
    m_Average(this, "average", 0, "ROI Pixel Average"),
    m_Minimum(this, "minimum", 0, "ROI Pixel Minimum"),
    m_Maximum(this, "maximum", 0, "ROI Pixel Maximum"),
    m_NPixels(this, "nPixels", 0, "ROI N Pixels"),
    m_NBackground(this, "nBackground", 0, "ROI N Bkgd Pixels"),
    m_Background(this, "background", 0, "ROI Background"),
    m_XGradient(this, "xGradient", 0, "ROI X Gradient"),
    m_YGradient(this, "yGradient", 0, "ROI Y Gradient"),
    m_Mutex(QMutex::Recursive)
{
  m_OuterShape = QxrdROIShape::newROIShape(roiOuterType, 1.0);
  m_InnerShape = QxrdROIShape::newROIShape(roiInnerType, 0.25);

  connect(m_OuterShape.data(), &QxrdROIShape::roiChanged, this, &QxrdROICoordinates::outerChanged);
  connect(m_InnerShape.data(), &QxrdROIShape::roiChanged, this, &QxrdROICoordinates::innerChanged);

  outerChanged();
  innerChanged();
}

QxrdROICoordinates::~QxrdROICoordinates()
{
}

void QxrdROICoordinates::writeSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepSerializableObject::writeSettings(settings, section);

  if (m_OuterShape) {
    m_OuterShape->writeSettings(settings, section+"/outer");
  }

  if (m_InnerShape) {
    m_InnerShape->writeSettings(settings, section+"/inner");
  }
}

void QxrdROICoordinates::readSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepSerializableObject::readSettings(settings, section);

  if (m_OuterShape) {
    m_OuterShape->readSettings(settings, section+"/outer");
  }

  if (m_InnerShape) {
    m_InnerShape->readSettings(settings, section+"/inner");
  }

  outerChanged();
  innerChanged();
}

QScriptValue QxrdROICoordinates::toScriptValue(QScriptEngine *engine, const QxrdROICoordinatesPtr &coords)
{
  return engine->newQObject(coords.data());
}

void QxrdROICoordinates::fromScriptValue(const QScriptValue &obj, QxrdROICoordinatesPtr &coords)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QxrdROICoordinates *qcoords = qobject_cast<QxrdROICoordinates*>(qobj);

    if (qcoords) {
      coords = QxrdROICoordinatesPtr(qcoords);
    }
  }
}

int QxrdROICoordinates::roiTypeID(int outerType, int innerType)
{
  return (outerType & 15) + ((innerType & 15)<<4);
}

QString QxrdROICoordinates::roiTypeName(int outerType, int innerType)
{
  if (outerType == QxrdROIShape::NoShape) {
    return QxrdROIShape::roiTypeName(innerType);
  } else {
    return QxrdROIShape::roiTypeName(innerType) + " in " + QxrdROIShape::roiTypeName(outerType);
  }
}

int QxrdROICoordinates::roiOuterType(int roiTypeID)
{
  return roiTypeID & 15;
}

int QxrdROICoordinates::roiInnerType(int roiTypeID)
{
  return (roiTypeID>>4) & 15;
}

QxrdROICoordinatesPtr QxrdROICoordinates::newROICoordinates(int roiTypeID)
{
  int outerType = roiOuterType(roiTypeID);
  int innerType = roiInnerType(roiTypeID);

  QxrdROICoordinatesPtr res = QxrdROICoordinatesPtr(
        new QxrdROICoordinates(outerType, innerType));

  return res;
}

QxrdROIShapePtr QxrdROICoordinates::outer() const
{
  return m_OuterShape;
}

QxrdROIShapePtr QxrdROICoordinates::inner() const
{
  return m_InnerShape;
}

void QxrdROICoordinates::selectNamedROIOuterType(QString nm)
{
  for (int i=0; i<QxrdROIShape::roiTypeCount(); i++) {
    if (QxrdROIShape::roiTypeName(i) == nm) {
      set_RoiOuterType(i);
      set_RoiOuterTypeName(nm);
      emit roiChanged();
      return;
    }
  }
}

void QxrdROICoordinates::selectNamedROIInnerType(QString nm)
{
  for (int i=0; i<QxrdROIShape::roiTypeCount(); i++) {
    if (QxrdROIShape::roiTypeName(i) == nm) {
      set_RoiInnerType(i);
      set_RoiInnerTypeName(nm);
      emit roiChanged();
      return;
    }
  }
}

void QxrdROICoordinates::updateBounds()
{
//  if (m_OuterShape) {
//    QRectF r = m_OuterShape->get_Coords();

//    if (m_InnerShape) {
//      QRectF r1 = m_InnerShape->get_Coords();

//      set_Coords(r | r1);
//    } else {
//      set_Coords(r);
//    }
//  } else if (m_InnerShape) {
//    set_Coords(m_InnerShape->get_Coords());
//  }
}

void QxrdROICoordinates::outerChanged()
{
  updateBounds();

  emit roiChanged();
}

void QxrdROICoordinates::innerChanged()
{
  updateBounds();

  emit roiChanged();
}

void QxrdROICoordinates::setCenter(QPointF c)
{
  set_Center(c);
}

void QxrdROICoordinates::setCenterX(double cx)
{
  QPointF c = get_Center();

  c.setX(cx);

  set_Center(c);
}

void QxrdROICoordinates::setCenterY(double cy)
{
  QPointF c = get_Center();

  c.setY(cy);

  set_Center(c);
}

void QxrdROICoordinates::setRotation(double r)
{
  set_Rotation(r);
}

QPointF QxrdROICoordinates::transform(const QPointF &pt) const
{
  QPointF res;
  QPointF c = get_Center();
  double  r = get_Rotation();
  QMatrix m;
  m.rotate(r);

  res = c + m.map(pt);

  return res;
}

QPointF QxrdROICoordinates::invTransform(const QPointF &pt) const
{
  QPointF res;

  QPointF c = get_Center();
  double  r = get_Rotation();
  QMatrix m;
  m.rotate(-r);

  res = m.map(pt - c);

  return res;
}

QVector<QPointF> QxrdROICoordinates::markerCoords()
{
  QVector<QPointF> res;
  QPointF c = get_Center();
  double  r = get_Rotation();
  QMatrix m;

  m.rotate(r);

  res.append(c);

  if (m_OuterShape) {
    res.append(QPointF(qQNaN(), qQNaN()));

    for (int i=0; i<m_OuterShape->markerCount(); i++) {
      res.append(c + m.map(m_OuterShape->markerPoint(i)));
    }
  }

  if (m_InnerShape) {
    res.append(QPointF(qQNaN(), qQNaN()));

    for (int i=0; i<m_InnerShape->markerCount(); i++) {
      res.append(c + m.map(m_InnerShape->markerPoint(i)));
    }
  }

  return res;
}

void QxrdROICoordinates::recalculate(QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
  recalculatePrivate(img, mask, VisualizeNone);
}

void QxrdROICoordinates::visualizeBackground(QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
  recalculatePrivate(img, mask, VisualizeBackground);
}

void QxrdROICoordinates::visualizePeak(QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
  recalculatePrivate(img, mask, VisualizePeak);
}

bool QxrdROICoordinates::pointInOuter(QPointF pt)
{
  QxrdROIShapePtr sh = outer();

  if (sh) {
    QPointF ptf = invTransform(pt);

    return sh->pointInShape(ptf);
  } else {
    return false;
  }
}

bool QxrdROICoordinates::pointInInner(QPointF pt)
{
  QxrdROIShapePtr sh = inner();

  if (sh) {
    QPointF ptf = invTransform(pt);

    return sh->pointInShape(ptf);
  } else {
    return false;
  }
}

void QxrdROICoordinates::recalculatePrivate(QcepImageDataBasePtr img, QcepMaskDataPtr mask, int vis)
{
#ifndef QT_NO_DEBUG
  printf("Recalculate ROI, inner: %s, outer: %s\n",
         qPrintable(get_RoiInnerTypeName()),
         qPrintable(get_RoiOuterTypeName()));
#endif

  QTime tic;

  tic.start();

  int nouter = 0;
  int ninner = 0;

  if (m_InnerShape && m_OuterShape && img) {
    QPointF c = get_Center();
    double  r = get_Rotation();
    QMatrix m, mn;
    m.rotate(r);
    mn.rotate(-r);

    QRectF innerBounds = m.mapRect(m_InnerShape->boundingRect()).translated(c);
    QRectF outerBounds = m.mapRect(m_OuterShape->boundingRect()).translated(c);

    QRectF bounds = innerBounds | outerBounds;

    int tp = qRound(bounds.top());
    int bt = qRound(bounds.bottom());
    int lf = qRound(bounds.left());
    int rt = qRound(bounds.right());

    double cx = c.x();
    double cy = c.y();

    int first = true;

    double min = 0;
    double max = 0;
    double sum = 0;
    double npx = 0;

    double sumvn = 0;
    double sumvx = 0;
    double sumvy = 0;

    double sumct = 0;
    double sumnn = 0;
    double sumnx = 0;
    double sumny = 0;
    double sumxy = 0;
    double sumxx = 0;
    double sumyy = 0;

    double bkgd = 0;
    double gradx = 0;
    double grady = 0;

    for (int row=tp; row<=bt; row++) {
      double dy = row - cy;

      for (int col=lf; col<=rt; col++) {
        double dx = col - cx;

        if (mask == NULL || mask->value(col, row)) {
          QPointF p = mn.map(QPointF(col,row)-c);

          if (m_InnerShape->pointInShape(p)) {
            // Peak point, skip for now...
            ninner += 1;
          } else if (m_OuterShape->pointInShape(p)) {
            nouter += 1;
            // Background point...
            double val = img->getImageData(col, row);

            if (val == val) {
              sumct += 1;
              sumnn += 1;
              sumnx += dx;
              sumny += dy;
              sumxy += dx*dy;
              sumxx += dx*dx;
              sumyy += dy*dy;

              sumvn += val;
              sumvx += val*dx;
              sumvy += val*dy;
            }
          }
        }
      }
    }

    if (sumct > 5) {
      QMatrix4x4 m;
      m(0,0) = sumnn;
      m(1,0) = sumnx;
      m(0,1) = sumnx;
      m(2,0) = sumny;
      m(0,2) = sumny;
      m(1,1) = sumxx;
      m(2,1) = sumxy;
      m(1,2) = sumxy;
      m(2,2) = sumyy;

      bool invertible;

      QMatrix4x4 inv = m.inverted(&invertible);

      if (invertible) {
        bkgd   = inv(0,0)*sumvn + inv(0,1)*sumvx + inv(0,2)*sumvy;
        gradx  = inv(1,0)*sumvn + inv(1,1)*sumvx + inv(1,2)*sumvy;
        grady  = inv(2,0)*sumvn + inv(2,1)*sumvx + inv(2,2)*sumvy;
      } else {
        bkgd = sumvn/sumct;
      }
    } else if (sumct > 0) {
      bkgd = sumvn/sumct;
    }

    tp = qRound(innerBounds.top());
    bt = qRound(innerBounds.bottom());
    lf = qRound(innerBounds.left());
    rt = qRound(innerBounds.right());

    for (int row=tp; row<=bt; row++) {
      double dy = row - cy;

      for (int col=lf; col<=rt; col++) {
        double dx = col - cx;

        if (mask == NULL || mask->value(col, row)) {
          QPointF p = mn.map(QPointF(col,row)-c);

          if (m_InnerShape->pointInShape(p)) {
            double val = img->getImageData(col, row);

            if (val == val) {
              double bk = bkgd + dx*gradx + dy*grady;
              double v  = val - bk;

              if (first) {
                min = v;
                max = v;
                first = false;
              } else if (v > max) {
                max = v;
              } else if (v < min) {
                min = v;
              }

              sum += v;
              npx += 1;
            }
          }
        }
      }
    }

    set_Sum(sum);
    set_NPixels(npx);
    set_NBackground(nouter);
    set_Minimum(min);
    set_Maximum(max);

    if (npx > 0) {
      set_Average(sum/npx);
    } else {
      set_Average(0);
    }

    set_Background(bkgd);
    set_XGradient(gradx);
    set_YGradient(grady);
  }

#ifndef QT_NO_DEBUG
  printMessage(tr("ROI Calculation took %1 msec").arg(tic.elapsed()));
  printMessage(tr("Inner #%1, Outer #%2").arg(ninner).arg(nouter));
#endif
}

//void QxrdROICoordinates::recalculatePrivate(QcepImageDataBasePtr img, QcepMaskDataPtr mask, int vis)
//{
//  int outerBounds = NoBounds;
//  int innerBounds = NoBounds;

//  switch (get_RoiOuterType()) {
//  case QxrdROIShape::RectangleShape:
//    outerBounds = RectangleBounds;
//    break;

//  case QxrdROIShape::EllipseShape:
//    outerBounds = EllipseBounds;
//    break;
//  }

//  switch (get_RoiInnerType()) {
//  case QxrdROIShape::RectangleShape:
//    innerBounds = RectangleBounds;
//    break;

//  case QxrdROIShape::EllipseShape:
//    innerBounds = EllipseBounds;
//    break;
//  }

//  int first = true;
//  double min = 0;
//  double max = 0;
//  double sum = 0;
//  double npx = 0;

//  double sumvn = 0;
//  double sumvx = 0;
//  double sumvy = 0;

//  double sumct = 0;
//  double sumnn = 0;
//  double sumnx = 0;
//  double sumny = 0;
//  double sumxy = 0;
//  double sumxx = 0;
//  double sumyy = 0;

//  double bkgd = 0;
//  double gradx = 0;
//  double grady = 0;

//  if (img) {
//    int tp = qRound(m_OuterShape->top());
//    int bt = qRound(m_OuterShape->bottom());
//    int lf = qRound(m_OuterShape->left());
//    int rt = qRound(m_OuterShape->right());

//    int tp2 = qRound(m_InnerShape->top());
//    int bt2 = qRound(m_InnerShape->bottom());
//    int lf2 = qRound(m_InnerShape->left());
//    int rt2 = qRound(m_InnerShape->right());

//    double cx = m_OuterShape->center().x();
//    double cy = m_OuterShape->center().y();
//    double a  = m_OuterShape->width()/2.0;
//    double b  = m_OuterShape->height()/2.0;
//    double a2 = m_InnerShape->width()/2.0;
//    double b2 = m_InnerShape->height()/2.0;

//    for (int row=tp; row<=bt; row++) {

//      if (outerBounds == EllipseBounds) {
//        double xx = a*sqrt(1 - pow(dy/b,2));
//        lf = qRound(cx - xx);
//        rt = qRound(cx + xx);
//      }

//      for (int col=lf; col<=rt; col++) {
//        double dx = col - cx;

//        if (innerBounds == EllipseBounds) {
//          double xx2 = a2*sqrt(1 - pow(dy/b2,2));
//          if (xx2==xx2) {
//            lf2 = qRound(cx - xx2);
//            rt2 = qRound(cx + xx2);
//          } else {
//            lf2 = rt;
//            rt2 = rt;
//          }
//        }

//        if (innerBounds == NoBounds
//            || (row <= tp2)
//            || (row >= bt2)
//            || (col <= lf2)
//            || (col >= rt2)) {
//          if (mask == NULL || mask->value(col, row)) {
//            double val = img->getImageData(col, row);

//            if (val == val) {
//              sumct += 1;
//              sumnn += 1;
//              sumnx += dx;
//              sumny += dy;
//              sumxy += dx*dy;
//              sumxx += dx*dx;
//              sumyy += dy*dy;

//              sumvn += val;
//              sumvx += val*dx;
//              sumvy += val*dy;

//              if (innerBounds == NoBounds) {
//                if (first) {
//                  min = val;
//                  max = val;
//                  first = false;
//                } else if (val > max) {
//                  max = val;
//                } else if (val < min) {
//                  min = val;
//                }
//              }

//              sum += val;
//              npx += 1;

//              if (vis == VisualizeBackground) {
//                img->setImageData(col, row, 1000 - val);
//              }
//            }
//          }
//        }
//      }
//    }

//    if (sumct > 5) {
//      QMatrix4x4 m;

//      m(0,0) = sumnn;
//      m(1,0) = sumnx;
//      m(0,1) = sumnx;
//      m(2,0) = sumny;
//      m(0,2) = sumny;
//      m(1,1) = sumxx;
//      m(2,1) = sumxy;
//      m(1,2) = sumxy;
//      m(2,2) = sumyy;

//      bool invertible;

//      QMatrix4x4 inv = m.inverted(&invertible);

//      if (invertible) {
//        bkgd   = inv(0,0)*sumvn + inv(0,1)*sumvx + inv(0,2)*sumvy;
//        gradx  = inv(1,0)*sumvn + inv(1,1)*sumvx + inv(1,2)*sumvy;
//        grady  = inv(2,0)*sumvn + inv(2,1)*sumvx + inv(2,2)*sumvy;
//      } else {
//        bkgd = sumvn/sumct;
//      }
//    } else if (sumct > 0) {
//      bkgd = sumvn/sumct;
//    }
//    if (innerBounds != NoBounds) {
//      first = true;
//      min = 0;
//      max = 0;
//      sum = 0;
//      npx = 0;

//      for (int row=tp2; row<=bt2; row++) {
//        double dy = row - cy;

//        if (innerBounds == EllipseBounds) {
//          double xx2 = a2*sqrt(1 - pow(dy/b2,2));
//          if (xx2==xx2) {
//            lf2 = qRound(cx - xx2);
//            rt2 = qRound(cx + xx2);
//          } else {
//            lf2 = rt;
//            rt2 = rt-1;
//          }
//        }

//        for (int col=lf2; col<=rt2; col++) {
//          if (mask == NULL || mask->value(col, row)) {
//            double val = img->getImageData(col, row);

//            if (val == val) {
//              double dx = col - cx;
//              double bk = bkgd + dx*gradx + dy*grady;
//              double v  = val - bk;

//              if (first) {
//                min = v;
//                max = v;
//                first = false;
//              } else if (v > max) {
//                max = v;
//              } else if (v < min) {
//                min = v;
//              }

//              sum += v;
//              npx += 1;

//              if (vis == VisualizePeak) {
//                img->setImageData(col, row, 1000 - val);
//              }
//            }
//          }
//        }
//      }
//    }
//  }

//  set_Sum(sum);

//  set_NPixels(npx);
//  set_Minimum(min);
//  set_Maximum(max);

//  if (npx > 0) {
//    set_Average(sum/npx);
//  } else {
//    set_Average(0);
//  }

//  set_Background(bkgd);
//  set_XGradient(gradx);
//  set_YGradient(grady);
//}

QVector<double> QxrdROICoordinates::values() const
{
  QVector<double> res;

  res.append(get_Sum());
  res.append(get_Average());
  res.append(get_Minimum());
  res.append(get_Maximum());
  res.append(get_NPixels());
  res.append(get_NBackground());
  res.append(get_Background());
  res.append(get_XGradient());
  res.append(get_YGradient());

  return res;
}

int QxrdROICoordinates::outputCount()
{
  return OutputCount;
}

QString QxrdROICoordinates::outputName(int opt)
{
  QString res;

  switch (opt) {
  case SumOutput:
    res = "Sum";
    break;
  case AverageOutput:
    res = "Average";
    break;
  case MinimumOutput:
    res = "Minimum";
    break;
  case MaximumOutput:
    res = "Maximum";
    break;
  case NPixelsOutput:
    res = "# Pixels";
    break;
  case NBackgroundOutput:
    res = "# Bkgd Px";
    break;
  case BackgroundOutput:
    res = "Background";
    break;
  case XGradientOutput:
    res = "X Gradient";
    break;
  case YGradientOutput:
    res = "Y Gradient";
    break;
  }

  return res;
}
