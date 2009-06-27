/******************************************************************
*
*  $Id: qxrdplotslicer.cpp,v 1.2 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#include "qxrdplotslicer.h"

QxrdPlotSlicer::QxrdPlotSlicer(QwtPlotCanvas *canvas, QxrdImagePlot *plot)
  : QwtPlotPicker(canvas),
    m_Plot(plot),
    SOURCE_IDENT("$Id: qxrdplotslicer.cpp,v 1.2 2009/06/27 22:50:32 jennings Exp $")
{
}

/******************************************************************
*
*  $Log: qxrdplotslicer.cpp,v $
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

