#ifndef QXRDPLOTVECTORFIELDDATA_H
#define QXRDPLOTVECTORFIELDDATA_H

#include "qxrdlib_global.h"
#include "qwt_series_data.h"
#include "qxrdpoint4d.h"

class QXRD_EXPORT QxrdPlotVectorFieldData :
    public QwtArraySeriesData<QxrdPoint4D>
{
public:
  QxrdPlotVectorFieldData();

  void append(const QxrdPoint4D & pt);

  QRectF boundingRect() const;
};


#endif // QXRDPLOTVECTORFIELDDATA_H
