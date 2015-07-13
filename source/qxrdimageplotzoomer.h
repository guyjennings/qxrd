#ifndef QXRDIMAGEPLOTZOOMER_H
#define QXRDIMAGEPLOTZOOMER_H

#include "qxrdplotzoomer.h"
#include "qxrdimageplot.h"

class QxrdImagePlotZoomer : public QxrdPlotZoomer
{
  Q_OBJECT

public:
  QxrdImagePlotZoomer(QWidget *canvas, QxrdImagePlot *plot);

private:
  QxrdImagePlot *m_ImagePlot;
};

#endif // QXRDIMAGEPLOTZOOMER_H
