#ifndef QXRDMASKPICKER_H
#define QXRDMASKPICKER_H

#include "qxrdimageplotmeasurer.h"

class QxrdMaskPicker : public QxrdImagePlotMeasurer
{
public:
  QxrdMaskPicker(QWidget *canvas, QxrdImagePlot *plot);

protected:
  QxrdImagePlot *m_Plot;
};

class QxrdCircularMaskPicker : public QxrdMaskPicker
{
public:
  QxrdCircularMaskPicker(QWidget *canvas, QxrdImagePlot *plot);
};

class QxrdPolygonalMaskPicker : public QxrdMaskPicker
{
public:
  QxrdPolygonalMaskPicker(QWidget *canvas, QxrdImagePlot *plot);

protected:
  void append(const QPoint &pt);
  bool end(bool ok=true);
};

#endif // QXRDMASKPICKER_H
