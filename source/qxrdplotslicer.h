#ifndef QXRDPLOTSLICER_H
#define QXRDPLOTSLICER_H

#include "qcepmacros.h"

#include "qxrdforwardtypes.h"
#include "qxrdimageplotmeasurer.h"

class QxrdPlotSlicer : public QxrdImagePlotMeasurer
{
public:
  QxrdPlotSlicer(QwtPlotCanvasPtr canvas, QxrdImagePlotPtr plot);

//  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

private:
  QxrdImagePlotPtr m_Plot;
};

#endif // QXRDPLOTSLICER_H
