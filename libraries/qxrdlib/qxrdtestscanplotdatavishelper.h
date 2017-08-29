#ifndef QXRDTESTSCANPLOTDATAVISHELPER_H
#define QXRDTESTSCANPLOTDATAVISHELPER_H

#include "qcepserializableobject.h"
#include "qcepdatacolumnscan-ptr.h"

class QxrdTestScanPlotDataVisHelper : public QcepSerializableObject
{
  Q_OBJECT

public:
  QxrdTestScanPlotDataVisHelper();

public slots:
  void onNewScanAvailable(QcepDataColumnScanPtr data);

signals:
  void newChartCurveAvailable();
};

#endif // QXRDTESTSCANPLOTDATAVISHELPER_H
