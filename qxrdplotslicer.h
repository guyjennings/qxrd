#ifndef QXRDPLOTSLICER_H
#define QXRDPLOTSLICER_H

#include <qwt_plot_picker.h>
class QxrdImagePlot;

class QxrdPlotSlicer : public QwtPlotPicker
{
public:
  QxrdPlotSlicer(QwtPlotCanvas *canvas, QxrdImagePlot *plot);
private:
  QxrdImagePlot   *m_Plot;
};

#endif // QXRDPLOTSLICER_H
