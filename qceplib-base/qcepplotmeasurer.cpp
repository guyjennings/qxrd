#include "qcepplotmeasurer.h"
#include "qcepplot.h"
#include "qwt_picker_machine.h"

QcepPlotMeasurer::QcepPlotMeasurer(QWidget *canvas, QcepPlotPtr plot)
  : QwtPlotPicker(canvas),
    m_Plot(plot)
{
  qRegisterMetaType< QVector<QPointF> >("QVector<QPointF>");

  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerPolygonMachine());
  setRubberBand(QwtPicker::PolygonRubberBand);
}

QwtText QcepPlotMeasurer::trackerTextF(const QPointF &pos) const
{
  return (m_Plot ? m_Plot->trackerTextF(pos) : tr("%1, %2").arg(pos.x()).arg(pos.y()));
}
