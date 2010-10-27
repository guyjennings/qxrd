/******************************************************************
*
*  $Id: qxrdcenterfinderpicker.cpp,v 1.3 2010/09/17 16:21:51 jennings Exp $
*
*******************************************************************/

#include "qxrdcenterfinderpicker.h"
#include "qxrdcenterfinderdialog.h"
#include "qxrdimageplot.h"
#include "qxrdrasterdata.h"

QxrdCenterFinderPicker::QxrdCenterFinderPicker(QxrdImagePlotPtr plot)
  : QxrdImagePlotMeasurer(plot -> canvas(), plot),
    m_Plot(plot),
    SOURCE_IDENT("$Id: qxrdcenterfinderpicker.cpp,v 1.3 2010/09/17 16:21:51 jennings Exp $")
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setSelectionFlags(QwtPicker::PointSelection);
  setRubberBand(QwtPicker::CrossRubberBand);
}

/******************************************************************
*
*  $Log: qxrdcenterfinderpicker.cpp,v $
*  Revision 1.3  2010/09/17 16:21:51  jennings
*  Rationalised the trackerText implementations
*
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:38  jennings
*  Moving files into source subdirectory
*
*  Revision 1.4.4.1  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.4  2009/07/22 11:55:34  jennings
*  Center finder modifications
*
*  Revision 1.3  2009/07/21 22:55:48  jennings
*  Rearranged center finder and integrator code so that the center finder and integrator objects go into the data processor thread, and the GUI stuff goes in the GUI thread
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

