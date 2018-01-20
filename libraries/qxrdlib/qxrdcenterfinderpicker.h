#ifndef QXRDCENTERFINDERPICKER_H
#define QXRDCENTERFINDERPICKER_H

#include "qxrdlib_global.h"
#include "qxrdimageplotmeasurer.h"

class QXRD_EXPORT QxrdCenterFinderPicker : public QxrdImagePlotMeasurer
{
  Q_OBJECT

public:
  QxrdCenterFinderPicker(QxrdImagePlot *plot);

//private:
//  QxrdImagePlot *m_Plot;
};

#endif // QXRDCENTERFINDERPICKER_H
