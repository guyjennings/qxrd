#include "qxrdmaskpicker.h"
#include "qxrdimageplot.h"
#include "qxrdrasterdata.h"

QxrdMaskPicker::QxrdMaskPicker(QwtPlotCanvasPtr canvas, QxrdImagePlotPtr plot)
  : QxrdImagePlotMeasurer(canvas, plot),
    m_Plot(plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
}

QxrdCircularMaskPicker::QxrdCircularMaskPicker(QwtPlotCanvasPtr canvas, QxrdImagePlotPtr plot)
  : QxrdMaskPicker(canvas, plot)
{
  qRegisterMetaType<QwtDoubleRect>("QwtDoubleRect");

  setSelectionFlags(QwtPicker::RectSelection | QwtPicker::CenterToRadius );
  setRubberBand(QwtPicker::EllipseRubberBand);
}

QxrdPolygonalMaskPicker::QxrdPolygonalMaskPicker(QwtPlotCanvasPtr canvas, QxrdImagePlotPtr plot)
  : QxrdMaskPicker(canvas, plot)
{
  qRegisterMetaType< QwtArray<QwtDoublePoint> >("QwtArray<QwtDoublePoint>");

  setSelectionFlags(QwtPicker::PolygonSelection);
  setRubberBand(QwtPicker::PolygonRubberBand);
}
