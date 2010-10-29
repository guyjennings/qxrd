#ifndef QXRDCENTERFINDERPICKER_H
#define QXRDCENTERFINDERPICKER_H

#include "qcepmacros.h"
#include "qxrdforwardtypes.h"
#include "qxrdimageplotmeasurer.h"

class QxrdCenterFinderPicker : public QxrdImagePlotMeasurer
{
public:
  QxrdCenterFinderPicker(QxrdImagePlotPtr plot);

private:
  QxrdImagePlotPtr m_Plot;
};

#endif // QXRDCENTERFINDERPICKER_H

