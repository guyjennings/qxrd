/******************************************************************
*
*  $Id: qxrdplotzoomer.h,v 1.3 2009/08/12 19:44:59 jennings Exp $
*
*******************************************************************/

#ifndef QXRDPLOTZOOMER_H
#define QXRDPLOTZOOMER_H

#include "qcepmacros.h"

#include <qwt_plot_zoomer.h>

class QxrdPlot;

class QxrdPlotZoomer : public QwtPlotZoomer
{
  Q_OBJECT;

public:
  QxrdPlotZoomer(QwtPlotCanvas *canvas, QxrdPlot *plot);

public:
  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

private:
  QxrdPlot   *m_Plot;
  HEADER_IDENT("$Id: qxrdplotzoomer.h,v 1.3 2009/08/12 19:44:59 jennings Exp $");
};

#endif // QXRDPLOTZOOMER_H

/******************************************************************
*
*  $Log: qxrdplotzoomer.h,v $
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

