#ifndef QXRDRASTERDATA_H
#define QXRDRASTERDATA_H

#include <qwt_raster_data.h>

class QxrdImageData;

class QxrdRasterData : public QwtRasterData
{
 public:
  QxrdRasterData(QxrdImageData *img);

  double value(double x, double y) const;
  

 private:
  QxrdImageData     *m_Image;
  QwtDoubleInterval  m_Range;
  int                m_Interpolate;
};

#endif
