#ifndef QXRDMASKRASTERDATA_H
#define QXRDMASKRASTERDATA_H

#include "qcepmacros.h"

#include <qwt_raster_data.h>
#include "qxrdmaskdata.h"

class QxrdMaskRasterData : public QwtRasterData
{
public:
  QxrdMaskRasterData(QxrdMaskDataPtr mask = QxrdMaskDataPtr(), int interp = 0);

public:
  double value(double x, double y) const;
  QwtDoubleInterval range() const;

  QxrdMaskRasterData* copy() const;

  short int *data() const;

  const QxrdMaskDataPtr mask() const { return m_Mask; }
  int interp() const                 { return m_Interpolate; }

private:
  QxrdMaskDataPtr  m_Mask;
  int              m_Interpolate;
};

#endif

class QxrdMaskRasterData;
