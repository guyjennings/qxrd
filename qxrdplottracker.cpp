/******************************************************************
*
*  $Id: qxrdplottracker.cpp,v 1.2 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#include "qxrdplottracker.h"
#include "qxrdimageplot.h"

QxrdPlotTracker::QxrdPlotTracker(QwtPlotCanvas *canvas, QxrdImagePlot *plot)
  : QwtPlotPicker(canvas),
    m_Plot(plot),
    SOURCE_IDENT("$Id: qxrdplottracker.cpp,v 1.2 2009/06/27 22:50:32 jennings Exp $")
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setTrackerPen(QPen(Qt::green));
  setRubberBand(QwtPicker::CrossRubberBand);
}

QwtText QxrdPlotTracker::trackerText(const QwtDoublePoint &pos) const
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
*  $Log: qxrdplottracker.cpp,v $
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  *** empty log message ***
*
*
*******************************************************************/

