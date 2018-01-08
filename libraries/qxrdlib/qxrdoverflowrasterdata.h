#ifndef QXRDOVERFLOWRASTERDATA_H
#define QXRDOVERFLOWRASTERDATA_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include <qwt_raster_data.h>
#include "qcepimagedata.h"
#include "qxrdimageplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdOverflowRasterData : public QwtRasterData
{
public:
  QxrdOverflowRasterData(QcepImageDataBaseWPtr           img,
                         QxrdImagePlotWidgetSettingsWPtr set);

public:
  double value(double x, double y) const;

  void setImage(QcepImageDataBaseWPtr img);

  int width() const;
  int height() const;

private:
  void setIntervals();

private:
  QcepImageDataBaseWPtr           m_ImageData;
  QxrdImagePlotWidgetSettingsWPtr m_Settings;
  int                             m_NRows;
  int                             m_NCols;
};

#endif // QXRDOVERFLOWRASTERDATA_H
