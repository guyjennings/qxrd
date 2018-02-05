#ifndef QXRDPLOTVECTORFIELDCURVE_H
#define QXRDPLOTVECTORFIELDCURVE_H

#include "qxrdlib_global.h"
#include "qwt_plot_seriesitem.h"
#include "qxrdplotvectorfielddata.h"

class QXRD_EXPORT QxrdPlotVectorFieldCurve :
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
  //TODO: change to QObjectWPtr...
  void initialize();
};

#endif // QXRDPLOTVECTORFIELDCURVE_H
