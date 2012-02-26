#ifndef QXRDPLOTIMAGE_H
#define QXRDPLOTIMAGE_H

#include <qwt_plot_rasteritem.h>
#include "qxrdimagedata-ptr.h"
#include "qxrdmaskdata-ptr.h"

class QxrdPlotImage : public QwtPlotRasterItem
{
public:
  QxrdPlotImage(const QString &title = "", QxrdDoubleImageDataPtr data = QxrdDoubleImageDataPtr(NULL), QxrdMaskDataPtr mask = QxrdMaskDataPtr(NULL));

  QImage renderImage (const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QwtDoubleRect &area) const;

private:
  QxrdDoubleImageDataPtr m_Data;
  QxrdMaskDataPtr        m_Mask;
};

#endif // QXRDPLOTIMAGE_H
