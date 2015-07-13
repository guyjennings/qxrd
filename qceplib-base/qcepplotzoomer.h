#ifndef QCEPPLOTZOOMER_H
#define QCEPPLOTZOOMER_H

#include <qwt_plot_zoomer.h>
#include "qcepplot.h"

class QcepPlotZoomer : public QwtPlotZoomer
{
  Q_OBJECT

public:
  QcepPlotZoomer(QWidget *canvas, QcepPlot *plot);

public:
  virtual QwtText trackerTextF(const QPointF &pos) const;

private:
  QcepPlot *m_Plot;
};

#endif // QCEPPLOTZOOMER_H
