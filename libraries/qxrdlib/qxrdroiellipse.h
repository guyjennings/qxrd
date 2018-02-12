#ifndef QXRDROIELLIPSE_H
#define QXRDROIELLIPSE_H

#include "qxrdlib_global.h"
#include "qxrdroicenteredshape.h"

class QXRD_EXPORT QxrdROIEllipse : public QxrdROICenteredShape
{
  Q_OBJECT

private:
  typedef QxrdROICenteredShape inherited;

public:
  QxrdROIEllipse(QString name,
                 double halfWidth,
                 double halfHeight);

  QxrdROIEllipse(QString name,
                 double scale);

  virtual int markerCount();
  virtual QPointF markerPoint(int n);
  virtual bool pointInShape(QPointF pt);
  virtual QPolygonF dragOutline();
};

#endif // QXRDROIELLIPSE_H
