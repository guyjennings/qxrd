/******************************************************************
*
*  $Id: qxrdcenterfinderpicker.cpp,v 1.4 2009/07/22 11:55:34 jennings Exp $
*
*******************************************************************/

#include "qxrdcenterfinderpicker.h"
#include "qxrdcenterfinderdialog.h"
#include "qxrdimageplot.h"
#include "qxrdrasterdata.h"

QxrdCenterFinderPicker::QxrdCenterFinderPicker(QxrdImagePlot *plot)
  : QwtPlotPicker(plot -> canvas()),
    m_Plot(plot),
    SOURCE_IDENT("$Id: qxrdcenterfinderpicker.cpp,v 1.4 2009/07/22 11:55:34 jennings Exp $")
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setSelectionFlags(QwtPicker::PointSelection);
  setRubberBand(QwtPicker::CrossRubberBand);
}

QwtText QxrdCenterFinderPicker::trackerText(const QwtDoublePoint &pos) const
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
*  $Log: qxrdcenterfinderpicker.cpp,v $
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

