#include "qxrdimageplotmeasurer.h"

#include "qcepplotmeasurer.h"
#include "qceprasterdata.h"
#include "qxrdimageplot.h"
#include "qxrdcenterfinder.h"
#include "qwt_picker_machine.h"

QxrdImagePlotMeasurer::QxrdImagePlotMeasurer(QxrdImagePlot *plot)
  : QcepPlotMeasurer(plot),
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
