#ifndef QXRDMASKPICKER_H
#define QXRDMASKPICKER_H

#include "qxrdimageplotmeasurer.h"

class QxrdMaskPicker : public QxrdImagePlotMeasurer
{
public:
  QxrdMaskPicker(QwtPlotCanvas *canvas, QxrdImagePlot *plot);

protected:
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

protected:
  void append(const QPoint &pt);
  bool end(bool ok=true);
};

#endif // QXRDMASKPICKER_H
