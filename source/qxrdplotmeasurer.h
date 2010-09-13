/******************************************************************
*
*  $Id: qxrdplotmeasurer.h,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#ifndef QXRDPLOTMEASURER_H
#define QXRDPLOTMEASURER_H

#include "qcepmacros.h"

#include "qxrdforwardtypes.h"
#include <qwt_plot_picker.h>

class QxrdPlotMeasurer : public QwtPlotPicker
{
public:
  QxrdPlotMeasurer(QwtPlotCanvasPtr canvas, QxrdPlotPtr plot);

  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

private:
  QxrdPlotPtr m_Plot;
  HEADER_IDENT("$Id: qxrdplotmeasurer.h,v 1.2 2010/09/13 20:00:40 jennings Exp $");
};

#endif // QXRDPLOTMEASURER_H

/******************************************************************
*
*  $Log: qxrdplotmeasurer.h,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
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
*  Revision 1.3  2009/08/09 15:39:10  jennings
*  Added a separate QxrdImagePlotMeasurer class
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

