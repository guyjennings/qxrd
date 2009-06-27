/******************************************************************
*
*  $Id: qxrdplottracker.h,v 1.2 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#ifndef QXRDPLOTTRACKER_H
#define QXRDPLOTTRACKER_H

#include "qcepmacros.h"

#include <qwt_plot_picker.h>

class QxrdImagePlot;

class QxrdPlotTracker : public QwtPlotPicker
{
  Q_OBJECT;

public:
  QxrdPlotTracker(QwtPlotCanvas *canvas, QxrdImagePlot *plot);

public:
  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

private:
  QxrdImagePlot   *m_Plot;
  HEADER_IDENT("$Id: qxrdplottracker.h,v 1.2 2009/06/27 22:50:32 jennings Exp $");
};

#endif // QXRDPLOTTRACKER_H

/******************************************************************
*
*  $Log: qxrdplottracker.h,v $
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

