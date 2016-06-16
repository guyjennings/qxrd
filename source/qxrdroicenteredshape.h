#ifndef QXRDROICENTEREDSHAPE_H
#define QXRDROICENTEREDSHAPE_H

#include "qxrdroishape.h"

class QxrdROICenteredShape : public QxrdROIShape
{
  Q_OBJECT

public:
  QxrdROICenteredShape(int shapeType,
                       double halfWidth,
                       double halfHeight);

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

#endif // QXRDROICENTEREDSHAPE_H
