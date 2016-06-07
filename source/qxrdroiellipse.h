#ifndef QXRDROIELLIPSE_H
#define QXRDROIELLIPSE_H

#include "qxrdroishape.h"

class QxrdROIEllipse : public QxrdROIShape
{
  Q_OBJECT

public:
  QxrdROIEllipse(double left,
                 double top,
                 double right,
                 double bottom);
  QxrdROIEllipse();

  virtual int markerCount();
  virtual QPointF markerPoint(int n);
};

#endif // QXRDROIELLIPSE_H
