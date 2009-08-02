/******************************************************************
*
*  $Id: qxrdmaskpicker.cpp,v 1.1 2009/08/02 18:02:42 jennings Exp $
*
*******************************************************************/

#include "qxrdmaskpicker.h"
#include "qxrdimageplot.h"
#include "qxrdrasterdata.h"

QxrdMaskPicker::QxrdMaskPicker(QwtPlotCanvas *canvas, QxrdImagePlot *plot)
  : QwtPlotPicker(canvas),
    m_Plot(plot),
    SOURCE_IDENT("$Id: qxrdmaskpicker.cpp,v 1.1 2009/08/02 18:02:42 jennings Exp $")
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setRubberBand(QwtPicker::CrossRubberBand);
}

QwtText QxrdMaskPicker::trackerText(const QwtDoublePoint &pos) const
{
  QxrdRasterData *raster = m_Plot->raster();

  if (raster) {
    return tr("%1, %2, %3").arg(pos.x()).arg(pos.y()).arg(raster->value(pos.x(),pos.y()));
  } else {
    return tr("%1, %2").arg(pos.x()).arg(pos.y());
  }
}

QxrdCircularMaskPicker::QxrdCircularMaskPicker(QwtPlotCanvas *canvas, QxrdImagePlot *plot)
  : QxrdMaskPicker(canvas, plot)
{
  setSelectionFlags(QwtPicker::PointSelection);
}

QxrdPolygonalMaskPicker::QxrdPolygonalMaskPicker(QwtPlotCanvas *canvas, QxrdImagePlot *plot)
  : QxrdMaskPicker(canvas, plot)
{
  setSelectionFlags(QwtPicker::PolygonSelection);
}

/******************************************************************
*
*  $Log: qxrdmaskpicker.cpp,v $
*  Revision 1.1  2009/08/02 18:02:42  jennings
*  Added a number of masking operations to the UI - no actual implementation yet
*
*
*******************************************************************/
