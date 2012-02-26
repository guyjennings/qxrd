#ifndef QXRDIMAGEPLOTMEASURER_H
#define QXRDIMAGEPLOTMEASURER_H

#include "qxrdplotmeasurer.h"

#include "qxrdimageplot-ptr.h"

class QxrdImagePlotMeasurer : public QxrdPlotMeasurer
{
  Q_OBJECT

public:
  QxrdImagePlotMeasurer(QwtPlotCanvas *canvas, QxrdImagePlot *plot);

private:
  QxrdImagePlot *m_Plot;
};

#endif // QXRDIMAGEPLOTMEASURER_H
