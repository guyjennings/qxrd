#ifndef QXRDPLOTMEASURER_H
#define QXRDPLOTMEASURER_H

#include <qwt_plot_picker.h>

#include "qxrdplot-ptr.h"

class QxrdPlotMeasurer : public QwtPlotPicker
{
  Q_OBJECT

public:
  QxrdPlotMeasurer(QwtPlotCanvas *canvas, QxrdPlotPtr plot);

  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

private:
  QxrdPlotPtr m_Plot;
};

#endif // QXRDPLOTMEASURER_H