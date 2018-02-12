#ifndef QXRDROIRECTANGLE_H
#define QXRDROIRECTANGLE_H

#include "qxrdlib_global.h"
#include "qxrdroicenteredshape.h"

class QXRD_EXPORT QxrdROIRectangle : public QxrdROICenteredShape
{
  Q_OBJECT

private:
  typedef QxrdROICenteredShape inherited;

public:
  QxrdROIRectangle(QString name,
                   double halfWidth,
                   double halfHeight);

  QxrdROIRectangle(QString name,
                   double scale);

public:
  virtual int markerCount();
  virtual QPointF markerPoint(int n);
  virtual bool pointInShape(QPointF pt);
  virtual QPolygonF dragOutline();
};

#endif // QXRDROIRECTANGLE_H
