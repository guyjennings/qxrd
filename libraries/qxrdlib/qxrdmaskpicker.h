#ifndef QXRDMASKPICKER_H
#define QXRDMASKPICKER_H

#include "qxrdlib_global.h"
#include "qxrdimageplotmeasurer.h"

class QXRD_EXPORT QxrdMaskPicker : public QxrdImagePlotMeasurer
{
public:
  QxrdMaskPicker(QcepImagePlot *plot);

protected:
  QcepImagePlot *m_Plot;
};

class QXRD_EXPORT QxrdRectangularMaskPicker : public QxrdMaskPicker
{
public:
  QxrdRectangularMaskPicker(QcepImagePlot *plot);
};

class QXRD_EXPORT QxrdCircularMaskPicker : public QxrdMaskPicker
{
public:
  QxrdCircularMaskPicker(QcepImagePlot *plot);

  QPolygon adjustedPoints(const QPolygon &points) const;
};

class QXRD_EXPORT QxrdPolygonalMaskPicker : public QxrdMaskPicker
{
public:
  QxrdPolygonalMaskPicker(QcepImagePlot *plot);
};

#endif // QXRDMASKPICKER_H
