#ifndef QXRDCENTERFINDERPICKER_H
#define QXRDCENTERFINDERPICKER_H

#include <qwt_plot_picker.h>

class QxrdImagePlot;
class QxrdCenterFinderDialog;

class QxrdCenterFinderPicker : public QwtPlotPicker
{
public:
  QxrdCenterFinderPicker
      (QwtPlotCanvas *canvas, QxrdImagePlot *plot, QxrdCenterFinderDialog *dlog);
private:
  QxrdImagePlot          *m_Plot;
  QxrdCenterFinderDialog *m_Dialog;
};

#endif // QXRDCENTERFINDERPICKER_H
