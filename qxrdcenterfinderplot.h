/******************************************************************
*
*  $Id: qxrdcenterfinderplot.h,v 1.8 2009/08/04 16:45:20 jennings Exp $
*
*******************************************************************/

#ifndef QXRDCENTERFINDERPLOT_H
#define QXRDCENTERFINDERPLOT_H

#include "qcepmacros.h"

#include "qxrdplot.h"

class QxrdImageData;
class QxrdMaskData;
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
  void onCenterXChanged(double cx);
  void onCenterYChanged(double cy);
  void onCenterChanged(double cx, double cy);
  void onProcessedImageAvailable(QxrdImageData *image);
  void onMaskedImageAvailable(QxrdImageData *image, QxrdMaskData *mask);

private:
  QwtLegend           *m_Legend;
  QVector<double>      m_XData, m_YData;
  QxrdDataProcessor   *m_DataProcessor;
  QxrdCenterFinder    *m_CenterFinder;
  bool                 m_FirstTime;

  HEADER_IDENT("$Id: qxrdcenterfinderplot.h,v 1.8 2009/08/04 16:45:20 jennings Exp $");
};

#endif // QXRDCENTERFINDERPLOT_H

/******************************************************************
*
*  $Log: qxrdcenterfinderplot.h,v $
*  Revision 1.8  2009/08/04 16:45:20  jennings
*  Moved mask data into separate class
*
*  Revision 1.7  2009/07/25 15:18:19  jennings
*  Moved graph zooming code into QxrdPlot - a common base class
*
*  Revision 1.6  2009/07/22 11:55:34  jennings
*  Center finder modifications
*
*  Revision 1.5  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

