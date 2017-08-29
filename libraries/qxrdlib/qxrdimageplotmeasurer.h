#ifndef QXRDIMAGEPLOTMEASURER_H
#define QXRDIMAGEPLOTMEASURER_H

#include "qcepplotmeasurer.h"
#include "qxrdimageplot-ptr.h"

class QxrdImagePlotMeasurer : public QcepPlotMeasurer
{
  Q_OBJECT

public:
  QxrdImagePlotMeasurer(QWidget *canvas, QxrdImagePlot *plot);

  QxrdImagePlot *imagePlot() const;

protected:
  QxrdImagePlot *m_Plot;
};

#endif // QXRDIMAGEPLOTMEASURER_H