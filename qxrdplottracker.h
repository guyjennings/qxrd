#ifndef QXRDPLOTTRACKER_H
#define QXRDPLOTTRACKER_H

#include <qwt_plot_picker.h>

class QxrdImagePlot;

class QxrdPlotTracker : public QwtPlotPicker
{
  Q_OBJECT;

public:
  QxrdPlotTracker(QwtPlotCanvas *canvas, QxrdImagePlot *plot);

public:
  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

private:
  QxrdImagePlot   *m_Plot;
};

#endif // QXRDPLOTTRACKER_H
