/******************************************************************
*
*  $Id: qxrdplotmeasurer.cpp,v 1.2 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#include "qxrdplotmeasurer.h"

QxrdPlotMeasurer::QxrdPlotMeasurer(QwtPlotCanvas *canvas, QxrdImagePlot *plot)
  : QwtPlotPicker(canvas),
    m_Plot(plot),
    SOURCE_IDENT("$Id: qxrdplotmeasurer.cpp,v 1.2 2009/06/27 22:50:32 jennings Exp $")
{
}

/******************************************************************
*
*  $Log: qxrdplotmeasurer.cpp,v $
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

