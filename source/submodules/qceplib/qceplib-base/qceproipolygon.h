#ifndef QCEPROIPOLYGON_H
#define QCEPROIPOLYGON_H

#include "qceplib_global.h"
#include "qceproishape.h"

class QCEP_EXPORT QcepROIPolygon : public QcepROIShape
{
  Q_OBJECT

private:
  typedef QcepROIShape inherited;

public:
  QcepROIPolygon(QString name, QPolygonF poly);
  QcepROIPolygon(QString name, double scale);

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

  virtual void deleteROIPoint(int n);
  virtual void changeROIPoint(int n, QPointF pt);
  virtual void insertROIPoint(int n, QPointF pt);

public:
  Q_PROPERTY(QcepPolygon poly READ get_Poly WRITE set_Poly)
  QCEP_POLYGON_PROPERTY(Poly)
};

#endif // QCEPROIPOLYGON_H
