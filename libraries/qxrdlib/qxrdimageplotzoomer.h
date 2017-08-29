#ifndef QXRDIMAGEPLOTZOOMER_H
#define QXRDIMAGEPLOTZOOMER_H

#include "qcepplotzoomer.h"
#include "qxrdimageplot.h"

class QxrdImagePlotZoomer : public QcepPlotZoomer
{
  Q_OBJECT

public:
  QxrdImagePlotZoomer(QWidget *canvas, QxrdImagePlot *plot);

private:
  QxrdImagePlot *m_ImagePlot;
};

#endif // QXRDIMAGEPLOTZOOMER_H
