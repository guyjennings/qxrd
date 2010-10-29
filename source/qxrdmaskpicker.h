#ifndef QXRDMASKPICKER_H
#define QXRDMASKPICKER_H

#include "qcepmacros.h"

#include "qxrdforwardtypes.h"
#include "qxrdimageplotmeasurer.h"

class QxrdMaskPicker : public QxrdImagePlotMeasurer
{
public:
  QxrdMaskPicker(QwtPlotCanvasPtr canvas, QxrdImagePlotPtr plot);

private:
  QxrdImagePlotPtr m_Plot;
};

class QxrdCircularMaskPicker : public QxrdMaskPicker
{
public:
  QxrdCircularMaskPicker(QwtPlotCanvasPtr canvas, QxrdImagePlotPtr plot);
};

class QxrdPolygonalMaskPicker : public QxrdMaskPicker
{
public:
  QxrdPolygonalMaskPicker(QwtPlotCanvasPtr canvas, QxrdImagePlotPtr plot);
};

#endif // QXRDMASKPICKER_H
