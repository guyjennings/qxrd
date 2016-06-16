#ifndef QXRDROICENTEREDSHAPE_H
#define QXRDROICENTEREDSHAPE_H

#include "qxrdroishape.h"

class QxrdROICenteredShape : public QxrdROIShape
{
public:
  QxrdROICenteredShape(int shapeType,
                       double halfWidth,
                       double halfHeight);

  virtual QRectF boundingRect();

public:
  Q_PROPERTY(double halfWidth READ get_HalfWidth WRITE set_HalfWidth)
  QCEP_DOUBLE_PROPERTY(HalfWidth)

  Q_PROPERTY(double halfHeight READ get_HalfHeight WRITE set_HalfHeight)
  QCEP_DOUBLE_PROPERTY(HalfHeight)
};

#endif // QXRDROICENTEREDSHAPE_H
