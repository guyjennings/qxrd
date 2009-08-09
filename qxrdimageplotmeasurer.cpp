/******************************************************************
*
*  $Id: qxrdimageplotmeasurer.cpp,v 1.1 2009/08/09 15:38:29 jennings Exp $
*
*******************************************************************/

#include "qxrdimageplotmeasurer.h"

#include "qxrdplotmeasurer.h"
#include "qxrdrasterdata.h"
#include "qxrdimageplot.h"

QxrdImagePlotMeasurer::QxrdImagePlotMeasurer(QwtPlotCanvas *canvas, QxrdImagePlot *plot)
  : QxrdPlotMeasurer(canvas, plot),
    m_Plot(plot),
    SOURCE_IDENT("$Id: qxrdimageplotmeasurer.cpp,v 1.1 2009/08/09 15:38:29 jennings Exp $")
{
  qRegisterMetaType< QwtArray<QwtDoublePoint> >("QwtArray<QwtDoublePoint>");

  setTrackerMode(QwtPicker::AlwaysOn);
  setSelectionFlags(QwtPicker::PolygonSelection);
  setRubberBand(QwtPicker::PolygonRubberBand);
}


QwtText QxrdImagePlotMeasurer::trackerText(const QwtDoublePoint &pos) const
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
*  $Log: qxrdimageplotmeasurer.cpp,v $
*  Revision 1.1  2009/08/09 15:38:29  jennings
*  Added a separate QxrdImagePlotMeasurer class
*
*
*******************************************************************/
