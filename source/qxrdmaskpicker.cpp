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

void QxrdPolygonalMaskPicker::append(const QPoint &pt)
{
  if (m_Plot) {
    m_Plot->disableContextMenu();
  }

  QxrdMaskPicker::append(pt);
}

bool QxrdPolygonalMaskPicker::end(bool ok)
{
  if (m_Plot) {
    m_Plot->enableContextMenu();
  }

  return QxrdMaskPicker::end(ok);
}
