#ifndef QXRDROISHAPE_H
#define QXRDROISHAPE_H

#include "qcepserializableobject.h"
#include "qxrdroishape-ptr.h"

class QxrdROIShape : public QcepSerializableObject
{
  Q_OBJECT

public:
  QxrdROIShape(int shapeType/*,
               double left,
               double top,
               double right,
               double bottom*/);

  QxrdROIShape(int    shapeType,
               QRectF boundingBox);
  enum {
    NoShape,
    RectangleShape,
    EllipseShape,
    PolygonShape,
    ShapeTypeCount
  };

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

  static double normedDistance(QPointF pt, QPointF delta);
  static double normedDistanceToPoint(QPointF p1, QPointF p2, QPointF delta);
  static double normedDistanceToLine(QPointF pt, QPointF p1, QPointF p2, QPointF delta, QPointF &nearpt);

  enum {
    MatchedNone,
    MatchedPoint,
    MatchedLine
  };

  bool findNearest(QPointF pt, QPointF delta, int &type, int &index, QPointF &ptn);

public:
  Q_PROPERTY(int shapeType READ get_ShapeType WRITE set_ShapeType)
  QCEP_INTEGER_PROPERTY(ShapeType)
};

#endif // QXRDROISHAPE_H
