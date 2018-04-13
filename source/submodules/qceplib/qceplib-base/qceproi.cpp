#include "qceproi.h"
#include <QtMath>
#include <QMatrix4x4>
#include "qceproishape.h"
#include "qcepmutexlocker.h"
#include "qcepproperty.h"
#include "qceproicache.h"
#include "qcepimagedata.h"
#include "qcepmaskdata.h"

QcepROI::QcepROI(int roiOuterType, int roiInnerType)
  : inherited("coords"),
    m_RoiOuterType(this, "roiOuterType", roiOuterType, "ROI Outer Type"),
    m_RoiInnerType(this, "roiInnerType", roiInnerType, "ROI Inner Type"),
//    m_RoiOuterTypeName(this, "roiOuterTypeName", QcepROIShape::roiTypeName(roiOuterType), "ROI Outer Type Name"),
//    m_RoiInnerTypeName(this, "roiInnerTypeName", QcepROIShape::roiTypeName(roiInnerType), "ROI Inner Type Name"),
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
    m_OuterShape(QcepROIShape::newROIShape("outerShape", roiOuterType, 1.0)),
    m_InnerShape(QcepROIShape::newROIShape("innerShape", roiInnerType, 0.25)),
    m_Cache(new QcepROICache()),
    m_Bounds(QRect()),
    m_InnerBounds(QRect()),
    m_OuterBounds(QRect())
{
  if (m_OuterShape) {
    connect(m_OuterShape.data(), &QcepROIShape::roiChanged, this, &QcepROI::changed);
  }

  if (m_InnerShape) {
    connect(m_InnerShape.data(), &QcepROIShape::roiChanged, this, &QcepROI::changed);
  }

  connect(prop_Center(), &QcepDoublePointProperty::valueChanged, this, &QcepROI::changed);
  connect(prop_Rotation(), &QcepDoubleProperty::valueChanged, this, &QcepROI::changed);

  changed();
}

void QcepROI::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  if (m_InnerShape) {
    m_InnerShape -> initialize(sharedFromThis());
  }

  if (m_OuterShape) {
    m_OuterShape -> initialize(sharedFromThis());
  }

  if (m_Cache) {
    m_Cache      -> initialize(sharedFromThis());
  }
}

QcepROIPtr QcepROI::readROI(QcepObjectWPtr parent, QSettings *settings)
{
  QcepROIPtr res =
      QcepROIPtr(new QcepROI(NoBounds, NoBounds));

  if (res) {
    res -> initialize(parent);

    res -> readSettings(settings);
  }

  return res;
}

int QcepROI::innerType()
{
  return get_RoiInnerType();
}

QString QcepROI::innerTypeName()
{
  return QcepROIShape::roiTypeName(get_RoiInnerType());
}

void QcepROI::changeInnerType(int t)
{
  if (get_RoiInnerType() != t) {
    set_RoiInnerType(t);

    m_InnerShape = QcepROIShape::newROIShape("innerShape", t, 0.25);

    m_InnerShape -> initialize(sharedFromThis());

    connect(m_InnerShape.data(), &QcepROIShape::roiChanged, this, &QcepROI::changed);

    changed();
  }
}

int QcepROI::outerType()
{
  return get_RoiOuterType();
}

QString QcepROI::outerTypeName()
{
  return QcepROIShape::roiTypeName(get_RoiOuterType());
}

void QcepROI::changeOuterType(int t)
{
  if (get_RoiOuterType() != t) {
    set_RoiOuterType(t);

    m_OuterShape = QcepROIShape::newROIShape("outerShape", t, 1.0);

    m_OuterShape -> initialize(sharedFromThis());

    connect(m_OuterShape.data(), &QcepROIShape::roiChanged, this, &QcepROI::changed);

    changed();
  }
}

QcepROI::~QcepROI()
{
}

void QcepROI::writeSettings(QSettings *settings)
{
  inherited::writeSettings(settings);

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

void QcepROI::readSettings(QSettings *settings)
{
  inherited::readSettings(settings);

  m_InnerShape = QcepROIShape::newROIShape("innerShape", get_RoiInnerType(), 0.25);
  m_OuterShape = QcepROIShape::newROIShape("outerShape", get_RoiOuterType(), 1.00);

  if (m_OuterShape) {
    m_OuterShape -> initialize(sharedFromThis());
    settings     -> beginGroup("outer");
    m_OuterShape -> readSettings(settings);
    settings     -> endGroup();
  }

  if (m_InnerShape) {
    m_InnerShape -> initialize(sharedFromThis());
    settings     -> beginGroup("inner");
    m_InnerShape -> readSettings(settings);
    settings     -> endGroup();
  }

  changed();
}

QScriptValue QcepROI::toScriptValue(QScriptEngine *engine, const QcepROIPtr &coords)
{
  return engine->newQObject(coords.data());
}

void QcepROI::fromScriptValue(const QScriptValue &obj, QcepROIPtr &coords)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QcepROI *qcoords = qobject_cast<QcepROI*>(qobj);

    if (qcoords) {
      coords = QcepROIPtr(qcoords);
    }
  }
}

int QcepROI::roiTypeID(int outerType, int innerType)
{
  return (outerType & 15) + ((innerType & 15)<<4);
}

QString QcepROI::roiTypeName(int outerType, int innerType)
{
  if (outerType == QcepROIShape::NoShape) {
    return QcepROIShape::roiTypeName(innerType);
  } else {
    return QcepROIShape::roiTypeName(innerType) + " in " + QcepROIShape::roiTypeName(outerType);
  }
}

int QcepROI::roiOuterType(int roiTypeID)
{
  return roiTypeID & 15;
}

int QcepROI::roiInnerType(int roiTypeID)
{
  return (roiTypeID>>4) & 15;
}

QcepROIPtr QcepROI::newROICoordinates(int roiTypeID)
{
  int outerType = roiOuterType(roiTypeID);
  int innerType = roiInnerType(roiTypeID);

  QcepROIPtr res = QcepROIPtr(
        new QcepROI(outerType, innerType));

  return res;
}

QcepROIShapePtr QcepROI::outer() const
{
  return m_OuterShape;
}

QcepROIShapePtr QcepROI::inner() const
{
  return m_InnerShape;
}

void QcepROI::selectNamedROIOuterType(QString nm)
{
  for (int i=0; i<QcepROIShape::roiTypeCount(); i++) {
    if (QcepROIShape::roiTypeName(i) == nm) {
      set_RoiOuterType(i);
      emit roiChanged();
      return;
    }
  }
}

void QcepROI::selectNamedROIInnerType(QString nm)
{
  for (int i=0; i<QcepROIShape::roiTypeCount(); i++) {
    if (QcepROIShape::roiTypeName(i) == nm) {
      set_RoiInnerType(i);
      emit roiChanged();
      return;
    }
  }
}

void QcepROI::changed()
{
  set_Changed(true);

  emit roiChanged();
}

void QcepROI::setCenter(QPointF c)
{
  set_Center(c);
}

void QcepROI::setCenterX(double cx)
{
  QPointF c = get_Center();

  c.setX(cx);

  set_Center(c);
}

void QcepROI::setCenterY(double cy)
{
  QPointF c = get_Center();

  c.setY(cy);

  set_Center(c);
}

void QcepROI::setRotation(double r)
{
  set_Rotation(r);
}

QPointF QcepROI::transform(const QPointF &pt) const
{
  QPointF res;
  QPointF c = get_Center();
  double  r = get_Rotation();
  QMatrix m;
  m.rotate(r);

  res = c + m.map(pt);

  return res;
}

QPointF QcepROI::invTransform(const QPointF &pt) const
{
  QPointF res;

  QPointF c = get_Center();
  double  r = get_Rotation();
  QMatrix m;
  m.rotate(-r);

  res = m.map(pt - c);

  return res;
}

QVector<QPointF> QcepROI::markerCoords()
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

double QcepROI::nearestDistance(QPointF pt)
{
  QVector<QPointF> markers = markerCoords();
  double nearest = qInf();

  for (int i=0; i<markers.count(); i++) {
    QPointF d = pt-markers.value(i);

    double dist = sqrt(d.x()*d.x() + d.y()*d.y());

    if (dist < nearest) {
      nearest = dist;
    }
  }

  return nearest;
}

QPolygonF QcepROI::dragOutline()
{
  double  r = get_Rotation();

  return rotatedDragOutline(r);
}

QPolygonF QcepROI::rotatedDragOutline(double r)
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
  } else if (m_InnerShape) {
    QPolygonF s = m_InnerShape->dragOutline();

    for (int i=0; i<s.count(); i++) {
      res.append(c + m.map(s.value(i)));
    }
  }

  return res;
}

QPolygonF QcepROI::innerOutline()
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

QPolygonF QcepROI::outerOutline()
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

QPolygonF QcepROI::scaledInnerOutline(double kx, double ky)
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

QPolygonF QcepROI::scaledOuterOutline(double kx, double ky)
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

QPolygonF QcepROI::scaledDragOutline(double kx, double ky)
{
  return scaledOuterOutline(kx, ky);
}

void QcepROI::scaleROI(int innerOuter, double kx, double ky)
{
  if (m_OuterShape && innerOuter == OuterShape) {
    m_OuterShape->scale(kx, ky);
  }

  if (m_InnerShape && innerOuter == InnerShape) {
    m_InnerShape->scale(kx, ky);
  }
}

void QcepROI::scaleROI(double kx, double ky)
{
  scaleROI(OuterShape, kx, ky);
  scaleROI(InnerShape, kx, ky);
}

void QcepROI::deleteROIPoint(int innerOuter, int n)
{
  if (innerOuter == InnerShape && m_InnerShape) {
    m_InnerShape->deleteROIPoint(n);
  } else if (m_OuterShape) {
    m_OuterShape->deleteROIPoint(n);
  }
}

void QcepROI::changeROIPoint(int innerOuter, int n, QPointF pt)
{
  if (innerOuter == InnerShape && m_InnerShape) {
    m_InnerShape->changeROIPoint(n, invTransform(pt));
  } else if (m_OuterShape) {
    m_OuterShape->changeROIPoint(n, invTransform(pt));
  }
}

void QcepROI::insertROIPoint(int innerOuter, int n, QPointF pt)
{
  if (innerOuter == InnerShape && m_InnerShape) {
    m_InnerShape->insertROIPoint(n, invTransform(pt));
  } else if (m_OuterShape) {
    m_OuterShape->insertROIPoint(n, invTransform(pt));
  }
}

void QcepROI::recalculate(QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
  recalculatePrivate(img, mask, VisualizeNone);
}

void QcepROI::visualizeBackground(QcepImageDataBasePtr img, QcepMaskDataPtr /*mask*/)
{
  int tp = m_Bounds.top();
  int bt = m_Bounds.bottom();
  int lf = m_Bounds.left();
  int rt = m_Bounds.right();

  if (img) {
    img->clear();

    for (int row=tp; row<=bt; row++) {
      for (int col=lf; col<=rt; col++) {
        img->setImageData(col, row, m_Cache->getPoint(col, row) == QcepROICache::OuterMask);
      }
    }
  }
}

void QcepROI::visualizePeak(QcepImageDataBasePtr img, QcepMaskDataPtr /*mask*/)
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

bool QcepROI::pointInOuter(QPointF pt)
{
  QcepROIShapePtr sh = outer();

  if (sh) {
    QPointF ptf = invTransform(pt);

    return sh->pointInShape(ptf);
  } else {
    return false;
  }
}

bool QcepROI::pointInInner(QPointF pt)
{
  QcepROIShapePtr sh = inner();

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

void QcepROI::recalculatePrivate(QcepImageDataBasePtr img, QcepMaskDataPtr mask, int /*vis*/)
{
#ifndef QT_NO_DEBUG
  printf("Recalculate ROI, inner: %s, outer: %s\n",
         qPrintable(innerTypeName()),
         qPrintable(outerTypeName()));
#endif

  QTime tic;

  tic.start();

  int nouter = 0;
  int ninner = 0;

  if (m_InnerShape && img) {
    if (m_OuterShape) {
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

            if (QcepROICache::innerPoint(code)) {
              // Peak point, skip for now...
              ninner += 1;

              if (val == val) {
                insum += val;
              }

            } else if (QcepROICache::outerPoint(code)) {
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
    } else { // Only an inner shape - no background fitting...
      QPointF c = get_Center();
//      double  cx = c.x();
//      double  cy = c.y();

      if (get_Changed()) {
        double  r = get_Rotation();
        QMatrix m, mn;
        m.rotate(r);
        mn.rotate(-r);

        QRectF bounds = m.mapRect(m_InnerShape->boundingRect()).translated(c);

        m_Bounds = bounds.toRect();

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
                              false);
          }
        }
      }

      int first = true;

      double min = 0;
      double max = 0;
      double sum = 0;
      double npx = 0;
      int    nnan = 0;

      int tp = inRange(0,m_Bounds.top(),img->get_Height()-1);
      int bt = inRange(0,m_Bounds.bottom(),img->get_Height()-1);
      int lf = inRange(0,m_Bounds.left(),img->get_Width()-1);
      int rt = inRange(0,m_Bounds.right(),img->get_Width()-1);

      double insum = 0;

      for (int row=tp; row<=bt; row++) {
        for (int col=lf; col<=rt; col++) {
          if (mask == NULL || mask->value(col, row)) {
            int code = m_Cache->getPoint(col, row);

            double val = img->getImageData(col, row);

            if (QcepROICache::innerPoint(code)) {
              ninner += 1;

              if (val == val) {
                insum += val;
                sum += val;
                npx += 1;

                if (first) {
                  min = val;
                  max = val;
                  first = false;
                } else if (val > max) {
                  max = val;
                } else if (val < min) {
                  min = val;
                }
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

      set_Background(0);
      set_XGradient(0);
      set_YGradient(0);

      set_InnerSum(insum);
      set_OuterSum(0);

      set_Changed(false);
    }
  }

#ifndef QT_NO_DEBUG
  printMessage(tr("ROI Calculation took %1 msec").arg(tic.elapsed()));
  printMessage(tr("Inner #%1, Outer #%2").arg(ninner).arg(nouter));
#endif
}

QVector<double> QcepROI::values() const
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

int QcepROI::outputCount()
{
  return OutputCount;
}

QString QcepROI::outputName(int opt)
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
