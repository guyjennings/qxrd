/******************************************************************
*
*  $Id: qxrdplotzoomer.h,v 1.3 2010/09/17 16:21:51 jennings Exp $
*
*******************************************************************/

#ifndef QXRDPLOTZOOMER_H
#define QXRDPLOTZOOMER_H

#include "qcepmacros.h"

#include "qxrdforwardtypes.h"
#include <qwt_plot_zoomer.h>

class QxrdPlotZoomer : public QwtPlotZoomer
{
  Q_OBJECT;

public:
  QxrdPlotZoomer(QwtPlotCanvasPtr canvas, QxrdPlotPtr plot);

public:
  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

private:
  QxrdPlotPtr m_Plot;
  HEADER_IDENT("$Id: qxrdplotzoomer.h,v 1.3 2010/09/17 16:21:51 jennings Exp $");
};

class QxrdImagePlotZoomer : public QxrdPlotZoomer
{
  Q_OBJECT;

public:
  QxrdImagePlotZoomer(QwtPlotCanvasPtr canvas, QxrdImagePlotPtr plot);

public:
  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

private:
  QxrdImagePlotPtr m_ImagePlot;
};

#endif // QXRDPLOTZOOMER_H

/******************************************************************
*
*  $Log: qxrdplotzoomer.h,v $
*  Revision 1.3  2010/09/17 16:21:51  jennings
*  Rationalised the trackerText implementations
*
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:41  jennings
*  Moving files into source subdirectory
*
*  Revision 1.3.4.2  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.3.4.1  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.3  2009/08/12 19:44:59  jennings
*  Reorganized plot zoomers into a single class, initialized in QxrdPlot, which
*  takes its tracker text from a QxrdPlot virtual member function
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

