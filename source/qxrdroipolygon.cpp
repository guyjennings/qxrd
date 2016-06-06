#include "qxrdroipolygon.h"

QxrdROIPolygon::QxrdROIPolygon(QPolygonF poly)
  : QxrdROIShape(PolygonShape, poly.boundingRect()),
    m_Poly(this, "poly", poly, "ROI Polygon Shape")
{
}

QxrdROIPolygon::QxrdROIPolygon()
  : QxrdROIShape(PolygonShape, QRectF()),
    m_Poly(this, "poly", QPolygonF(), "ROI Polygon Shape")
{
}
