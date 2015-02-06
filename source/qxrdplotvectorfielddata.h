#ifndef QXRDPLOTVECTORFIELDDATA_H
#define QXRDPLOTVECTORFIELDDATA_H

#include "qwt_series_data.h"
#include "qxrdpoint4d.h"

class QxrdPlotVectorFieldData :
    public QwtArraySeriesData<QxrdPoint4D>
{
public:
  QxrdPlotVectorFieldData();

  void append(const QxrdPoint4D & pt);

  QRectF boundingRect() const;
};


#endif // QXRDPLOTVECTORFIELDDATA_H
