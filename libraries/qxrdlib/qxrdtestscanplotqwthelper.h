#ifndef QXRDTESTSCANPLOTQWTHELPER_H
#define QXRDTESTSCANPLOTQWTHELPER_H

#include "qxrdlib_global.h"
#include "qcepserializableobject.h"
#include "qcepdatacolumnscan-ptr.h"
#include "qcepdatacolumnscan.h"
#include <qwt_plot_curve.h>

typedef QSharedPointer<QwtPlotCurve> QwtPlotCurvePtr;

class QXRD_EXPORT QxrdTestScanPlotQwtHelper : public QcepSerializableObject
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
