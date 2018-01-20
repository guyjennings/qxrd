#ifndef QXRDIMAGEPLOTZOOMER_H
#define QXRDIMAGEPLOTZOOMER_H

#include "qxrdlib_global.h"
#include "qcepplotzoomer.h"
#include "qxrdimageplot.h"

class QXRD_EXPORT QxrdImagePlotZoomer : public QcepPlotZoomer
{
  Q_OBJECT

public:
  QxrdImagePlotZoomer(QxrdImagePlot *plot);

//private:
//  QxrdImagePlot *m_ImagePlot;
};

#endif // QXRDIMAGEPLOTZOOMER_H
