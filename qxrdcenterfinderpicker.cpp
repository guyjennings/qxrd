/******************************************************************
*
*  $Id: qxrdcenterfinderpicker.cpp,v 1.3 2009/07/21 22:55:48 jennings Exp $
*
*******************************************************************/

#include "qxrdcenterfinderpicker.h"
#include "qxrdcenterfinderdialog.h"

QxrdCenterFinderPicker::QxrdCenterFinderPicker(QwtPlotCanvas *canvas)
  : QwtPlotPicker(canvas),
    SOURCE_IDENT("$Id: qxrdcenterfinderpicker.cpp,v 1.3 2009/07/21 22:55:48 jennings Exp $")
{
  setSelectionFlags(QwtPicker::PointSelection);
  setRubberBand(QwtPicker::CrossRubberBand);
}

/******************************************************************
*
*  $Log: qxrdcenterfinderpicker.cpp,v $
*  Revision 1.3  2009/07/21 22:55:48  jennings
*  Rearranged center finder and integrator code so that the center finder and integrator objects go into the data processor thread, and the GUI stuff goes in the GUI thread
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

