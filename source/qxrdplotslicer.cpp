#include "qxrdplotslicer.h"
#include "qxrdrasterdata.h"
#include "qxrdimageplot.h"

QxrdPlotSlicer::QxrdPlotSlicer(QwtPlotCanvasPtr canvas, QxrdImagePlotPtr plot)
  : QxrdImagePlotMeasurer(canvas, plot),
    m_Plot(plot)
{
  qRegisterMetaType< QwtArray<QwtDoublePoint> >("QwtArray<QwtDoublePoint>");

  setTrackerMode(QwtPicker::AlwaysOn);
  setSelectionFlags(QwtPicker::PolygonSelection);
  setRubberBand(QwtPicker::PolygonRubberBand);
}
