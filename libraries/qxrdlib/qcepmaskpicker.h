#ifndef QCEPMASKPICKER_H
#define QCEPMASKPICKER_H

#include "qceplib_global.h"
#include "qcepimageplotmeasurer.h"

class QCEP_EXPORT QcepMaskPicker : public QcepImagePlotMeasurer
{
public:
  QcepMaskPicker(QcepImagePlot *plot);

protected:
  QcepImagePlot *m_Plot;
};

class QCEP_EXPORT QcepRectangularMaskPicker : public QcepMaskPicker
{
public:
  QcepRectangularMaskPicker(QcepImagePlot *plot);
};

class QCEP_EXPORT QcepCircularMaskPicker : public QcepMaskPicker
{
public:
  QcepCircularMaskPicker(QcepImagePlot *plot);

  QPolygon adjustedPoints(const QPolygon &points) const;
};

class QCEP_EXPORT QcepPolygonalMaskPicker : public QcepMaskPicker
{
public:
  QcepPolygonalMaskPicker(QcepImagePlot *plot);
};

#endif // QCEPMASKPICKER_H
