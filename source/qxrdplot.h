/******************************************************************
*
*  $Id: qxrdplot.h,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#ifndef QXRDPLOT_H
#define QXRDPLOT_H

#include "qcepmacros.h"

#include "qxrdforwardtypes.h"
#include <qwt_plot.h>

class QxrdPlot : public QwtPlot
{
  Q_OBJECT;
public:
  QxrdPlot(QWidget *parent = 0);
  typedef QwtPlot inherited;
  virtual ~QxrdPlot();

public:
  void setPlotCurveStyle(int index, QwtPlotCurvePtr curve);
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
  QwtLegendPtr        m_Legend;
  QwtPlotZoomerPtr    m_Zoomer;
  QwtPlotPannerPtr    m_Panner;
  QwtPlotMagnifierPtr m_Magnifier;
  QxrdPlotMeasurerPtr m_Measurer;

private:
  HEADER_IDENT("$Id: qxrdplot.h,v 1.2 2010/09/13 20:00:40 jennings Exp $");
};

#endif // QXRDPLOT_H

/******************************************************************
*
*  $Log: qxrdplot.h,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:41  jennings
*  Moving files into source subdirectory
*
*  Revision 1.9.2.3  2010/06/18 16:29:13  jennings
*  Added an (empty) destructor to QxrdPlot - wanted to check that plots were being deleted properly
*
*  Revision 1.9.2.2  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.9.2.1  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.9  2009/11/12 20:17:43  jennings
*  Version 0.3.5, fix problems with first-time zooming of graphs
*
*  Revision 1.8  2009/11/02 20:19:27  jennings
*  Changes to make it work with VC compiler
*
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

