#ifndef QCEPIMAGEPLOTMEASURER_H
#define QCEPIMAGEPLOTMEASURER_H

#include "qceplib_global.h"
#include "qcepplotmeasurer.h"
#include "qcepimageplot-ptr.h"

class QCEP_EXPORT QcepImagePlotMeasurer : public QcepPlotMeasurer
{
  Q_OBJECT

public:
  QcepImagePlotMeasurer(QcepImagePlot *plot);

  QcepImagePlot *imagePlot() const;

protected:
  QcepImagePlot *m_Plot;
};

#endif // QCEPIMAGEPLOTMEASURER_H
