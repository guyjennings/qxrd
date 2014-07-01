#ifndef QXRDPLOTVECTORFIELDCURVE_H
#define QXRDPLOTVECTORFIELDCURVE_H

#include "qwt_plot_seriesitem.h"
#include "qxrdplotvectorfielddata.h"

class QxrdPlotVectorFieldCurve :
    public QwtPlotSeriesItem, QwtSeriesStore<QxrdPoint4D>
{
public:
  QxrdPlotVectorFieldCurve();

  void setSamples(QxrdPlotVectorFieldData *data);

  void drawSeries(QPainter *painter,
                  const QwtScaleMap &xMap,
                  const QwtScaleMap &yMap,
                  const QRectF &canvasRect,
                  int from, int to) const;

  QRectF boundingRect() const;

  QRectF dataRect() const;

private:
  void init();
};

#endif // QXRDPLOTVECTORFIELDCURVE_H
