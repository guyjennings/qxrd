#ifndef QXRDINTEGRATORPLOT_H
#define QXRDINTEGRATORPLOT_H

#include <qwt_plot.h>

class QwtPlotZoomer;

class QxrdIntegratorPlot : public QwtPlot
{
  Q_OBJECT;

public:
  QxrdIntegratorPlot(QWidget *parent=0);

public slots:
  void doZoomIn();
  void doZoomOut();
  void doZoomAll();

private:
  QwtPlotZoomer   *m_Zoomer;
};

#endif // QXRDINTEGRATORPLOT_H
