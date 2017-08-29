#ifndef QXRDROIRECTANGLE_H
#define QXRDROIRECTANGLE_H

#include "qxrdroicenteredshape.h"

class QxrdROIRectangle : public QxrdROICenteredShape
{
  Q_OBJECT

public:
  QxrdROIRectangle(double halfWidth,
                   double halfHeight);
  QxrdROIRectangle(double scale);

public:
  virtual int markerCount();
  virtual QPointF markerPoint(int n);
  virtual bool pointInShape(QPointF pt);
  virtual QPolygonF dragOutline();
};

#endif // QXRDROIRECTANGLE_H
