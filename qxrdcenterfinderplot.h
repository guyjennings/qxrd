/******************************************************************
*
*  $Id: qxrdcenterfinderplot.h,v 1.6 2009/07/22 11:55:34 jennings Exp $
*
*******************************************************************/

#ifndef QXRDCENTERFINDERPLOT_H
#define QXRDCENTERFINDERPLOT_H

#include "qcepmacros.h"

#include "qxrdplot.h"

class QxrdImageData;
class QwtPlotPicker;
class QwtPlotZoomer;
class QwtPlotPanner;
class QwtPlotMagnifier;
class QwtLegend;
class QxrdDataProcessor;
class QxrdCenterFinder;

class QxrdCenterFinderPlot : public QxrdPlot
{
  Q_OBJECT;

public:
  QxrdCenterFinderPlot(QWidget *parent=0);

  void setDataProcessor(QxrdDataProcessor *proc);

public slots:
  void doZoomIn();
  void doZoomOut();
  void doZoomAll();
  void autoScale();
  void onCenterXChanged(double cx);
  void onCenterYChanged(double cy);
  void onCenterChanged(double cx, double cy);
  void onProcessedImageAvailable(QxrdImageData *image);

private:
  QwtPlotPicker       *m_Tracker;
  QwtPlotPanner       *m_Panner;
  QwtPlotZoomer       *m_Zoomer;
  QwtPlotMagnifier    *m_Magnifier;
  QwtLegend           *m_Legend;
  QVector<double>      m_XData, m_YData;
  QxrdDataProcessor   *m_DataProcessor;
  QxrdCenterFinder    *m_CenterFinder;

  HEADER_IDENT("$Id: qxrdcenterfinderplot.h,v 1.6 2009/07/22 11:55:34 jennings Exp $");
};

#endif // QXRDCENTERFINDERPLOT_H

/******************************************************************
*
*  $Log: qxrdcenterfinderplot.h,v $
*  Revision 1.6  2009/07/22 11:55:34  jennings
*  Center finder modifications
*
*  Revision 1.5  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

