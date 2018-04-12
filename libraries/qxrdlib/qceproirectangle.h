#ifndef QCEPROIRECTANGLE_H
#define QCEPROIRECTANGLE_H

#include "qceplib_global.h"
#include "qceproicenteredshape.h"

class QCEP_EXPORT QcepROIRectangle : public QcepROICenteredShape
{
  Q_OBJECT

private:
  typedef QcepROICenteredShape inherited;

public:
  QcepROIRectangle(QString name,
                   double halfWidth,
                   double halfHeight);

  QcepROIRectangle(QString name,
                   double scale);

public:
  virtual int markerCount();
  virtual QPointF markerPoint(int n);
  virtual bool pointInShape(QPointF pt);
  virtual QPolygonF dragOutline();
};

#endif // QCEPROIRECTANGLE_H
