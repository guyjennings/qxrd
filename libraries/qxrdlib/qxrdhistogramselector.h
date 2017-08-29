#ifndef QXRDHISTOGRAMSELECTOR_H
#define QXRDHISTOGRAMSELECTOR_H

#include "qcepplotmeasurer.h"

class QxrdHistogramSelector : public QcepPlotMeasurer
{
    Q_OBJECT
public:
    explicit QxrdHistogramSelector(QWidget *canvas, QcepPlot *plot);

signals:

public slots:

};

#endif // QXRDHISTOGRAMSELECTOR_H
