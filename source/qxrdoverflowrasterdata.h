#ifndef QXRDOVERFLOWRASTERDATA_H
#define QXRDOVERFLOWRASTERDATA_H

#include "qcepmacros.h"

#include <qwt_raster_data.h>
#include "qxrdimagedata-ptr.h"

class QxrdOverflowRasterData : public QwtRasterData
{
public:
  QxrdOverflowRasterData(QxrdDoubleImageDataPtr image, double level);

public:
  double value(double x, double y) const;

private:
  QxrdDoubleImageDataPtr m_Image;
  double                 m_OverflowLevel;
};

#endif // QXRDOVERFLOWRASTERDATA_H
