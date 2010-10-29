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
};

#endif // QXRDCENTERFINDERPLOT_H
