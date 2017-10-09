#ifndef QXRDTESTSCANPLOTDATAVISHELPER_H
#define QXRDTESTSCANPLOTDATAVISHELPER_H

#ifdef HAVE_DATAVIS

#include "qxrdlib_global.h"
#include "qcepserializableobject.h"
#include "qcepdatacolumnscan-ptr.h"

class QXRD_EXPORT QxrdTestScanPlotDataVisHelper : public QcepSerializableObject
{
  Q_OBJECT

public:
  QxrdTestScanPlotDataVisHelper();

public slots:
  void onNewScanAvailable(QcepDataColumnScanPtr data);

signals:
  void newChartCurveAvailable();
};

#endif

#endif // QXRDTESTSCANPLOTDATAVISHELPER_H
