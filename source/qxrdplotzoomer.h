#ifndef QXRDPLOTZOOMER_H
#define QXRDPLOTZOOMER_H

#include <qwt_plot_zoomer.h>
#include "qxrdplot.h"
#include "qxrdimageplot.h"

class QxrdPlotZoomer : public QwtPlotZoomer
{
  Q_OBJECT

public:
  QxrdPlotZoomer(QWidget *canvas, QxrdPlot *plot);

public:
  virtual QwtText trackerText(const QPointF &pos) const;

private:
  QxrdPlot *m_Plot;
};

class QxrdImagePlotZoomer : public QxrdPlotZoomer
{
  Q_OBJECT

public:
  QxrdImagePlotZoomer(QWidget *canvas, QxrdImagePlot *plot);

private:
  QxrdImagePlot *m_ImagePlot;
};

#endif // QXRDPLOTZOOMER_H
