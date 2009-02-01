#ifndef QXRDIMAGEPLOT_H
#define QXRDIMAGEPLOT_H

#include <qwt_plot.h>

class QwtPlotZoomer;
class QwtLegend;

class QxrdImagePlot : public QwtPlot
{
  Q_OBJECT;

 public:
  QxrdImagePlot(QWidget *parent);

 public slots:
  void autoScale();

 private:
  QwtPlotZoomer   *m_Zoomer;
  QwtLegend       *m_Legend;
};

#endif
