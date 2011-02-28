#ifndef QXRDIMAGEPLOTMEASURER_H
#define QXRDIMAGEPLOTMEASURER_H

#include "qcepmacros.h"

#include "qxrdforwardtypes.h"
#include "qxrdplotmeasurer.h"

#include <qwt_plot_picker.h>

class QxrdImagePlotMeasurer : public QxrdPlotMeasurer
{
  Q_OBJECT;

public:
  QxrdImagePlotMeasurer(QwtPlotCanvasPtr canvas, QxrdImagePlotPtr plot);

private:
  QxrdImagePlotPtr m_Plot;
};

#endif // QXRDIMAGEPLOTMEASURER_H
