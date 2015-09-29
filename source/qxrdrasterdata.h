#ifndef QXRDRASTERDATA_H
#define QXRDRASTERDATA_H

#include "qcepmacros.h"

#include <qwt_raster_data.h>
#include "qcepimagedata.h"
#include "qcepmaskdata.h"

class QxrdRasterData : public QwtRasterData
{
//  Q_OBJECT

public:
  QxrdRasterData(QcepImageDataBasePtr img = QcepImageDataBasePtr(),
                 int interp = 1,
                 QcepMaskDataPtr msk = QcepMaskDataPtr(),
                 QwtInterval range = QwtInterval(0,40000));

public:
  double value(double x, double y) const;
  QxrdRasterData* copy() const;
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
  const QcepMaskDataPtr mask() const { return m_Mask; }
  int interp() const          { return m_Interpolate; }

  QPointF optimizePeakPosition(QPointF pt) const;

private:
  QcepImageDataBasePtr   m_Data;
  QcepMaskDataPtr        m_Mask;
  int                    m_NRows;
  int                    m_NCols;
  QwtInterval            m_Range;
  int                    m_Interpolate;
};

#endif
