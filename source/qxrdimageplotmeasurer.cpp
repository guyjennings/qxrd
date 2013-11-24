#include "qxrdimageplotmeasurer.h"

#include "qxrdplotmeasurer.h"
#include "qxrdrasterdata.h"
#include "qxrdimageplot.h"
#include "qxrdcenterfinder.h"

QxrdImagePlotMeasurer::QxrdImagePlotMeasurer(QwtPlotCanvas *canvas, QxrdImagePlot *plot)
  : QxrdPlotMeasurer(canvas, plot),
    m_Plot(plot)
{
  qRegisterMetaType< QVector<QPointF> >("QVector<QPointF>");

  setTrackerMode(QwtPicker::AlwaysOn);
  setSelectionFlags(QwtPicker::PolygonSelection);
  setRubberBand(QwtPicker::PolygonRubberBand);
}
