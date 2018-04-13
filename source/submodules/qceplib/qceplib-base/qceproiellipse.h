#ifndef QCEPROIELLIPSE_H
#define QCEPROIELLIPSE_H

#include "qceplib_global.h"
#include "qceproicenteredshape.h"

class QCEP_EXPORT QcepROIEllipse : public QcepROICenteredShape
{
  Q_OBJECT

private:
  typedef QcepROICenteredShape inherited;

public:
  QcepROIEllipse(QString name,
                 double halfWidth,
                 double halfHeight);

  QcepROIEllipse(QString name,
                 double scale);

  virtual int markerCount();
  virtual QPointF markerPoint(int n);
  virtual bool pointInShape(QPointF pt);
  virtual QPolygonF dragOutline();
};

#endif // QCEPROIELLIPSE_H
