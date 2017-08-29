#ifndef QXRDROISHAPE_H
#define QXRDROISHAPE_H

#include "qxrdlib_global.h"
#include "qcepserializableobject.h"
#include "qxrdroishape-ptr.h"

class QXRD_EXPORT QxrdROIShape : public QcepSerializableObject
{
  Q_OBJECT

public:
  enum ROIShapeType {
    NoShape,
    RectangleShape,
    EllipseShape,
    PolygonShape,
    ShapeTypeCount
  };

  Q_ENUM(ROIShapeType)

  QxrdROIShape(ROIShapeType shapeType);

  QxrdROIShape(int    shapeType,
               QRectF boundingBox);
signals:
  void roiChanged();

public:
  static QxrdROIShapePtr newROIShape(int shapeType, double scale);
  static int             roiTypeCount();
  static QString         roiTypeName(int roiType);

  static QScriptValue toScriptValue(QScriptEngine *engine, const QxrdROIShapePtr &shape);
  static void fromScriptValue(const QScriptValue &obj, QxrdROIShapePtr &shape);

  virtual int markerCount() = 0;
  virtual QPointF markerPoint(int n) = 0;
  virtual QRectF boundingRect() = 0;
  virtual bool pointInShape(QPointF pt) = 0;
  virtual QPolygonF dragOutline() = 0;
  virtual void scale(double kx, double ky) = 0;
  virtual void deleteROIPoint(int n) = 0;
  virtual void changeROIPoint(int n, QPointF pt) = 0;
  virtual void insertROIPoint(int n, QPointF pt) = 0;

public:
  Q_PROPERTY(int shapeType READ get_ShapeType WRITE set_ShapeType STORED false)
  QCEP_INTEGER_PROPERTY(ShapeType)
};

#endif // QXRDROISHAPE_H
