#ifndef QXRDMASKRASTERDATA_H
#define QXRDMASKRASTERDATA_H

#include "qcepmacros.h"

#include <qwt_raster_data.h>
#include "qxrdmaskdata.h"

class QxrdMaskRasterData : public QwtRasterData
{
public:
  QxrdMaskRasterData(QxrdMaskDataPtr mask);

public:
  double value(double x, double y) const;

private:
  QxrdMaskDataPtr  m_Mask;
};

#endif
