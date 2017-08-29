#ifndef QXRDMASKRASTERDATA_H
#define QXRDMASKRASTERDATA_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include <qwt_raster_data.h>
#include "qcepmaskdata.h"

class QXRD_EXPORT QxrdMaskRasterData : public QwtRasterData
{
public:
  QxrdMaskRasterData(QcepMaskDataPtr mask = QcepMaskDataPtr(), int interp = 0);

public:
  double value(double x, double y) const;
  QwtInterval range() const;

  short int *data() const;

  const QcepMaskDataPtr mask() const { return m_Mask; }
  int interp() const                 { return m_Interpolate; }

private:
  QcepMaskDataPtr  m_Mask;
  int              m_Interpolate;
};

#endif
