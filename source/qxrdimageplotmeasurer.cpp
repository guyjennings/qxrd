#include "qxrdimageplotmeasurer.h"

#include "qcepplotmeasurer.h"
#include "qxrdrasterdata.h"
#include "qxrdimageplot.h"
#include "qxrdcenterfinder.h"
#include "qwt_picker_machine.h"

QxrdImagePlotMeasurer::QxrdImagePlotMeasurer(QWidget *canvas, QxrdImagePlot *plot)
  : QcepPlotMeasurer(canvas, plot),
    m_Plot(plot)
{
  qRegisterMetaType< QVector<QPointF> >("QVector<QPointF>");

  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerPolygonMachine());
  setRubberBand(QwtPicker::PolygonRubberBand);
}

QxrdImagePlot* QxrdImagePlotMeasurer::imagePlot() const
{
  return m_Plot;
}
