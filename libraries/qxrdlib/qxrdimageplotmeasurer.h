#ifndef QXRDIMAGEPLOTMEASURER_H
#define QXRDIMAGEPLOTMEASURER_H

#include "qxrdlib_global.h"
#include "qcepplotmeasurer.h"
#include "qcepimageplot-ptr.h"

class QXRD_EXPORT QxrdImagePlotMeasurer : public QcepPlotMeasurer
{
  Q_OBJECT

public:
  QxrdImagePlotMeasurer(QcepImagePlot *plot);

  QcepImagePlot *imagePlot() const;

protected:
  QcepImagePlot *m_Plot;
};

#endif // QXRDIMAGEPLOTMEASURER_H
