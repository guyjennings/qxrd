#include "qcepplotzoomer.h"

QcepPlotZoomer::QcepPlotZoomer(QWidget *canvas, QcepPlot *plot)
  : QwtPlotZoomer(canvas),
  m_Plot(plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setTrackerPen(QPen(Qt::green));
}

QwtText QcepPlotZoomer::trackerTextF(const QPointF &pos) const
{
  return (m_Plot ? m_Plot->trackerTextF(pos) : tr("%1, %2").arg(pos.x()).arg(pos.y()));
}

