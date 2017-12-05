#ifndef QXRDTESTSCANPLOTDATAVISHELPER_H
#define QXRDTESTSCANPLOTDATAVISHELPER_H

#ifdef HAVE_DATAVIS

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qcepdatacolumnscan-ptr.h"

class QXRD_EXPORT QxrdTestScanPlotDataVisHelper : public QcepObject
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
