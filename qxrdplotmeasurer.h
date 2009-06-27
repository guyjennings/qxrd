/******************************************************************
*
*  $Id: qxrdplotmeasurer.h,v 1.2 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#ifndef QXRDPLOTMEASURER_H
#define QXRDPLOTMEASURER_H

#include "qcepmacros.h"

#include <qwt_plot_picker.h>
class QxrdImagePlot;

class QxrdPlotMeasurer : public QwtPlotPicker
{
public:
  QxrdPlotMeasurer(QwtPlotCanvas *canvas, QxrdImagePlot *plot);
private:
  QxrdImagePlot   *m_Plot;
  HEADER_IDENT("$Id: qxrdplotmeasurer.h,v 1.2 2009/06/27 22:50:32 jennings Exp $");
};

#endif // QXRDPLOTMEASURER_H

/******************************************************************
*
*  $Log: qxrdplotmeasurer.h,v $
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

