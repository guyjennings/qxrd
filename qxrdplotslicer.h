/******************************************************************
*
*  $Id: qxrdplotslicer.h,v 1.4 2009/08/09 15:40:32 jennings Exp $
*
*******************************************************************/

#ifndef QXRDPLOTSLICER_H
#define QXRDPLOTSLICER_H

#include "qcepmacros.h"

#include <qwt_plot_picker.h>

class QxrdImagePlot;

class QxrdPlotSlicer : public QwtPlotPicker
{
public:
  QxrdPlotSlicer(QwtPlotCanvas *canvas, QxrdImagePlot *plot);

  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

private:
  QxrdImagePlot   *m_Plot;
  HEADER_IDENT("$Id: qxrdplotslicer.h,v 1.4 2009/08/09 15:40:32 jennings Exp $");
};

#endif // QXRDPLOTSLICER_H

/******************************************************************
*
*  $Log: qxrdplotslicer.h,v $
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

