#ifndef QXRDPLOTMEASURER_H
#define QXRDPLOTMEASURER_H

#include <qwt_plot_picker.h>
class QxrdImagePlot;

class QxrdPlotMeasurer : public QwtPlotPicker
{
public:
  QxrdPlotMeasurer(QwtPlotCanvas *canvas, QxrdImagePlot *plot);
private:
  QxrdImagePlot   *m_Plot;
};

#endif // QXRDPLOTMEASURER_H
