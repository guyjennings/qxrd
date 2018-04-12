#include "qceproishape.h"
#include "qceproirectangle.h"
#include "qceproiellipse.h"
#include "qceproipolygon.h"
#include <QScriptEngine>

QcepROIShape::QcepROIShape(QString                    name,
                           QcepROIShape::ROIShapeType shapeType)
  : QcepObject(name),
    m_ShapeType(this, "shapeType", shapeType, "ROI Shape Type")
{
}

QcepROIShapePtr QcepROIShape::newROIShape(QString name, int shapeType, double scale)
{
  QcepROIShapePtr res;

  switch (shapeType) {
  case RectangleShape:
    res = QcepROIShapePtr(new QcepROIRectangle(name, scale));
    break;

  case EllipseShape:
    res = QcepROIShapePtr(new QcepROIEllipse(name, scale));
    break;

  case PolygonShape:
    res = QcepROIShapePtr(new QcepROIPolygon(name, scale));
    break;
  }

  return res;
}

QString QcepROIShape::roiTypeName(int roiType)
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

QScriptValue QcepROIShape::toScriptValue(QScriptEngine *engine, const QcepROIShapePtr &shape)
{
  return engine->newQObject(shape.data());
}

void QcepROIShape::fromScriptValue(const QScriptValue &obj, QcepROIShapePtr &shape)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QcepROIShape *qshape = qobject_cast<QcepROIShape*>(qobj);

    if (qshape) {
      shape = QcepROIShapePtr(qshape);
    }
  }
}

int QcepROIShape::roiTypeCount()
{
  return ShapeTypeCount;
}
