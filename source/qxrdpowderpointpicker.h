#ifndef QXRDPOWDERPOINTPICKER_H
#define QXRDPOWDERPOINTPICKER_H

#include "qxrdimageplotmeasurer.h"

class QxrdPowderPointPicker : public QxrdImagePlotMeasurer
{
public:
  QxrdPowderPointPicker(QxrdImagePlot *plot);

private:
  QxrdImagePlot *m_Plot;
};

#endif // QXRDPOWDERPOINTPICKER_H
