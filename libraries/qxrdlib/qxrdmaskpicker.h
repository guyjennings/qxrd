#ifndef QXRDMASKPICKER_H
#define QXRDMASKPICKER_H

#include "qxrdlib_global.h"
#include "qxrdimageplotmeasurer.h"

class QXRD_EXPORT QxrdMaskPicker : public QxrdImagePlotMeasurer
{
public:
  QxrdMaskPicker(QxrdImagePlot *plot);

protected:
  QxrdImagePlot *m_Plot;
};

class QXRD_EXPORT QxrdCircularMaskPicker : public QxrdMaskPicker
{
public:
  QxrdCircularMaskPicker(QxrdImagePlot *plot);

  QPolygon adjustedPoints(const QPolygon &points) const;
};

class QXRD_EXPORT QxrdPolygonalMaskPicker : public QxrdMaskPicker
{
public:
  QxrdPolygonalMaskPicker(QxrdImagePlot *plot);
};

#endif // QXRDMASKPICKER_H
