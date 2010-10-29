#ifndef QXRDPLOTMEASURER_H
#define QXRDPLOTMEASURER_H

#include "qcepmacros.h"

#include "qxrdforwardtypes.h"
#include <qwt_plot_picker.h>

class QxrdPlotMeasurer : public QwtPlotPicker
{
public:
  QxrdPlotMeasurer(QwtPlotCanvasPtr canvas, QxrdPlotPtr plot);

  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

private:
  QxrdPlotPtr m_Plot;
};

#endif // QXRDPLOTMEASURER_H
