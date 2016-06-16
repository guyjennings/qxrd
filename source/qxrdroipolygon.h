#ifndef QXRDROIPOLYGON_H
#define QXRDROIPOLYGON_H

#include "qxrdroishape.h"

class QxrdROIPolygon : public QxrdROIShape
{
  Q_OBJECT

public:
  QxrdROIPolygon(QPolygonF poly);
  QxrdROIPolygon(double scale);

public slots:
  void clear();
  void append(double x, double y);
  void append(QPointF pt);

  void setPoint(int n, QPointF val);

  int count();
  QPointF point(int n);
  double  x(int n);
  double  y(int n);

  virtual int markerCount();
  virtual QPointF markerPoint(int n);
  virtual QRectF boundingRect();
  virtual bool pointInShape(QPointF pt);
  virtual QPolygonF dragOutline();
  virtual void scale(double kx, double ky);

public:
  Q_PROPERTY(QcepPolygon poly READ get_Poly WRITE set_Poly)
  QCEP_POLYGON_PROPERTY(Poly)
};

#endif // QXRDROIPOLYGON_H
