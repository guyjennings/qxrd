#ifndef QXRDPLOTZOOMER_H
#define QXRDPLOTZOOMER_H

#include <qwt_plot_zoomer.h>

class QxrdPlot;
class QxrdImagePlot;

class QxrdPlotZoomer : public QwtPlotZoomer
{
  Q_OBJECT

public:
  QxrdPlotZoomer(QwtPlotCanvas *canvas, QxrdPlot *plot);

public:
  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

private:
  QxrdPlot *m_Plot;
};

class QxrdImagePlotZoomer : public QxrdPlotZoomer
{
  Q_OBJECT

public:
  QxrdImagePlotZoomer(QwtPlotCanvas *canvas, QxrdImagePlot *plot);

private:
  QxrdImagePlot *m_ImagePlot;
};

#endif // QXRDPLOTZOOMER_H
