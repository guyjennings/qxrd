#ifndef QXRDPLOTSLICER_H
#define QXRDPLOTSLICER_H

#include "qxrdimageplotmeasurer.h"

class QxrdPlotSlicer : public QxrdImagePlotMeasurer
{
public:
  QxrdPlotSlicer(QWidget *canvas, QxrdImagePlot *plot);

//  virtual QwtText trackerText(const QPointF &pos) const;

private:
  QxrdImagePlot *m_Plot;
};

#endif // QXRDPLOTSLICER_H
