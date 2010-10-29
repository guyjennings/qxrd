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

  HEADER_IDENT("$Id: qxrdcenterfinderpicker.h,v 1.3 2010/09/17 16:21:51 jennings Exp $");
};

#endif // QXRDCENTERFINDERPICKER_H

