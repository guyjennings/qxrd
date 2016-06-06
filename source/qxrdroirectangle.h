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
  QxrdROIRectangle();

public:
};

#endif // QXRDROIRECTANGLE_H
