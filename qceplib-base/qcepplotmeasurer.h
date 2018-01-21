#ifndef QCEPPLOTMEASURER_H
#define QCEPPLOTMEASURER_H

#include "qceplib_global.h"
#include <qwt_plot_picker.h>

#include "qcepplot-ptr.h"

class QCEP_EXPORT QcepPlotMeasurer : public QwtPlotPicker
{
  Q_OBJECT

public:
  QcepPlotMeasurer(QcepPlot *plot);

  virtual QwtText trackerTextF(const QPointF &pos) const;

private:
  QcepPlot *m_Plot;
};

#endif // QCEPPLOTMEASURER_H
