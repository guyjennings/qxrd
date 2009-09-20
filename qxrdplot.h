/******************************************************************
*
*  $Id: qxrdplot.h,v 1.7 2009/09/20 21:18:53 jennings Exp $
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
  QxrdPlot(QWidget *parent = 0);

public:
  void setPlotCurveStyle(int index, QwtPlotCurve *curve);
  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

public slots:
  void autoScale();
  void zoomIn();
  void zoomOut();

  void enableZooming();
  void enableMeasuring();

  void onLegendClicked(QwtPlotItem *item);
  void onLegendChecked(QwtPlotItem *item, bool checked);

//  void setCustomTracker(QwtPlotPicker *tracker);
//  void setCustomZoomer(QwtPlotZoomer *zoomer);

protected:
  QwtLegend           *m_Legend;
//  QwtPlotPicker       *m_Tracker;
  QwtPlotZoomer       *m_Zoomer;
  QwtPlotPanner       *m_Panner;
  QwtPlotMagnifier    *m_Magnifier;
  QxrdPlotMeasurer    *m_Measurer;

private:
  HEADER_IDENT("$Id: qxrdplot.h,v 1.7 2009/09/20 21:18:53 jennings Exp $");
};

#endif // QXRDPLOT_H

/******************************************************************
*
*  $Log: qxrdplot.h,v $
*  Revision 1.7  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
*
*  Revision 1.6  2009/08/12 19:44:59  jennings
*  Reorganized plot zoomers into a single class, initialized in QxrdPlot, which
*  takes its tracker text from a QxrdPlot virtual member function
*
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

