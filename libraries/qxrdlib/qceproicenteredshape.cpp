#include "qceproicenteredshape.h"

QcepROICenteredShape::QcepROICenteredShape(QString      name,
                                           ROIShapeType shapeType,
                                           double       halfWidth,
                                           double       halfHeight)
  : inherited(name, shapeType),
    m_HalfWidth(this, "halfWidth", halfWidth, "Centered ROI Half Width"),
    m_HalfHeight(this, "halfHeight", halfHeight, "Centered ROI Half Height")
{
}

QRectF QcepROICenteredShape::boundingRect()
{
  double hw = get_HalfWidth();
  double hh = get_HalfHeight();

  QRectF res(0, 0, 2*hw, 2*hh);

  res.moveCenter(QPointF(0,0));

  return res;
}

void QcepROICenteredShape::scale(double kx, double ky)
{
  set_HalfWidth(get_HalfWidth()*kx);
  set_HalfHeight(get_HalfHeight()*ky);
}

void QcepROICenteredShape::deleteROIPoint(int /*n*/)
{
}

void QcepROICenteredShape::changeROIPoint(int /*n*/, QPointF /*pt*/)
{
}

void QcepROICenteredShape::insertROIPoint(int /*n*/, QPointF /*pt*/)
{
}
