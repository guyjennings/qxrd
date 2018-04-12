#include "qceproirasterdata.h"

QcepROIRasterData::QcepROIRasterData(QcepROIWPtr roi)
  : QwtRasterData(),
    m_ROI(roi)
{
}

double QcepROIRasterData::value(double /*x*/, double /*y*/) const
{
  return 0;
}

QwtInterval QcepROIRasterData::range()
{
  return QwtInterval(0,1);
}
