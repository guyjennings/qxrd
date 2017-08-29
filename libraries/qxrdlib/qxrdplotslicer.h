#ifndef QXRDPLOTSLICER_H
#define QXRDPLOTSLICER_H

#include "qxrdlib_global.h"
#include "qxrdimageplotmeasurer.h"

class QXRD_EXPORT QxrdPlotSlicer : public QxrdImagePlotMeasurer
{
public:
  QxrdPlotSlicer(QWidget *canvas, QxrdImagePlot *plot);

//  virtual QwtText trackerTextF(const QPointF &pos) const;

private:
  QxrdImagePlot *m_Plot;
};

#endif // QXRDPLOTSLICER_H
