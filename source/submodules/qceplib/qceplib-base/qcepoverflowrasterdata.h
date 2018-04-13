#ifndef QCEPOVERFLOWRASTERDATA_H
#define QCEPOVERFLOWRASTERDATA_H

#include "qceplib_global.h"
#include "qcepmacros.h"

#include <qwt_raster_data.h>
#include "qcepimagedata.h"
#include "qcepimageplotwidgetsettings-ptr.h"

class QCEP_EXPORT QcepOverflowRasterData : public QwtRasterData
{
public:
  QcepOverflowRasterData(QcepImageDataBaseWPtr           img,
                         QcepImagePlotWidgetSettingsWPtr set);

public:
  double value(double x, double y) const;

  void setImage(QcepImageDataBaseWPtr img);

  int width() const;
  int height() const;

private:
  void setIntervals();

private:
  QcepImageDataBaseWPtr           m_ImageData;
  QcepImagePlotWidgetSettingsWPtr m_Settings;
  int                             m_NRows;
  int                             m_NCols;
};

#endif // QCEPOVERFLOWRASTERDATA_H
