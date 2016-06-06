#ifndef QXRDROIPOLYGON_H
#define QXRDROIPOLYGON_H

#include "qxrdroishape.h"

class QxrdROIPolygon : public QxrdROIShape
{
  Q_OBJECT

public:
  QxrdROIPolygon(QPolygonF poly);
  QxrdROIPolygon();

public:
  Q_PROPERTY(QPolygonF poly READ get_Poly WRITE set_Poly)
  QCEP_POLYGON_PROPERTY(Poly)
};

#endif // QXRDROIPOLYGON_H
