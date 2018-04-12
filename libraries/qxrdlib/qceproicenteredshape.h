#ifndef QCEPROICENTEREDSHAPE_H
#define QCEPROICENTEREDSHAPE_H

#include "qceplib_global.h"
#include "qceproishape.h"

class QCEP_EXPORT QcepROICenteredShape : public QcepROIShape
{
  Q_OBJECT

private:
  typedef QcepROIShape inherited;

public:
  QcepROICenteredShape(QString      name,
                       ROIShapeType shapeType,
                       double       halfWidth,
                       double       halfHeight);

  virtual QRectF boundingRect();
  virtual void scale(double kx, double ky);
  virtual void deleteROIPoint(int n);
  virtual void changeROIPoint(int n, QPointF pt);
  virtual void insertROIPoint(int n, QPointF pt);

public:
  Q_PROPERTY(double halfWidth READ get_HalfWidth WRITE set_HalfWidth)
  QCEP_DOUBLE_PROPERTY(HalfWidth)

  Q_PROPERTY(double halfHeight READ get_HalfHeight WRITE set_HalfHeight)
  QCEP_DOUBLE_PROPERTY(HalfHeight)
};

#endif // QCEPROICENTEREDSHAPE_H
