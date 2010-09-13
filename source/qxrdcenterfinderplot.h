/******************************************************************
*
*  $Id: qxrdcenterfinderplot.h,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#ifndef QXRDCENTERFINDERPLOT_H
#define QXRDCENTERFINDERPLOT_H

#include "qcepmacros.h"

#include "qxrdplot.h"

#include "qxrdimagedata.h"
#include "qxrddataprocessor.h"

class QxrdCenterFinderPlot : public QxrdPlot
{
  Q_OBJECT;

public:
  QxrdCenterFinderPlot(QWidget *parent=0);

  void setWindow(QxrdWindowPtr win);

public slots:
  void onParameterChanged();
  void onCenterXChanged(double cx);
  void onCenterYChanged(double cy);
  void onCenterChanged(double cx, double cy);
  void onCenterChanged(QwtDoublePoint c);
  void onProcessedImageAvailable(QxrdDoubleImageDataPtr image);
  void onMaskedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr mask);

private:
  QVector<double>      m_XData, m_YData;
  QxrdWindowPtr        m_Window;
  QxrdDataProcessorPtr m_DataProcessor;
  QxrdCenterFinderPtr  m_CenterFinder;
  bool                 m_FirstTime;

  HEADER_IDENT("$Id: qxrdcenterfinderplot.h,v 1.2 2010/09/13 20:00:39 jennings Exp $");
};

#endif // QXRDCENTERFINDERPLOT_H

/******************************************************************
*
*  $Log: qxrdcenterfinderplot.h,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.3  2010/09/10 18:54:10  jennings
*  Partial implement tilt in center finder dialog
*
*  Revision 1.1.2.2  2010/08/11 15:58:27  jennings
*  Added changedValue(QwtDoublePoint) slots
*
*  Revision 1.1.2.1  2010/07/22 18:39:38  jennings
*  Moving files into source subdirectory
*
*  Revision 1.13.2.3  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.13.2.2  2010/04/26 20:53:25  jennings
*  More attempts to get QSharedPointers to work...
*
*  Revision 1.13.2.1  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.13  2009/11/02 20:16:22  jennings
*  Changes to make it work with VC compiler
*
*  Revision 1.12  2009/09/25 14:22:16  jennings
*  Simplified double-buffering for plotted data - there is now a separate copy of data and mask
*  in QxrdWindow
*
*  Revision 1.11  2009/08/25 18:43:03  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.10  2009/08/12 19:44:58  jennings
*  Reorganized plot zoomers into a single class, initialized in QxrdPlot, which
*  takes its tracker text from a QxrdPlot virtual member function
*
*  Revision 1.9  2009/08/09 15:39:10  jennings
*  Added a separate QxrdImagePlotMeasurer class
*
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

