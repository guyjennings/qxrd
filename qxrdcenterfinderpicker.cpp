/******************************************************************
*
*  $Id: qxrdcenterfinderpicker.cpp,v 1.2 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#include "qxrdcenterfinderpicker.h"
#include "qxrdcenterfinderdialog.h"

QxrdCenterFinderPicker::QxrdCenterFinderPicker(
    QwtPlotCanvas *canvas, QxrdImagePlot *plot, QxrdCenterFinderDialog *dlog)
  : QwtPlotPicker(canvas),
    m_Plot(plot),
    m_Dialog(dlog),
    SOURCE_IDENT("$Id: qxrdcenterfinderpicker.cpp,v 1.2 2009/06/27 22:50:32 jennings Exp $")
{
  setSelectionFlags(QwtPicker::PointSelection);
  setRubberBand(QwtPicker::CrossRubberBand);
}

/******************************************************************
*
*  $Log: qxrdcenterfinderpicker.cpp,v $
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

