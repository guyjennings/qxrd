#ifndef QXRDROIRASTERDATA_H
#define QXRDROIRASTERDATA_H

#include "qxrdlib_global.h"
#include <qwt_raster_data.h>
#include "qxrdroicoordinates-ptr.h"

class QXRD_EXPORT QxrdROIRasterData : public QwtRasterData
{
public:
  QxrdROIRasterData(QxrdROICoordinatesWPtr roi);

public:
  double value(double x, double y) const;
  QwtInterval range();

private:
  QxrdROICoordinatesWPtr m_ROI;
};

#endif // QXRDROIRASTERDATA_H
