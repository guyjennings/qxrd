#include "qxrdmaskpicker.h"
#include "qxrdimageplot.h"
#include "qxrdrasterdata.h"

QxrdMaskPicker::QxrdMaskPicker(QwtPlotCanvas *canvas, QxrdImagePlot *plot)
  : QxrdImagePlotMeasurer(canvas, plot),
    m_Plot(plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
}

QxrdCircularMaskPicker::QxrdCircularMaskPicker(QwtPlotCanvas *canvas, QxrdImagePlot *plot)
  : QxrdMaskPicker(canvas, plot)
{
  qRegisterMetaType<QwtDoubleRect>("QwtDoubleRect");

  setSelectionFlags(QwtPicker::RectSelection | QwtPicker::CenterToRadius );
  setRubberBand(QwtPicker::EllipseRubberBand);
}

QxrdPolygonalMaskPicker::QxrdPolygonalMaskPicker(QwtPlotCanvas *canvas, QxrdImagePlot *plot)
  : QxrdMaskPicker(canvas, plot)
{
  qRegisterMetaType< QwtArray<QwtDoublePoint> >("QwtArray<QwtDoublePoint>");

  setSelectionFlags(QwtPicker::PolygonSelection);
  setRubberBand(QwtPicker::PolygonRubberBand);
}
