#ifndef QXRDTESTSCANPLOTQWTHELPER_H
#define QXRDTESTSCANPLOTQWTHELPER_H

#include "qcepserializableobject.h"
#include "qcepdatacolumnscan-ptr.h"
#include "qcepdatacolumnscan.h"
#include <qwt_plot_curve.h>

typedef QSharedPointer<QwtPlotCurve> QwtPlotCurvePtr;

class QxrdTestScanPlotQwtHelper : public QcepSerializableObject
{
  Q_OBJECT

public:
  QxrdTestScanPlotQwtHelper();

public slots:
  void onNewScanAvailable(QcepDataColumnScanPtr data);

signals:
  void newPlotCurve(QwtPlotCurvePtr pc);
};

Q_DECLARE_METATYPE(QwtPlotCurvePtr)

#endif // QXRDTESTSCANPLOTQWTHELPER_H
