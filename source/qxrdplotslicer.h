/******************************************************************
*
*  $Id: qxrdplotslicer.h,v 1.3 2010/09/17 16:21:51 jennings Exp $
*
*******************************************************************/

#ifndef QXRDPLOTSLICER_H
#define QXRDPLOTSLICER_H

#include "qcepmacros.h"

#include "qxrdforwardtypes.h"
#include "qxrdimageplotmeasurer.h"

class QxrdPlotSlicer : public QxrdImagePlotMeasurer
{
public:
  QxrdPlotSlicer(QwtPlotCanvasPtr canvas, QxrdImagePlotPtr plot);

//  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

private:
  QxrdImagePlotPtr m_Plot;
  HEADER_IDENT("$Id: qxrdplotslicer.h,v 1.3 2010/09/17 16:21:51 jennings Exp $");
};

#endif // QXRDPLOTSLICER_H

/******************************************************************
*
*  $Log: qxrdplotslicer.h,v $
*  Revision 1.3  2010/09/17 16:21:51  jennings
*  Rationalised the trackerText implementations
*
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:41  jennings
*  Moving files into source subdirectory
*
*  Revision 1.4.4.2  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.4.4.1  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.4  2009/08/09 15:40:32  jennings
*  Added measurer tool to all graphs
*
*  Revision 1.3  2009/08/02 18:03:34  jennings
*  Minor formatting change
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

