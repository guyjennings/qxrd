#include "qxrdroicoordinates.h"
#include "qxrdexperiment.h"
#include <QtMath>
#include <QMatrix4x4>
#include "qxrdroishape.h"
#include "qcepmutexlocker.h"
#include "qcepproperty.h"
#include "qxrdroicache.h"

QxrdROICoordinates::QxrdROICoordinates(int                   roiOuterType,
                                       int                   roiInnerType)
  : QcepSerializableObject("coords"),
    m_RoiOuterType(this, "roiOuterType", roiOuterType, "ROI Outer Type"),
    m_RoiInnerType(this, "roiInnerType", roiInnerType, "ROI Inner Type"),
    m_RoiOuterTypeName(this, "roiOuterTypeName", QxrdROIShape::roiTypeName(roiOuterType), "ROI Outer Type Name"),
    m_RoiInnerTypeName(this, "roiInnerTypeName", QxrdROIShape::roiTypeName(roiInnerType), "ROI Inner Type Name"),
    m_Center(this, "center", QPointF(0, 0), "ROI Center"),
    m_Rotation(this, "rotation", 0, "ROI Rotation"),
    m_Changed(this, "changed", true, "ROI Has been changed?"),
    m_Sum(this, "sum", 0, "ROI Pixel Sum"),
    m_Average(this, "average", 0, "ROI Pixel Average"),
    m_Minimum(this, "minimum", 0, "ROI Pixel Minimum"),
    m_Maximum(this, "maximum", 0, "ROI Pixel Maximum"),
    m_NPixels(this, "nPixels", 0, "ROI N Pixels"),
    m_NBackground(this, "nBackground", 0, "ROI N Bkgd Pixels"),
    m_Background(this, "background", 0, "ROI Background"),
    m_XGradient(this, "xGradient", 0, "ROI X Gradient"),
    m_YGradient(this, "yGradient", 0, "ROI Y Gradient"),
    m_InnerSum(this, "innerSum", 0, "Inner ROI Pixel Sum"),
    m_OuterSum(this, "outerSum", 0, "Outer ROI Pixel Sum"),
    m_Mutex(QMutex::Recursive),
    m_OuterShape(QxrdROIShape::newROIShape(roiOuterType, 1.0)),
    m_InnerShape(QxrdROIShape::newROIShape(roiInnerType, 0.25)),
    m_Cache(new QxrdROICache()),
    m_Bounds(QRect()),
    m_InnerBounds(QRect()),
    m_OuterBounds(QRect())
{
  connect(m_OuterShape.data(), &QxrdROIShape::roiChanged, this, &QxrdROICoordinates::changed);
  connect(m_InnerShape.data(), &QxrdROIShape::roiChanged, this, &QxrdROICoordinates::changed);

  connect(prop_Center(), &QcepDoublePointProperty::valueChanged, this, &QxrdROICoordinates::changed);
  connect(prop_Rotation(), &QcepDoubleProperty::valueChanged, this, &QxrdROICoordinates::changed);

  changed();
}

QxrdROICoordinates::~QxrdROICoordinates()
{
}

void QxrdROICoordinates::writeSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepSerializableObject::writeSettings(settings);

  if (m_OuterShape) {
    settings->beginGroup("outer");
    m_OuterShape->writeSettings(settings);
    settings->endGroup();
  }

  if (m_InnerShape) {
    settings->beginGroup("inner");
    m_InnerShape->writeSettings(settings);
    settings->endGroup();
  }
}

void QxrdROICoordinates::readSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepSerializableObject::readSettings(settings);

  if (m_OuterShape) {
    settings->beginGroup("outer");
    m_OuterShape->readSettings(settings);
    settings->endGroup();
  }

  if (m_InnerShape) {
    settings->beginGroup("inner");
    m_InnerShape->readSettings(settings);
    settings->endGroup();
  }

  changed();
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

void QxrdROICoordinates::changed()
{
  set_Changed(true);

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

QPolygonF QxrdROICoordinates::dragOutline()
{
  double  r = get_Rotation();

  return rotatedDragOutline(r);
}

QPolygonF QxrdROICoordinates::rotatedDragOutline(double r)
{
  QPolygonF res;
  QPointF c = get_Center();
  QMatrix m;

  m.rotate(r);

  if (m_OuterShape) {
    QPolygonF s = m_OuterShape->dragOutline();

    for (int i=0; i<s.count(); i++) {
      res.append(c + m.map(s.value(i)));
    }
  }

  return res;
}

QPolygonF QxrdROICoordinates::innerOutline()
{
  QPolygonF res;
  QPointF c = get_Center();
  double  r = get_Rotation();
  QMatrix m;

  m.rotate(r);

  if (m_InnerShape) {
    QPolygonF s = m_InnerShape->dragOutline();

    for (int i=0; i<s.count(); i++) {
      res.append(c + m.map(s.value(i)));
    }
  }

  return res;
}

QPolygonF QxrdROICoordinates::outerOutline()
{
  QPolygonF res;
  QPointF c = get_Center();
  double  r = get_Rotation();
  QMatrix m;

  m.rotate(r);

  if (m_OuterShape) {
    QPolygonF s = m_OuterShape->dragOutline();

    for (int i=0; i<s.count(); i++) {
      res.append(c + m.map(s.value(i)));
    }
  }

  return res;
}

QPolygonF QxrdROICoordinates::scaledInnerOutline(double kx, double ky)
{
  QPolygonF res;
  QPointF c = get_Center();
  double  r = get_Rotation();
  QMatrix m;

  m.rotate(r);

  if (m_InnerShape) {
    QPolygonF s = m_InnerShape->dragOutline();

    for (int i=0; i<s.count(); i++) {
      QPointF p = s.value(i);

      res.append(c + m.map(QPointF(p.x()*kx, p.y()*ky)));
    }
  }

  return res;
}

QPolygonF QxrdROICoordinates::scaledOuterOutline(double kx, double ky)
{
  QPolygonF res;
  QPointF c = get_Center();
  double  r = get_Rotation();
  QMatrix m;

  m.rotate(r);

  if (m_OuterShape) {
    QPolygonF s = m_OuterShape->dragOutline();

    for (int i=0; i<s.count(); i++) {
      QPointF p = s.value(i);

      res.append(c + m.map(QPointF(p.x()*kx, p.y()*ky)));
    }
  }

  return res;
}

QPolygonF QxrdROICoordinates::scaledDragOutline(double kx, double ky)
{
  return scaledOuterOutline(kx, ky);
}

void QxrdROICoordinates::scaleROI(int innerOuter, double kx, double ky)
{
  if (m_OuterShape && innerOuter == OuterShape) {
    m_OuterShape->scale(kx, ky);
  }

  if (m_InnerShape && innerOuter == InnerShape) {
    m_InnerShape->scale(kx, ky);
  }
}

void QxrdROICoordinates::deleteROIPoint(int innerOuter, int n)
{
  if (innerOuter == InnerShape && m_InnerShape) {
    m_InnerShape->deleteROIPoint(n);
  } else if (m_OuterShape) {
    m_OuterShape->deleteROIPoint(n);
  }
}

void QxrdROICoordinates::changeROIPoint(int innerOuter, int n, QPointF pt)
{
  if (innerOuter == InnerShape && m_InnerShape) {
    m_InnerShape->changeROIPoint(n, invTransform(pt));
  } else if (m_OuterShape) {
    m_OuterShape->changeROIPoint(n, invTransform(pt));
  }
}

void QxrdROICoordinates::insertROIPoint(int innerOuter, int n, QPointF pt)
{
  if (innerOuter == InnerShape && m_InnerShape) {
    m_InnerShape->insertROIPoint(n, invTransform(pt));
  } else if (m_OuterShape) {
    m_OuterShape->insertROIPoint(n, invTransform(pt));
  }
}

void QxrdROICoordinates::recalculate(QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
  recalculatePrivate(img, mask, VisualizeNone);
}

void QxrdROICoordinates::visualizeBackground(QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
  int tp = m_Bounds.top();
  int bt = m_Bounds.bottom();
  int lf = m_Bounds.left();
  int rt = m_Bounds.right();

  if (img) {
    img->clear();

    for (int row=tp; row<=bt; row++) {
      for (int col=lf; col<=rt; col++) {
        img->setImageData(col, row, m_Cache->getPoint(col, row) == QxrdROICache::OuterMask);
      }
    }
  }
}

void QxrdROICoordinates::visualizePeak(QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
  int tp = m_Bounds.top();
  int bt = m_Bounds.bottom();
  int lf = m_Bounds.left();
  int rt = m_Bounds.right();

  if (img) {
    img->clear();

    for (int row=tp; row<=bt; row++) {
      for (int col=lf; col<=rt; col++) {
        img->setImageData(col, row, m_Cache->innerPoint(col, row));
      }
    }
  }
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

static int inRange(int minVal, int val, int maxVal)
{
  return qMin(qMax(minVal,val),maxVal);
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
    double  cx = c.x();
    double  cy = c.y();

    if (get_Changed()) {
      double  r = get_Rotation();
      QMatrix m, mn;
      m.rotate(r);
      mn.rotate(-r);

      QRectF innerBounds = m.mapRect(m_InnerShape->boundingRect()).translated(c);
      QRectF outerBounds = m.mapRect(m_OuterShape->boundingRect()).translated(c);

      QRectF bounds = innerBounds | outerBounds;

      m_Bounds = bounds.toRect();
      m_InnerBounds = innerBounds.toRect();
      m_OuterBounds = outerBounds.toRect();

      m_Cache->redimension(m_Bounds);

      int tp = m_Bounds.top();
      int bt = m_Bounds.bottom();
      int lf = m_Bounds.left();
      int rt = m_Bounds.right();

      for (int row=tp; row<=bt; row++) {
        for (int col=lf; col<=rt; col++) {
          QPointF p = mn.map(QPointF(col,row)-c);

          m_Cache->setPoint(col,row,
                            m_InnerShape->pointInShape(p),
                            m_OuterShape->pointInShape(p));
        }
      }
    }

    int first = true;

    double min = 0;
    double max = 0;
    double sum = 0;
    double npx = 0;
    int    nnan = 0;

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

    int tp = inRange(0,m_Bounds.top(),img->get_Height()-1);
    int bt = inRange(0,m_Bounds.bottom(),img->get_Height()-1);
    int lf = inRange(0,m_Bounds.left(),img->get_Width()-1);
    int rt = inRange(0,m_Bounds.right(),img->get_Width()-1);

    double insum = 0;
    double outsum = 0;

    for (int row=tp; row<=bt; row++) {
      double dy = row - cy;

      for (int col=lf; col<=rt; col++) {
        double dx = col - cx;

        if (mask == NULL || mask->value(col, row)) {
          int code = m_Cache->getPoint(col, row);

          double val = img->getImageData(col, row);

          if (QxrdROICache::innerPoint(code)) {
            // Peak point, skip for now...
            ninner += 1;

            if (val == val) {
              insum += val;
            }

          } else if (QxrdROICache::outerPoint(code)) {
            nouter += 1;
            // Background point...

            if (val == val) {
              outsum += val;

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

    tp = inRange(0,m_InnerBounds.top()-1,img->get_Height()-1);
    bt = inRange(0,m_InnerBounds.bottom()+1,img->get_Height()-1);
    lf = inRange(0,m_InnerBounds.left()-1,img->get_Width()-1);
    rt = inRange(0,m_InnerBounds.right()+1,img->get_Width()-1);

    for (int row=tp; row<=bt; row++) {
      double dy = row - cy;

      for (int col=lf; col<=rt; col++) {
        double dx = col - cx;

        if (mask == NULL || mask->value(col, row)) {
          if (m_Cache->innerPoint(col, row)) {
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
            } else {
              nnan += 1;
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

    set_InnerSum(insum);
    set_OuterSum(outsum);

    set_Changed(false);
  }

#ifndef QT_NO_DEBUG
  printMessage(tr("ROI Calculation took %1 msec").arg(tic.elapsed()));
  printMessage(tr("Inner #%1, Outer #%2").arg(ninner).arg(nouter));
#endif
}

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
  res.append(get_InnerSum());
  res.append(get_OuterSum());

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
  case InnerSum:
    res = "Inner Sum";
    break;
  case OuterSum:
    res = "Outer Sum";
    break;
  }

  return res;
}
