#include "qxrdplottracker.h"
#include "qxrdimageplot.h"

QxrdPlotTracker::QxrdPlotTracker(QwtPlotCanvas *canvas, QxrdImagePlot *plot)
  : QwtPlotPicker(canvas),
    m_Plot(plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setTrackerPen(QPen(Qt::green));
  setRubberBand(QwtPicker::CrossRubberBand);
}

QwtText QxrdPlotTracker::trackerText(const QwtDoublePoint &pos) const
{
  QxrdRasterData *raster = m_Plot->raster();

  if (raster) {
    return tr("%1, %2, %3").arg(pos.x()).arg(pos.y()).arg(raster->value(pos.x(),pos.y()));
  } else {
    return tr("%1, %2").arg(pos.x()).arg(pos.y());
  }
}

