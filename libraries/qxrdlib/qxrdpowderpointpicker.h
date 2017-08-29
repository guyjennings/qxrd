#ifndef QXRDPOWDERPOINTPICKER_H
#define QXRDPOWDERPOINTPICKER_H

#include "qxrdlib_global.h"
#include "qxrdimageplotmeasurer.h"

class QXRD_EXPORT QxrdPowderPointPicker : public QxrdImagePlotMeasurer
{
public:
  QxrdPowderPointPicker(QxrdImagePlot *plot);

private:
  QxrdImagePlot *m_Plot;
};

#endif // QXRDPOWDERPOINTPICKER_H
