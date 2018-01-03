#ifndef QXRDMASKRASTERDATA_H
#define QXRDMASKRASTERDATA_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include <qwt_raster_data.h>
#include "qcepmaskdata.h"

class QXRD_EXPORT QxrdMaskRasterData : public QwtRasterData
{
public:
  QxrdMaskRasterData(QcepMaskDataWPtr mask);

public:
  void setMask(QcepMaskDataWPtr mask);

  double value(double x, double y) const;

  int width() const;
  int height() const;

private:
  void setIntervals();

private:
  QcepMaskDataWPtr  m_MaskData;
  int               m_NRows;
  int               m_NCols;
};

#endif
