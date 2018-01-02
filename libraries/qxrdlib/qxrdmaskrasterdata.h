#ifndef QXRDMASKRASTERDATA_H
#define QXRDMASKRASTERDATA_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include <qwt_raster_data.h>
#include "qcepmaskdata.h"

class QXRD_EXPORT QxrdMaskRasterData : public QwtRasterData
{
public:
  QxrdMaskRasterData(QcepMaskDataPtr mask,
                     int interp);

public:
  double value(double x, double y) const;
  QwtInterval range() const;

  short int *data() const;

  int width() const;
  int height() const;

  const QcepMaskDataPtr mask() const { return m_Mask; }
  int interp() const                 { return m_Interpolate; }

private:
  QcepMaskDataPtr  m_Mask;
  int              m_NRows;
  int              m_NCols;
  int              m_Interpolate;
};

#endif
