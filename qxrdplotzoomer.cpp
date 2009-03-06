#include "qxrdplotzoomer.h"
#include "qxrdimageplot.h"

QxrdPlotZoomer::QxrdPlotZoomer(QwtPlotCanvas *canvas, QxrdImagePlot *plot)
  : QwtPlotZoomer(canvas),
    m_Plot(plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setTrackerPen(QPen(Qt::green));
}

QwtText QxrdPlotZoomer::trackerText(const QwtDoublePoint &pos) const
{
  QxrdRasterData *raster = m_Plot->raster();

  if (raster) {
    return tr("%1, %2, %3").arg(pos.x()).arg(pos.y()).arg(raster->value(pos.x(),pos.y()));
  } else {
    return tr("%1, %2").arg(pos.x()).arg(pos.y());
  }
}
