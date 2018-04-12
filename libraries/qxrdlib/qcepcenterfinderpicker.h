#ifndef QCEPCENTERFINDERPICKER_H
#define QCEPCENTERFINDERPICKER_H

#include "qceplib_global.h"
#include "qcepimageplotmeasurer.h"

class QCEP_EXPORT QcepCenterFinderPicker : public QcepImagePlotMeasurer
{
  Q_OBJECT

public:
  QcepCenterFinderPicker(QcepImagePlot *plot);

//private:
//  QxrdImagePlot *m_Plot;
};

#endif // QCEPCENTERFINDERPICKER_H
