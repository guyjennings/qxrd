#ifndef QXRDMASKRASTERDATA_H
#define QXRDMASKRASTERDATA_H

#include "qcepmacros.h"

#include <qwt_raster_data.h>
#include "qxrdmaskdata.h"

class QxrdMaskRasterData : public QwtRasterData
{
public:
  QxrdMaskRasterData(QxrdMaskDataPtr mask = QxrdMaskDataPtr(), int interp = 0);

//private:
//  QxrdMaskRasterData();
//  QxrdMaskRasterData(const QxrdMaskRasterData &cpy);

public:
  double value(double x, double y) const;
  QwtDoubleInterval range() const;

  QxrdMaskRasterData* copy() const;

  short int *data() const;

  void mask_combine(QxrdMaskRasterData *a);

  const QxrdMaskDataPtr mask() const { return m_Mask; }
  int interp() const                 { return m_Interpolate; }

  /*  private: */
  /*   QxrdMaskRasterData(const QxrdMaskRasterData&); */

private:
  QxrdMaskDataPtr  m_Mask;
  int              m_Interpolate;
};

#endif
