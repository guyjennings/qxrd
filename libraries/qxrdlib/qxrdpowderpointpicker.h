#ifndef QXRDPOWDERPOINTPICKER_H
#define QXRDPOWDERPOINTPICKER_H

#include "qxrdlib_global.h"
#include "qxrdimageplotmeasurer.h"

class QXRD_EXPORT QxrdPowderPointPicker : public QxrdImagePlotMeasurer
{
public:
  QxrdPowderPointPicker(QcepImagePlot *plot);
};

#endif // QXRDPOWDERPOINTPICKER_H
