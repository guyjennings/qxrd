#ifndef QXRDMASKRASTERDATA_H
#define QXRDMASKRASTERDATA_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include <qwt_raster_data.h>
#include "qcepmaskdata.h"
#include "qxrdimageplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdMaskRasterData : public QwtRasterData
{
public:
  QxrdMaskRasterData(QcepMaskDataWPtr                mask,
                     QxrdImagePlotWidgetSettingsWPtr set);

public:
  void setMask(QcepMaskDataWPtr mask);

  double value(double x, double y) const;

  int width() const;
  int height() const;

private:
  void setIntervals();

private:
  QcepMaskDataWPtr                m_MaskData;
  QxrdImagePlotWidgetSettingsWPtr m_Settings;
  int                             m_NRows;
  int                             m_NCols;
};

#endif
