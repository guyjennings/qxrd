#ifndef QXRDTESTIMAGEPLOTDATAVISHELPERPTR_H
#define QXRDTESTIMAGEPLOTDATAVISHELPERPTR_H

#ifdef HAVE_DATAVIS

#include <QSharedPointer>

class QxrdTestImagePlotDataVisHelper;

typedef QSharedPointer<QxrdTestImagePlotDataVisHelper> QxrdTestImagePlotDataVisHelperPtr;
typedef QWeakPointer<QxrdTestImagePlotDataVisHelper>   QxrdTestImagePlotDataVisHelperWPtr;

#endif // HAVE_DATAVIS

#endif // QXRDTESTIMAGEPLOTDATAVISHELPERPTR_H
