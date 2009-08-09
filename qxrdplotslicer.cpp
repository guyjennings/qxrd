/******************************************************************
*
*  $Id: qxrdplotslicer.cpp,v 1.3 2009/08/09 15:40:32 jennings Exp $
*
*******************************************************************/

#include "qxrdplotslicer.h"
#include "qxrdrasterdata.h"
#include "qxrdimageplot.h"

QxrdPlotSlicer::QxrdPlotSlicer(QwtPlotCanvas *canvas, QxrdImagePlot *plot)
  : QwtPlotPicker(canvas),
    m_Plot(plot),
    SOURCE_IDENT("$Id: qxrdplotslicer.cpp,v 1.3 2009/08/09 15:40:32 jennings Exp $")
{
  qRegisterMetaType< QwtArray<QwtDoublePoint> >("QwtArray<QwtDoublePoint>");

  setTrackerMode(QwtPicker::AlwaysOn);
  setSelectionFlags(QwtPicker::PolygonSelection);
  setRubberBand(QwtPicker::PolygonRubberBand);
}

QwtText QxrdPlotSlicer::trackerText(const QwtDoublePoint &pos) const
{
  QxrdRasterData *raster = m_Plot->raster();

  if (raster) {
    return tr("%1, %2, %3").arg(pos.x()).arg(pos.y()).arg(raster->value(pos.x(),pos.y()));
  } else {
    return tr("%1, %2").arg(pos.x()).arg(pos.y());
  }
}

/******************************************************************
*
*  $Log: qxrdplotslicer.cpp,v $
*  Revision 1.3  2009/08/09 15:40:32  jennings
*  *** empty log message ***
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

