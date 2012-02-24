#ifndef QXRDMASKPICKER_H
#define QXRDMASKPICKER_H

#include "qxrdimageplotmeasurer.h"

class QxrdMaskPicker : public QxrdImagePlotMeasurer
{
public:
  QxrdMaskPicker(QwtPlotCanvas *canvas, QxrdImagePlot *plot);

private:
  QxrdImagePlot *m_Plot;
};

class QxrdCircularMaskPicker : public QxrdMaskPicker
{
public:
  QxrdCircularMaskPicker(QwtPlotCanvas *canvas, QxrdImagePlot *plot);
};

class QxrdPolygonalMaskPicker : public QxrdMaskPicker
{
public:
  QxrdPolygonalMaskPicker(QwtPlotCanvas *canvas, QxrdImagePlot *plot);
};

#endif // QXRDMASKPICKER_H

class QxrdMaskPicker;
class QxrdCircularMaskPicker;
class QxrdPolygonalMaskPicker;
