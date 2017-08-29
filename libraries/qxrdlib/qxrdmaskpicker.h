#ifndef QXRDMASKPICKER_H
#define QXRDMASKPICKER_H

#include "qxrdlib_global.h"
#include "qxrdimageplotmeasurer.h"

class QXRD_EXPORT QxrdMaskPicker : public QxrdImagePlotMeasurer
{
public:
  QxrdMaskPicker(QWidget *canvas, QxrdImagePlot *plot);

protected:
  QxrdImagePlot *m_Plot;
};

class QXRD_EXPORT QxrdCircularMaskPicker : public QxrdMaskPicker
{
public:
  QxrdCircularMaskPicker(QWidget *canvas, QxrdImagePlot *plot);

  QPolygon adjustedPoints(const QPolygon &points) const;
};

class QXRD_EXPORT QxrdPolygonalMaskPicker : public QxrdMaskPicker
{
public:
  QxrdPolygonalMaskPicker(QWidget *canvas, QxrdImagePlot *plot);

protected:
  void append(const QPoint &pt);
  bool end(bool ok=true);
};

#endif // QXRDMASKPICKER_H
