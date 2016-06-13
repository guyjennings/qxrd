#ifndef QXRDROISHAPE_H
#define QXRDROISHAPE_H

#include "qcepserializableobject.h"
#include "qxrdroishape-ptr.h"

class QxrdROIShape : public QcepSerializableObject
{
  Q_OBJECT

public:
  QxrdROIShape(int shapeType,
               double left,
               double top,
               double right,
               double bottom);

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

  double left() const;
  double top() const;
  double right() const;
  double bottom() const;
  double width() const;
  double height() const;
  QPointF center() const;
  QSizeF size() const;

  void setLeft(double l);
  void setTop(double t);
  void setRight(double r);
  void setBottom(double b);

  void setCenter(QPointF c);
  void setCenter(double cx, double cy);
  void setSize(QSizeF s);
  void setSize(double w, double h);

  void setCenterX(double cx);
  void setCenterY(double cy);
  void setWidth(double w);
  void setHeight(double h);

  virtual int markerCount() = 0;
  virtual QPointF markerPoint(int n) = 0;
  virtual QRectF boundingRect() = 0;
  virtual bool pointInShape(QPointF pt) = 0;

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

  Q_PROPERTY(QRectF coords READ get_Coords WRITE set_Coords)
  QCEP_DOUBLE_RECT_PROPERTY(Coords)
};

#endif // QXRDROISHAPE_H
