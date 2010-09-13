/******************************************************************
*
*  $Id: qxrdplotzoomer.cpp,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#include "qxrdplotzoomer.h"
#include "qxrdplot.h"

QxrdPlotZoomer::QxrdPlotZoomer(QwtPlotCanvasPtr canvas, QxrdPlotPtr plot)
  : QwtPlotZoomer(canvas),
    m_Plot(plot),
    SOURCE_IDENT("$Id: qxrdplotzoomer.cpp,v 1.2 2010/09/13 20:00:41 jennings Exp $")
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
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:41  jennings
*  Moving files into source subdirectory
*
*  Revision 1.4.4.4  2010/05/02 16:56:50  jennings
*  Removed embedded C comments from commit log
*
*  Revision 1.4.4.3  2010/05/02 08:12:07  jennings
*  Replaced 'returnImageToPool' and 'replaceImageFromPool' by
*  equivalent smart pointer assignments
*
*  Revision 1.4.4.2  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.4.4.1  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.4  2009/08/12 19:44:59  jennings
*  Reorganized plot zoomers into a single class, initialized in QxrdPlot, which
*  takes its tracker text from a QxrdPlot virtual member function
*
*  Revision 1.3  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

