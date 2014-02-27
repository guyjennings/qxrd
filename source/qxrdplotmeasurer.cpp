#include "qxrdplotmeasurer.h"
#include "qxrdrasterdata.h"
#include "qxrdimageplot.h"
#include "qxrdplot.h"
#include "qwt_picker_machine.h"

QxrdPlotMeasurer::QxrdPlotMeasurer(QWidget *canvas, QxrdPlotPtr plot)
  : QwtPlotPicker(canvas),
    m_Plot(plot)
{
  qRegisterMetaType< QVector<QPointF> >("QVector<QPointF>");

  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerPolygonMachine());
  setRubberBand(QwtPicker::PolygonRubberBand);
}

QwtText QxrdPlotMeasurer::trackerText(const QPointF &pos) const
{
  return (m_Plot ? m_Plot->trackerText(pos) : tr("%1, %2").arg(pos.x()).arg(pos.y()));
}
