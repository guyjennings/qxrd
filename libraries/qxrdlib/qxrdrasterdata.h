#ifndef QXRDRASTERDATA_H
#define QXRDRASTERDATA_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include <qwt_raster_data.h>
#include "qcepimagedata.h"
#include "qcepmaskdata.h"

class QXRD_EXPORT QxrdRasterData : public QwtRasterData
{
public:
  QxrdRasterData(QcepImageDataBaseWPtr img, int interp);

public:
  double value(double x, double y) const;
  QwtInterval range() const;
  void setDisplayedRange(double min, double max);

  double minValue();
  double maxValue();

  void setImage(QcepImageDataBasePtr img);

  void setInterpolate(int interp);
  int interpolate();

  QwtInterval percentileRange(double lowpct, double highpct);

  int width() const;
  int height() const;

  QPointF optimizePeakPosition(QPointF pt) const;

private:
  void setIntervals();

private:
  QcepImageDataBaseWPtr  m_ImageData;
  int                    m_NRows;
  int                    m_NCols;
  QwtInterval            m_Range;
  int                    m_Interpolate;
};

#endif
