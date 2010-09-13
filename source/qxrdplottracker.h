/******************************************************************
*
*  $Id: qxrdplottracker.h,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#ifndef QXRDPLOTTRACKER_H
#define QXRDPLOTTRACKER_H

#include "qcepmacros.h"

#include "qxrdforwardtypes.h"
#include <qwt_plot_picker.h>

class QxrdPlotTracker : public QwtPlotPicker
{
  Q_OBJECT;

public:
  QxrdPlotTracker(QwtPlotCanvasPtr canvas, QxrdImagePlotPtr plot);

public:
  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

private:
  QxrdImagePlotPtr m_Plot;
  HEADER_IDENT("$Id: qxrdplottracker.h,v 1.2 2010/09/13 20:00:41 jennings Exp $");
};

#endif // QXRDPLOTTRACKER_H

/******************************************************************
*
*  $Log: qxrdplottracker.h,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:41  jennings
*  Moving files into source subdirectory
*
*  Revision 1.2.4.2  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.2.4.1  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

