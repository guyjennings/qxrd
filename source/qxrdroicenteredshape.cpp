#include "qxrdroicenteredshape.h"

QxrdROICenteredShape::QxrdROICenteredShape(int shapeType, double halfWidth, double halfHeight)
  : QxrdROIShape(shapeType),
    m_HalfWidth(this, "halfWidth", halfWidth, "Centered ROI Half Width"),
    m_HalfHeight(this, "halfHeight", halfHeight, "Centered ROI Half Height")
{
}

QRectF QxrdROICenteredShape::boundingRect()
{
  double hw = get_HalfWidth();
  double hh = get_HalfHeight();

  QRectF res(0, 0, 2*hw, 2*hh);

  res.moveCenter(QPointF(0,0));

  return res;
}
