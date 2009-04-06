#ifndef QXRDMASKRASTERDATA_H
#define QXRDMASKRASTERDATA_H

#include <qwt_raster_data.h>

class QxrdImageData;

class QxrdMaskRasterData : public QwtRasterData
{
 public:
  QxrdMaskRasterData(QxrdImageData *img, int interp);
  QxrdMaskRasterData();

  double value(double x, double y) const;
  QwtDoubleInterval range() const;

  QxrdMaskRasterData *copy() const;

/*  private: */
/*   QxrdMaskRasterData(const QxrdMaskRasterData&); */

 private:
  QxrdImageData *m_Image;
  int            m_Interpolate;
};

#endif
