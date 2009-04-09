#ifndef QXRDCENTERFINDERPLOT_H
#define QXRDCENTERFINDERPLOT_H

#include <qwt_plot.h>

class QxrdImageData;
class QwtPlotPicker;
class QwtPlotZoomer;
class QwtPlotPanner;
class QwtPlotMagnifier;
class QwtLegend;

class QxrdCenterFinderPlot : public QwtPlot
{
  Q_OBJECT;

public:
  QxrdCenterFinderPlot(QWidget *parent=0);

  void onCenterChanged(QxrdImageData *img, double cx, double cy);

public slots:
  void doZoomIn();
  void doZoomOut();
  void doZoomAll();
  void autoScale();

private:
  QwtPlotPicker       *m_Tracker;
  QwtPlotPanner       *m_Panner;
  QwtPlotZoomer       *m_Zoomer;
  QwtPlotMagnifier    *m_Magnifier;
  QwtLegend           *m_Legend;
  QVector<double>      m_XData, m_YData;
};

#endif // QXRDCENTERFINDERPLOT_H
