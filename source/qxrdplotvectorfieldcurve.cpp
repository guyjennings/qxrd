#include "qxrdplotvectorfieldcurve.h"

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

}
