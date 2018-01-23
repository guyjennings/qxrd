#ifndef QXRDROIRASTERDATA_H
#define QXRDROIRASTERDATA_H

#include "qxrdlib_global.h"
#include <qwt_raster_data.h>
#include "qxrdroi-ptr.h"

//TODO: eliminate
class QXRD_EXPORT QxrdROIRasterData : public QwtRasterData
{
public:
  QxrdROIRasterData(QxrdROIWPtr roi);

public:
  double value(double x, double y) const;
  QwtInterval range();

private:
  QxrdROIWPtr m_ROI;
};

#endif // QXRDROIRASTERDATA_H
