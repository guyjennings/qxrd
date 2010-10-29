#ifndef QXRDPLOTZOOMER_H
#define QXRDPLOTZOOMER_H

#include "qcepmacros.h"

#include "qxrdforwardtypes.h"
#include <qwt_plot_zoomer.h>

class QxrdPlotZoomer : public QwtPlotZoomer
{
  Q_OBJECT;

public:
  QxrdPlotZoomer(QwtPlotCanvasPtr canvas, QxrdPlotPtr plot);

public:
  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

private:
  QxrdPlotPtr m_Plot;
};

class QxrdImagePlotZoomer : public QxrdPlotZoomer
{
  Q_OBJECT;

public:
  QxrdImagePlotZoomer(QwtPlotCanvasPtr canvas, QxrdImagePlotPtr plot);

private:
  QxrdImagePlotPtr m_ImagePlot;
};

#endif // QXRDPLOTZOOMER_H
