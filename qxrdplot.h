/******************************************************************
*
*  $Id: qxrdplot.h,v 1.5 2009/08/11 20:53:42 jennings Exp $
*
*******************************************************************/

#ifndef QXRDPLOT_H
#define QXRDPLOT_H

#include "qcepmacros.h"

#include <qwt_plot.h>

class QwtPlotCurve;
class QwtPlotPicker;
class QwtPlotZoomer;
class QwtPlotPanner;
class QwtPlotMagnifier;
class QxrdPlotMeasurer;

class QxrdPlot : public QwtPlot
{
  Q_OBJECT;
public:
  QxrdPlot(bool customTracker, bool customZoomer, QWidget *parent = 0);

public:
  void setPlotCurveStyle(int index, QwtPlotCurve *curve);

public slots:
  void autoScale();
  void zoomIn();
  void zoomOut();

  void enableZooming();
  void enableMeasuring();

protected:
  void setCustomTracker(QwtPlotPicker *tracker);
  void setCustomZoomer(QwtPlotZoomer *zoomer);

protected:
  QwtPlotPicker       *m_Tracker;
  QwtPlotZoomer       *m_Zoomer;
  QwtPlotPanner       *m_Panner;
  QwtPlotMagnifier    *m_Magnifier;
  QxrdPlotMeasurer    *m_Measurer;

private:
  HEADER_IDENT("$Id: qxrdplot.h,v 1.5 2009/08/11 20:53:42 jennings Exp $");
};

#endif // QXRDPLOT_H

/******************************************************************
*
*  $Log: qxrdplot.h,v $
*  Revision 1.5  2009/08/11 20:53:42  jennings
*  Added automatic plot style options to plot curves
*
*  Revision 1.4  2009/08/09 15:40:32  jennings
*  Added measurer tool to all graphs
*
*  Revision 1.3  2009/07/25 15:18:19  jennings
*  Moved graph zooming code into QxrdPlot - a common base class
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

