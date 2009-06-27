/******************************************************************
*
*  $Id: qxrdplotzoomer.h,v 1.2 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#ifndef QXRDPLOTZOOMER_H
#define QXRDPLOTZOOMER_H

#include "qcepmacros.h"

#include <qwt_plot_zoomer.h>

class QxrdImagePlot;

class QxrdPlotZoomer : public QwtPlotZoomer
{
  Q_OBJECT;

public:
  QxrdPlotZoomer(QwtPlotCanvas *canvas, QxrdImagePlot *plot);

public:
  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

private:
  QxrdImagePlot   *m_Plot;
  HEADER_IDENT("$Id: qxrdplotzoomer.h,v 1.2 2009/06/27 22:50:32 jennings Exp $");
};

#endif // QXRDPLOTZOOMER_H

/******************************************************************
*
*  $Log: qxrdplotzoomer.h,v $
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

