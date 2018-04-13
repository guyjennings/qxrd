#include "qcepimageplotmeasurer.h"

#include "qcepplotmeasurer.h"
#include "qceprasterdata.h"
#include "qcepimageplot.h"
#include "qcepcenterfinder.h"
#include "qwt_picker_machine.h"

QcepImagePlotMeasurer::QcepImagePlotMeasurer(QcepImagePlot *plot)
  : QcepPlotMeasurer(plot),
    m_Plot(plot)
{
  qRegisterMetaType< QVector<QPointF> >("QVector<QPointF>");

  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerPolygonMachine());
  setRubberBand(QwtPicker::PolygonRubberBand);
}

QcepImagePlot* QcepImagePlotMeasurer::imagePlot() const
{
  return m_Plot;
}
