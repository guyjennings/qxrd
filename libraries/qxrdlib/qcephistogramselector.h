#ifndef QCEPHISTOGRAMSELECTOR_H
#define QCEPHISTOGRAMSELECTOR_H

#include "qceplib_global.h"
#include "qcepplotmeasurer.h"

class QCEP_EXPORT QcepHistogramSelector : public QcepPlotMeasurer
{
    Q_OBJECT
public:
    explicit QcepHistogramSelector(QcepPlot *plot);
};

#endif // QCEPHISTOGRAMSELECTOR_H
