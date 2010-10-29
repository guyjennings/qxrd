#include "qxrdplotzoomer.h"
#include "qxrdplot.h"
#include "qxrdrasterdata.h"
#include "qxrdimageplot.h"
#include "qxrdcenterfinder.h"

QxrdPlotZoomer::QxrdPlotZoomer(QwtPlotCanvasPtr canvas, QxrdPlotPtr plot)
  : QwtPlotZoomer(canvas),
    m_Plot(plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setTrackerPen(QPen(Qt::green));
}

QwtText QxrdPlotZoomer::trackerText(const QwtDoublePoint &pos) const
{
  return (m_Plot ? m_Plot->trackerText(pos) : tr("%1, %2").arg(pos.x()).arg(pos.y()));
}

QxrdImagePlotZoomer::QxrdImagePlotZoomer(QwtPlotCanvasPtr canvas, QxrdImagePlotPtr plot)
  : QxrdPlotZoomer(canvas, plot),
    m_ImagePlot(plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setTrackerPen(QPen(Qt::green));
}
