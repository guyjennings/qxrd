#ifndef QXRDTESTSCANPLOTDATAVISHELPERPTR_H
#define QXRDTESTSCANPLOTDATAVISHELPERPTR_H

#ifdef HAVE_DATAVIS

#include <QSharedPointer>

class QxrdTestScanPlotDataVisHelper;

typedef QSharedPointer<QxrdTestScanPlotDataVisHelper> QxrdTestScanPlotDataVisHelperPtr;
typedef QWeakPointer<QxrdTestScanPlotDataVisHelper>   QxrdTestScanPlotDataVisHelperWPtr;

#endif

#endif // QXRDTESTSCANPLOTDATAVISHELPERPTR_H
