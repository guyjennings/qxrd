#ifndef QCEPROIRASTERDATA_H
#define QCEPROIRASTERDATA_H

#include "qceplib_global.h"
#include <qwt_raster_data.h>
#include "qceproi-ptr.h"

//TODO: eliminate
class QCEP_EXPORT QcepROIRasterData : public QwtRasterData
{
public:
  QcepROIRasterData(QcepROIWPtr roi);

public:
  double value(double x, double y) const;
  QwtInterval range();

private:
  QcepROIWPtr m_ROI;
};

#endif // QCEPROIRASTERDATA_H
