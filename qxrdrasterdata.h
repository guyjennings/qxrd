#ifndef QXRDRASTERDATA_H
#define QXRDRASTERDATA_H

#include <qwt_raster_data.h>
#include "qxrdimagedata.h"

class QxrdRasterData : public QwtRasterData
{
 public:
  QxrdRasterData(QxrdImageData *img);
  QxrdRasterData();

  double value(double x, double y) const;
  QxrdRasterData* copy() const;
  QwtDoubleInterval range() const;
  void setDisplayedRange(double min, double max);

  double minValue();
  double maxValue();

 private:
  QxrdImageData     *m_Data;
//  int                m_Offset;
  int                m_NRows;
  int                m_NCols;
  QwtDoubleInterval  m_Range;
  int                m_Interpolate;
};

#endif
