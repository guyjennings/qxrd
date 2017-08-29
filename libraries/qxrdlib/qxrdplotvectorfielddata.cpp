#include "qxrdplotvectorfielddata.h"

QxrdPlotVectorFieldData::QxrdPlotVectorFieldData()
{
}

void QxrdPlotVectorFieldData::append(const QxrdPoint4D &pt)
{
  d_samples.append(pt);
}

QRectF QxrdPlotVectorFieldData::boundingRect() const
{
  int n = d_samples.count();

  if (n > 0) {
    double minx, maxx, miny, maxy;

    for (int i=0; i<n; i++) {
      QxrdPoint4D pt = d_samples[i];
      double x0 = pt.x(),
             y0 = pt.y();
      double x1 = x0 + pt.z(),
             y1 = y0 + pt.t();

      if (i==0) {
        minx = qMin(x0, x1);
        maxx = qMax(x0, x1);
        miny = qMin(y0, y1);
        maxy = qMax(y0, y1);
      } else {
        minx = qMin(minx, x0);
        minx = qMin(minx, x1);
        maxx = qMax(maxx, x0);
        maxx = qMax(maxx, x1);
        miny = qMin(miny, y0);
        miny = qMin(miny, y1);
        maxy = qMax(maxy, y0);
        maxy = qMax(maxy, y1);
      }
    }

    return QRectF(minx, miny, maxx - minx, maxy - miny);
  } else {
    return QRectF();
  }
}

