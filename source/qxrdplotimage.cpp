#include "qxrdplotimage.h"
#include "qxrdimagedata.h"

QxrdPlotImage::QxrdPlotImage(const QString &title, QxrdDoubleImageDataPtr data, QxrdMaskDataPtr mask) :
    QwtPlotRasterItem(title),
    m_Data(data),
    m_Mask(mask)
{
  setAlpha(40);
}

QImage QxrdPlotImage::renderImage(const QwtScaleMap &/*xMap*/, const QwtScaleMap &/*yMap*/, const QRectF &/*area*/,
                                  const QSize &/*imageSize*/) const
{
  int width = (m_Data ? m_Data->get_Width() : 256);
  int height = (m_Data ?  m_Data->get_Height() : 256);

  QImage res(width, height, QImage::Format_ARGB32);

  res.fill(0x33445566);

  for (int i=0; i<height; i++) {
//    QRgb* aLine = (QRgb*) res.scanLine(i);
//
    for (int j=0; j<width; j++) {
      res.setPixel(j,i, qRgba(i,j,0,i));
    }
  }
  return res;
}
