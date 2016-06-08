#ifndef QXRDROIRECTANGLE_H
#define QXRDROIRECTANGLE_H

#include "qxrdroishape.h"

class QxrdROIRectangle : public QxrdROIShape
{
  Q_OBJECT

public:
  QxrdROIRectangle(double left,
                   double top,
                   double right,
                   double bottom);
  QxrdROIRectangle(double scale);

public:
  virtual int markerCount();
  virtual QPointF markerPoint(int n);
  virtual QRectF boundingRect();
  virtual bool pointInShape(QPointF pt);
};

#endif // QXRDROIRECTANGLE_H
