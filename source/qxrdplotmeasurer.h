#ifndef QXRDPLOTMEASURER_H
#define QXRDPLOTMEASURER_H

#include <qwt_plot_picker.h>

class QxrdPlot;

class QxrdPlotMeasurer : public QwtPlotPicker
{
  Q_OBJECT;

public:
  QxrdPlotMeasurer(QwtPlotCanvas *canvas, QxrdPlot *plot);

  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

private:
  QxrdPlot *m_Plot;
};

#endif // QXRDPLOTMEASURER_H
