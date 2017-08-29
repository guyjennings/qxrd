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
  QxrdRasterData(QcepImageDataBasePtr img = QcepImageDataBasePtr(),
                 int interp = 1,
                 QwtInterval range = QwtInterval(0,40000));

public:
  double value(double x, double y) const;
  QwtInterval range() const;
  void setDisplayedRange(double min, double max);

  double minValue();
  double maxValue();

  void setInterpolate(int interp);
  int interpolate();

  QwtInterval percentileRange(double lowpct, double highpct);

  int width() const;
  int height() const;

  const QcepImageDataBasePtr data() const { return m_Data; }
  int interp() const          { return m_Interpolate; }

  QPointF optimizePeakPosition(QPointF pt) const;

private:
  QcepImageDataBasePtr   m_Data;
  int                    m_NRows;
  int                    m_NCols;
  QwtInterval            m_Range;
  int                    m_Interpolate;
};

#endif
