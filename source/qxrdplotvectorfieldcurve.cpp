#include "qxrdplotvectorfieldcurve.h"
#include "qwt_painter.h"
#include"qwt_scale_map.h"

QxrdPlotVectorFieldCurve::QxrdPlotVectorFieldCurve()
{
}

void QxrdPlotVectorFieldCurve::setSamples(QxrdPlotVectorFieldData *data)
{
  setData(data);
}

void QxrdPlotVectorFieldCurve::drawSeries
  (QPainter *painter,
   const QwtScaleMap &xMap,
   const QwtScaleMap &yMap,
   const QRectF &canvasRect,
   int from, int to) const
{
  if (to < 0) {
    to = dataSize() - 1;
  }

  if (from < 0) {
    from = 0;
  }

  if (from > to) {
    return;
  }

  painter->save();

  const QwtSeriesData<QxrdPoint4D> *series = data();

  for (int i=from; i<=to; i++) {
    const QxrdPoint4D sample = series->sample(i);

    double x0 = xMap.transform(sample.x());
    double y0 = yMap.transform(sample.y());
    double x1 = xMap.transform(sample.x()+sample.z()*50.0);
    double y1 = yMap.transform(sample.y()+sample.t()*50.0);

    QwtPainter::drawLine(painter, x0, y0, x1, y1);
  }

  painter->restore();
}
