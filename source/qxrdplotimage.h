#ifndef QXRDPLOTIMAGE_H
#define QXRDPLOTIMAGE_H

#include <qwt_plot_rasteritem.h>
#include "qcepimagedata.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata.h"
#include "qcepmaskdata-ptr.h"

class QxrdPlotImage : public QwtPlotRasterItem
{
public:
  QxrdPlotImage(const QString &title = "", QcepDoubleImageDataPtr data = QcepDoubleImageDataPtr(NULL), QcepMaskDataPtr mask = QcepMaskDataPtr(NULL));

  QImage renderImage (const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &area,
                      const QSize &imageSize) const;

private:
  QcepDoubleImageDataPtr m_Data;
  QcepMaskDataPtr        m_Mask;
};

#endif // QXRDPLOTIMAGE_H
