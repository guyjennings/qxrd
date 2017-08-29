#ifndef QXRDCENTERFINDERPICKER_H
#define QXRDCENTERFINDERPICKER_H

#include "qxrdimageplotmeasurer.h"

class QxrdCenterFinderPicker : public QxrdImagePlotMeasurer
{
public:
  QxrdCenterFinderPicker(QxrdImagePlot *plot);

private:
  QxrdImagePlot *m_Plot;
};

#endif // QXRDCENTERFINDERPICKER_H
