#include "qxrdroirasterdata.h"

QxrdROIRasterData::QxrdROIRasterData(QxrdROIWPtr roi)
  : QwtRasterData(),
    m_ROI(roi)
{
}

double QxrdROIRasterData::value(double /*x*/, double /*y*/) const
{
  return 0;
}

QwtInterval QxrdROIRasterData::range()
{
  return QwtInterval(0,1);
}
