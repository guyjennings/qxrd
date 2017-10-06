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
                 QwtInterval range = QwtInterval(0,40000));

public:
  double value(double x, double y) const;
  QwtInterval range() const;
  void setDisplayedRange(double min, double max);

private:
  QxrdDoubleImageDataPtr m_Data;
  QxrdMaskDataPtr        m_Mask;
  int                    m_NRows;
  int                    m_NCols;
  QwtInterval            m_Range;
  int                    m_Interpolate;
};

#endif
