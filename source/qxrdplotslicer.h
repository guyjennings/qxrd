#ifndef QXRDPLOTSLICER_H
#define QXRDPLOTSLICER_H

#include "qxrdimageplotmeasurer.h"
#include "qxrdimageplot.h"

class QxrdPlotSlicer : public QxrdImagePlotMeasurer
{
public:
  QxrdPlotSlicer(QwtPlotCanvas *canvas, QxrdImagePlot *plot);

//  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

private:
  QxrdImagePlot *m_Plot;
};

#endif // QXRDPLOTSLICER_H

class QxrdPlotSlicer;
