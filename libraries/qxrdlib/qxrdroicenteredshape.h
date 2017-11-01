#ifndef QXRDROICENTEREDSHAPE_H
#define QXRDROICENTEREDSHAPE_H

#include "qxrdlib_global.h"
#include "qxrdroishape.h"

class QXRD_EXPORT QxrdROICenteredShape : public QxrdROIShape
{
  Q_OBJECT

public:
  QxrdROICenteredShape(ROIShapeType shapeType,
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