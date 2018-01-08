#ifndef QXRDRASTERDATA_H
#define QXRDRASTERDATA_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include <qwt_raster_data.h>
#include "qcepimagedata.h"
#include "qcepmaskdata.h"
#include "qxrdimageplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdRasterData : public QwtRasterData
{
public:
  QxrdRasterData(QcepImageDataBaseWPtr           img,
                 QxrdImagePlotWidgetSettingsWPtr set);

public:
  double value(double x, double y) const;
  QwtInterval range() const;
  void setDisplayedRange(double min, double max);

  double minValue();
  double maxValue();

  void setImage(QcepImageDataBasePtr img);

  QwtInterval percentileRange(double lowpct, double highpct);

  int width() const;
  int height() const;

  QPointF optimizePeakPosition(QPointF pt) const;

private:
  void setIntervals();

private:
  QcepImageDataBaseWPtr           m_ImageData;
  QxrdImagePlotWidgetSettingsWPtr m_Settings;
  int                             m_NRows;
  int                             m_NCols;
  QwtInterval                     m_Range;
};

#endif
