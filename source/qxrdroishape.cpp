#include "qxrdroishape.h"
#include "qxrdroirectangle.h"
#include "qxrdroiellipse.h"
#include "qxrdroipolygon.h"
#include <QScriptEngine>

QxrdROIShape::QxrdROIShape(int shapeType)
  : QcepSerializableObject("shape"),
    m_ShapeType(this, "shapeType", shapeType, "ROI Shape Type")
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

