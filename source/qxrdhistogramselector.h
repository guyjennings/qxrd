#ifndef QXRDHISTOGRAMSELECTOR_H
#define QXRDHISTOGRAMSELECTOR_H

#include "qxrdplotmeasurer.h"

class QxrdHistogramSelector : public QxrdPlotMeasurer
{
    Q_OBJECT
public:
    explicit QxrdHistogramSelector(QWidget *canvas, QxrdPlot *plot);

signals:

public slots:

};

#endif // QXRDHISTOGRAMSELECTOR_H
