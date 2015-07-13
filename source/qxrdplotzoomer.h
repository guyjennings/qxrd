#ifndef QXRDPLOTZOOMER_H
#define QXRDPLOTZOOMER_H

#include "qcepplot.h"
#include "qcepplotzoomer.h"

class QxrdPlotZoomer : public QcepPlotZoomer
{
  Q_OBJECT

public:
  QxrdPlotZoomer(QWidget *canvas, QcepPlot *plot);
};

#endif // QXRDPLOTZOOMER_H
