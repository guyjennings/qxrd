#ifndef QCEPPLOTSLICER_H
#define QCEPPLOTSLICER_H

#include "qceplib_global.h"
#include "qcepimageplotmeasurer.h"

class QCEP_EXPORT QcepPlotSlicer : public QcepImagePlotMeasurer
{
public:
  QcepPlotSlicer(QcepImagePlot *plot);

//  virtual QwtText trackerTextF(const QPointF &pos) const;

//private:
//  QcepImagePlot *m_Plot;
};

#endif // QCEPPLOTSLICER_H
