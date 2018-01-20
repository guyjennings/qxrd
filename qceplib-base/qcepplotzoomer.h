#ifndef QCEPPLOTZOOMER_H
#define QCEPPLOTZOOMER_H

#include "qceplib_global.h"
#include <qwt_plot_zoomer.h>
#include "qcepplot.h"

class QCEP_EXPORT QcepPlotZoomer : public QwtPlotZoomer
{
  Q_OBJECT

public:
  QcepPlotZoomer(QcepPlot *plot);

public:
  virtual QwtText trackerTextF(const QPointF &pos) const;

private:
  QcepPlot *m_Plot;
};

#endif // QCEPPLOTZOOMER_H
