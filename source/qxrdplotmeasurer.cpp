#include "qxrdplotmeasurer.h"
#include "qxrdrasterdata.h"
#include "qxrdimageplot.h"

QxrdPlotMeasurer::QxrdPlotMeasurer(QwtPlotCanvas *canvas, QxrdPlot *plot)
  : QwtPlotPicker(canvas),
    m_Plot(plot)
{
  qRegisterMetaType< QwtArray<QwtDoublePoint> >("QwtArray<QwtDoublePoint>");

  setTrackerMode(QwtPicker::AlwaysOn);
  setSelectionFlags(QwtPicker::PolygonSelection);
  setRubberBand(QwtPicker::PolygonRubberBand);
}

QwtText QxrdPlotMeasurer::trackerText(const QwtDoublePoint &pos) const
{
  return (m_Plot ? m_Plot->trackerText(pos) : tr("%1, %2").arg(pos.x()).arg(pos.y()));
}
