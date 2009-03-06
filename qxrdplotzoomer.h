#ifndef QXRDPLOTZOOMER_H
#define QXRDPLOTZOOMER_H

#include <qwt_plot_zoomer.h>

class QxrdImagePlot;

class QxrdPlotZoomer : public QwtPlotZoomer
{
  Q_OBJECT;

public:
  QxrdPlotZoomer(QwtPlotCanvas *canvas, QxrdImagePlot *plot);

public:
  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

private:
  QxrdImagePlot   *m_Plot;
};

#endif // QXRDPLOTZOOMER_H
