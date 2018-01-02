#ifndef QXRDOVERFLOWRASTERDATA_H
#define QXRDOVERFLOWRASTERDATA_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include <qwt_raster_data.h>
#include "qcepimagedata.h"

class QXRD_EXPORT QxrdOverflowRasterData : public QwtRasterData
{
public:
  QxrdOverflowRasterData(QcepImageDataBasePtr img,
                         int interp,
                         double level);

public:
  double value(double x, double y) const;
  QwtInterval range() const;

//  short int *data() const;

  int width() const;
  int height() const;

//  const QcepImageDataBasePtr data() const { return m_Data; }
  int interp() const          { return m_Interpolate; }

private:
  QcepImageDataBasePtr   m_Data;
  int                    m_NRows;
  int                    m_NCols;
  double                 m_OverflowLevel;
  int                    m_Interpolate;
};

#endif // QXRDOVERFLOWRASTERDATA_H
