#include "qxrdroicenteredshape.h"

QxrdROICenteredShape::QxrdROICenteredShape(ROIShapeType shapeType, double halfWidth, double halfHeight)
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

void QxrdROICenteredShape::scale(double kx, double ky)
{
  set_HalfWidth(get_HalfWidth()*kx);
  set_HalfHeight(get_HalfHeight()*ky);
}

void QxrdROICenteredShape::deleteROIPoint(int /*n*/)
{
}

void QxrdROICenteredShape::changeROIPoint(int /*n*/, QPointF /*pt*/)
{
}

void QxrdROICenteredShape::insertROIPoint(int /*n*/, QPointF /*pt*/)
{
}
