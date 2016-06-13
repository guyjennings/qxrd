#include "qxrdroishape.h"
#include "qxrdroirectangle.h"
#include "qxrdroiellipse.h"
#include "qxrdroipolygon.h"
#include <QScriptEngine>

QxrdROIShape::QxrdROIShape(int shapeType, double left, double top, double right, double bottom)
  : QcepSerializableObject("shape"),
    m_ShapeType(this, "shapeType", shapeType, "ROI Shape Type"),
    m_Coords(this, "coords", QRectF(left, top, right-left, bottom-top), "ROI Shape Bounding Rect")
{
}

QxrdROIShape::QxrdROIShape(int    shapeType, QRectF boundingBox)
  : QcepSerializableObject("shape"),
    m_ShapeType(this, "shapeType", shapeType, "ROI Shape Type"),
    m_Coords(this, "coords", boundingBox, "ROI Shape Bounding Rect")
{
}

QxrdROIShapePtr QxrdROIShape::newROIShape(int shapeType, double scale)
{
  QxrdROIShapePtr res;

  switch (shapeType) {
  case RectangleShape:
    res = QxrdROIShapePtr(new QxrdROIRectangle(scale));
    break;

  case EllipseShape:
    res = QxrdROIShapePtr(new QxrdROIEllipse(scale));
    break;

  case PolygonShape:
    res = QxrdROIShapePtr(new QxrdROIPolygon(scale));
    break;
  }

  return res;
}

QString QxrdROIShape::roiTypeName(int roiType)
{
  if (roiType == NoShape) {
    return "None";
  } else if (roiType == RectangleShape) {
    return "Rectangle";
  } else if (roiType == EllipseShape) {
    return "Ellipse";
  } else if (roiType == PolygonShape) {
    return "Polygon";
  }

  return "";
}

QScriptValue QxrdROIShape::toScriptValue(QScriptEngine *engine, const QxrdROIShapePtr &shape)
{
  return engine->newQObject(shape.data());
}

void QxrdROIShape::fromScriptValue(const QScriptValue &obj, QxrdROIShapePtr &shape)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QxrdROIShape *qshape = qobject_cast<QxrdROIShape*>(qobj);

    if (qshape) {
      shape = QxrdROIShapePtr(qshape);
    }
  }
}

int QxrdROIShape::roiTypeCount()
{
  return ShapeTypeCount;
}

double QxrdROIShape::left() const
{
  return get_Coords().left();
}

double QxrdROIShape::top() const
{
  return get_Coords().top();
}

double QxrdROIShape::right() const
{
  return get_Coords().right();
}

double QxrdROIShape::bottom() const
{
  return get_Coords().bottom();
}

double QxrdROIShape::width() const
{
  return get_Coords().width();
}

double QxrdROIShape::height() const
{
  return get_Coords().height();
}

QPointF QxrdROIShape::center() const
{
  return get_Coords().center();
}

QSizeF QxrdROIShape::size() const
{
  return get_Coords().size();
}

void QxrdROIShape::setLeft(double l)
{
  QRectF c = get_Coords();

  c.setLeft(l);

  set_Coords(c);

  emit roiChanged();
}

void QxrdROIShape::setTop(double t)
{
  QRectF c = get_Coords();

  c.setTop(t);

  set_Coords(c);

  emit roiChanged();
}

void QxrdROIShape::setRight(double r)
{
  QRectF c = get_Coords();

  c.setRight(r);

  set_Coords(c);

  emit roiChanged();
}

void QxrdROIShape::setBottom(double b)
{
  QRectF c = get_Coords();

  c.setBottom(b);

  set_Coords(c);

  emit roiChanged();
}

void QxrdROIShape::setCenter(QPointF c)
{
  QRectF crd = get_Coords();

  crd.moveCenter(c);

  set_Coords(crd);

  emit roiChanged();
}

void QxrdROIShape::setCenter(double cx, double cy)
{
  setCenter(QPointF(cx, cy));
}

void QxrdROIShape::setSize(QSizeF s)
{
  QRectF c = get_Coords();
  QPointF cen = c.center();

  c.setSize(s);
  c.moveCenter(cen);

  set_Coords(c);

  emit roiChanged();
}

void QxrdROIShape::setSize(double w, double h)
{
  setSize(QSizeF(w,h));
}

void QxrdROIShape::setCenterX(double cx)
{
  QRectF  c = get_Coords();
  QPointF cen = c.center();

  cen.setX(cx);
  c.moveCenter(cen);

  set_Coords(c);

  emit roiChanged();
}

void QxrdROIShape::setCenterY(double cy)
{
  QRectF  c = get_Coords();
  QPointF cen = c.center();

  cen.setY(cy);
  c.moveCenter(cen);

  set_Coords(c);

  emit roiChanged();
}

void QxrdROIShape::setWidth(double w)
{
  QRectF c = get_Coords();
  QPointF cen = c.center();

  c.setWidth(w);
  c.moveCenter(cen);

  set_Coords(c);

  emit roiChanged();
}

void QxrdROIShape::setHeight(double h)
{
  QRectF c = get_Coords();
  QPointF cen = c.center();

  c.setHeight(h);
  c.moveCenter(cen);

  set_Coords(c);

  emit roiChanged();
}

double QxrdROIShape::normedDistance(QPointF pt, QPointF delta)
{
  double dnx = pt.x() / delta.x();
  double dny = pt.y() / delta.y();

  return sqrt(dnx*dnx + dny*dny);
}

double QxrdROIShape::normedDistanceToPoint(QPointF p1, QPointF p2, QPointF delta)
{
  double res = normedDistance(p1 - p2, delta);

  if (res < 2) {
    printf("Normed Distance from [%g,%g] to [%g,%g] = %g\n", p1.x(), p1.y(), p2.x(), p2.y(), res);
  }

  return res;
}

double QxrdROIShape::normedDistanceToLine(QPointF p0, QPointF p1, QPointF p2, QPointF delta, QPointF &nearpt)
{
  QLineF l2(p0, p1);
  QLineF l0(p1, p2);
  QLineF l1(p2, p0);

  double a0 = 180.0 - l1.angleTo(l2);
  double a1 = 180.0 - l2.angleTo(l0);
  double a2 = 180.0 - l0.angleTo(l1);

  if (fabs(a1) < 90 && fabs(a2 < 90)) {
    printf("Normed Distance from [%g,%g] to line [%g,%g]-[%g,%g]\n",
           p0.x(), p0.y(),
           p1.x(), p1.y(),
           p2.x(), p2.y()
           );

    printf("Angles %g,%g,%g : sum %g\n", a0,a1,a2, a0+a1+a2);
  }

  return qInf();
}

bool QxrdROIShape::findNearest(QPointF pt, QPointF delta, int &type, int &index, QPointF &ptn)
{
  double dist = qInf();
  type = MatchedNone;
  index = -1;

  for (int i=0; i<markerCount(); i++) {
    QPointF mpt = markerPoint(i);
    double nDist = normedDistanceToPoint(pt, mpt, delta);

    if (nDist < dist) {
      dist = nDist;
      type = MatchedPoint;
      index = i;
      ptn = mpt;
    }
  }

  if (dist > 1) {
    for (int i=1; i<markerCount(); i++) {
      QPointF mp0 = markerPoint(i-1);
      QPointF mp1 = markerPoint(i);
      QPointF mpt;
      double nDist = normedDistanceToLine(pt, mp0, mp1, delta, mpt);

      if (nDist < dist) {
        dist = nDist;
        type = MatchedLine;
        index = i;
        ptn = mpt;
      }
    }
  }

  return (dist < 1);
}

