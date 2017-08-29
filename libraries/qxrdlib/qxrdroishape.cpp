#include "qxrdroishape.h"
#include "qxrdroirectangle.h"
#include "qxrdroiellipse.h"
#include "qxrdroipolygon.h"
#include <QScriptEngine>

QxrdROIShape::QxrdROIShape(QxrdROIShape::ROIShapeType shapeType)
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
