/******************************************************************
*
*  $Id: qxrdplotzoomer.cpp,v 1.4 2009/08/12 19:44:59 jennings Exp $
*
*******************************************************************/

#include "qxrdplotzoomer.h"
#include "qxrdplot.h"

QxrdPlotZoomer::QxrdPlotZoomer(QwtPlotCanvas *canvas, QxrdPlot *plot)
  : QwtPlotZoomer(canvas),
    m_Plot(plot),
    SOURCE_IDENT("$Id: qxrdplotzoomer.cpp,v 1.4 2009/08/12 19:44:59 jennings Exp $")
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setTrackerPen(QPen(Qt::green));
}

QwtText QxrdPlotZoomer::trackerText(const QwtDoublePoint &pos) const
{
  if (m_Plot) {
    return m_Plot -> trackerText(pos);
  } else {
    return tr("(NULL)");
  }
}

/******************************************************************
*
*  $Log: qxrdplotzoomer.cpp,v $
*  Revision 1.4  2009/08/12 19:44:59  jennings
*  *** empty log message ***
*
*  Revision 1.3  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

