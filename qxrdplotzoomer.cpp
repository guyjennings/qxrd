/******************************************************************
*
*  $Id: qxrdplotzoomer.cpp,v 1.3 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#include "qxrdplotzoomer.h"
#include "qxrdimageplot.h"

QxrdPlotZoomer::QxrdPlotZoomer(QwtPlotCanvas *canvas, QxrdImagePlot *plot)
  : QwtPlotZoomer(canvas),
    m_Plot(plot),
    SOURCE_IDENT("$Id: qxrdplotzoomer.cpp,v 1.3 2009/06/27 22:50:32 jennings Exp $")
{
  setTrackerMode(QwtPicker::ActiveOnly);
  setTrackerPen(QPen(Qt::green));
}

QwtText QxrdPlotZoomer::trackerText(const QwtDoublePoint &pos) const
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
*  $Log: qxrdplotzoomer.cpp,v $
*  Revision 1.3  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

