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
  static QxrdROIShapePtr newROIShape(int shapeType);
  static int             roiTypeCount();
  static QString         roiTypeName(int roiType);

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

public:
  Q_PROPERTY(int shapeType READ get_ShapeType WRITE set_ShapeType)
  QCEP_INTEGER_PROPERTY(ShapeType)

  Q_PROPERTY(QRectF coords READ get_Coords WRITE set_Coords)
  QCEP_DOUBLE_RECT_PROPERTY(Coords)
};

#endif // QXRDROISHAPE_H
