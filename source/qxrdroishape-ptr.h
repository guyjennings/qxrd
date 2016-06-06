#ifndef QXRDROISHAPEPTR_H
#define QXRDROISHAPEPTR_H

#include <QSharedPointer>

class QxrdROIShape;

typedef QSharedPointer<QxrdROIShape> QxrdROIShapePtr;
typedef QWeakPointer<QxrdROIShape>   QxrdROIShapeWPtr;

#endif // QXRDROISHAPEPTR_H
