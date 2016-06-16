#ifndef QXRDROIELLIPSE_H
#define QXRDROIELLIPSE_H

#include "qxrdroicenteredshape.h"

class QxrdROIEllipse : public QxrdROICenteredShape
{
  Q_OBJECT

public:
  QxrdROIEllipse(double halfWidth,
                 double halfHeight);
  QxrdROIEllipse(double scale);

  virtual int markerCount();
  virtual QPointF markerPoint(int n);
  virtual bool pointInShape(QPointF pt);
  virtual QPolygonF dragOutline();
};

#endif // QXRDROIELLIPSE_H
