#ifndef QXRDOVERFLOWRASTERDATA_H
#define QXRDOVERFLOWRASTERDATA_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include <qwt_raster_data.h>
#include "qcepimagedata.h"

class QXRD_EXPORT QxrdOverflowRasterData : public QwtRasterData
{
public:
  QxrdOverflowRasterData(QcepImageDataBaseWPtr img,
                         double level);

public:
  double value(double x, double y) const;

  void setImage(QcepImageDataBaseWPtr img);

  int width() const;
  int height() const;

private:
  void setIntervals();

private:
  QcepImageDataBaseWPtr  m_ImageData;
  int                    m_NRows;
  int                    m_NCols;
  double                 m_OverflowLevel;
};

#endif // QXRDOVERFLOWRASTERDATA_H
