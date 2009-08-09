/******************************************************************
*
*  $Id: qxrdplotmeasurer.cpp,v 1.3 2009/08/09 15:39:10 jennings Exp $
*
*******************************************************************/

#include "qxrdplotmeasurer.h"
#include "qxrdrasterdata.h"
#include "qxrdimageplot.h"

QxrdPlotMeasurer::QxrdPlotMeasurer(QwtPlotCanvas *canvas, QxrdPlot *plot)
  : QwtPlotPicker(canvas),
    m_Plot(plot),
    SOURCE_IDENT("$Id: qxrdplotmeasurer.cpp,v 1.3 2009/08/09 15:39:10 jennings Exp $")
{
  qRegisterMetaType< QwtArray<QwtDoublePoint> >("QwtArray<QwtDoublePoint>");

  setTrackerMode(QwtPicker::AlwaysOn);
  setSelectionFlags(QwtPicker::PolygonSelection);
  setRubberBand(QwtPicker::PolygonRubberBand);
}

QwtText QxrdPlotMeasurer::trackerText(const QwtDoublePoint &pos) const
{
  return tr("%1, %2").arg(pos.x()).arg(pos.y());
}

/******************************************************************
*
*  $Log: qxrdplotmeasurer.cpp,v $
*  Revision 1.3  2009/08/09 15:39:10  jennings
*  *** empty log message ***
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

