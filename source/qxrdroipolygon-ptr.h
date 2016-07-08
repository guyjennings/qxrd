#ifndef QXRDROIPOLYGONPTR_H
#define QXRDROIPOLYGONPTR_H

#include <QSharedPointer>

class QxrdROIPolygon;

typedef QSharedPointer<QxrdROIPolygon> QxrdROIPolygonPtr;
typedef QWeakPointer<QxrdROIPolygon>   QxrdROIPolygonWPtr;

#endif // QXRDROIPOLYGONPTR_H
