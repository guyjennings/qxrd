#ifndef QXRDRASTERDATA_H
#define QXRDRASTERDATA_H

#include "qcepmacros.h"

#include <qwt_raster_data.h>
#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"

class QxrdRasterData : public QwtRasterData
{
//  Q_OBJECT

public:
  QxrdRasterData(QxrdDoubleImageDataPtr img = QxrdDoubleImageDataPtr(),
                 int interp = 1,
                 QxrdMaskDataPtr msk = QxrdMaskDataPtr(),
                 QwtDoubleInterval range = QwtDoubleInterval(0,40000));

public:
  double value(double x, double y) const;
  QxrdRasterData* copy() const;
  QwtDoubleInterval range() const;
  void setDisplayedRange(double min, double max);

  double minValue();
  double maxValue();

  void setInterpolate(int interp);
  int interpolate();

  QwtDoubleInterval percentileRange(double lowpct, double highpct);

  int width() const;
  int height() const;

  const QxrdDoubleImageDataPtr data() const { return m_Data; }
  const QxrdMaskDataPtr mask() const { return m_Mask; }
  int interp() const          { return m_Interpolate; }

private:
  QxrdDoubleImageDataPtr m_Data;
  QxrdMaskDataPtr        m_Mask;
  int                    m_NRows;
  int                    m_NCols;
  QwtDoubleInterval      m_Range;
  int                    m_Interpolate;
};

#endif
