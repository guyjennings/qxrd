#ifndef QXRDTESTIMAGEPLOTDATAVISHELPER_H
#define QXRDTESTIMAGEPLOTDATAVISHELPER_H

#include "qcepserializableobject.h"
#include "qcepimagedata-ptr.h"
#include "qcepimagedata.h"
#include <QtDataVisualization>

typedef QSharedPointer<QtDataVisualization::QSurface3DSeries> QSurface3DSeriesPtr;

class QxrdTestImagePlotDataVisHelper : public QcepSerializableObject
{
  Q_OBJECT

public:
  QxrdTestImagePlotDataVisHelper();

public slots:
  virtual void onNewImageAvailable(QcepDoubleImageDataPtr data);

signals:
  void newSurfaceSeries(QSurface3DSeriesPtr surface);
};

Q_DECLARE_METATYPE(QSurface3DSeriesPtr)

#endif // QXRDTESTIMAGEPLOTDATAVISHELPER_H
