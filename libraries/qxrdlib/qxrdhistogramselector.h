#ifndef QXRDHISTOGRAMSELECTOR_H
#define QXRDHISTOGRAMSELECTOR_H

#include "qxrdlib_global.h"
#include "qcepplotmeasurer.h"

class QXRD_EXPORT QxrdHistogramSelector : public QcepPlotMeasurer
{
    Q_OBJECT
public:
    explicit QxrdHistogramSelector(QcepPlot *plot);
};

#endif // QXRDHISTOGRAMSELECTOR_H
