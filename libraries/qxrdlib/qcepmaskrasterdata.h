#ifndef QCEPMASKRASTERDATA_H
#define QCEPMASKRASTERDATA_H

#include "qceplib_global.h"
#include "qcepmacros.h"

#include <qwt_raster_data.h>
#include "qcepmaskdata.h"
#include "qcepimageplotwidgetsettings-ptr.h"

class QCEP_EXPORT QcepMaskRasterData : public QwtRasterData
{
public:
  QcepMaskRasterData(QcepMaskDataWPtr                mask,
                     QcepImagePlotWidgetSettingsWPtr set);

public:
  void setMask(QcepMaskDataWPtr mask);

  double value(double x, double y) const;

  int width() const;
  int height() const;

private:
  void setIntervals();

private:
  QcepMaskDataWPtr                m_MaskData;
  QcepImagePlotWidgetSettingsWPtr m_Settings;
  int                             m_NRows;
  int                             m_NCols;
};

#endif // QCEPMASKRASTERDATA_H
