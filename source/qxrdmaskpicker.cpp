/******************************************************************
*
*  $Id: qxrdmaskpicker.cpp,v 1.3 2010/09/17 16:21:51 jennings Exp $
*
*******************************************************************/

#include "qxrdmaskpicker.h"
#include "qxrdimageplot.h"
#include "qxrdrasterdata.h"

QxrdMaskPicker::QxrdMaskPicker(QwtPlotCanvasPtr canvas, QxrdImagePlotPtr plot)
  : QxrdImagePlotMeasurer(canvas, plot),
    m_Plot(plot),
    SOURCE_IDENT("$Id: qxrdmaskpicker.cpp,v 1.3 2010/09/17 16:21:51 jennings Exp $")
{
  setTrackerMode(QwtPicker::AlwaysOn);
}

QxrdCircularMaskPicker::QxrdCircularMaskPicker(QwtPlotCanvasPtr canvas, QxrdImagePlotPtr plot)
  : QxrdMaskPicker(canvas, plot)
{
  qRegisterMetaType<QwtDoubleRect>("QwtDoubleRect");

  setSelectionFlags(QwtPicker::RectSelection | QwtPicker::CenterToRadius );
  setRubberBand(QwtPicker::EllipseRubberBand);
}

QxrdPolygonalMaskPicker::QxrdPolygonalMaskPicker(QwtPlotCanvasPtr canvas, QxrdImagePlotPtr plot)
  : QxrdMaskPicker(canvas, plot)
{
  qRegisterMetaType< QwtArray<QwtDoublePoint> >("QwtArray<QwtDoublePoint>");

  setSelectionFlags(QwtPicker::PolygonSelection);
  setRubberBand(QwtPicker::PolygonRubberBand);
}

/******************************************************************
*
*  $Log: qxrdmaskpicker.cpp,v $
*  Revision 1.3  2010/09/17 16:21:51  jennings
*  Rationalised the trackerText implementations
*
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:40  jennings
*  Moving files into source subdirectory
*
*  Revision 1.2.4.4  2010/05/02 16:56:50  jennings
*  Removed embedded C comments from commit log
*
*  Revision 1.2.4.3  2010/05/02 08:12:06  jennings
*  Replaced 'returnImageToPool' and 'replaceImageFromPool' by
*  equivalent smart pointer assignments
*
*  Revision 1.2.4.2  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.2.4.1  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.2  2009/08/02 21:14:16  jennings
*  Added masking dummy routines
*
*  Revision 1.1  2009/08/02 18:02:42  jennings
*  Added a number of masking operations to the UI - no actual implementation yet
*
*
*******************************************************************/
