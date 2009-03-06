#ifndef QXRDRASTERDATA_H
#define QXRDRASTERDATA_H

#include <qwt_raster_data.h>

class QxrdRasterData : public QwtRasterData
{
 public:
  QxrdRasterData(QVector<double> img, int offset, int nrows, int ncols);

  double value(double x, double y) const;
  QxrdRasterData* copy() const;
  QwtDoubleInterval range() const;

 private:
  QVector<double>    m_Data;
  int                m_Offset;
  int                m_NRows;
  int                m_NCols;
  QwtDoubleInterval  m_Range;
  int                m_Interpolate;
};

#endif
