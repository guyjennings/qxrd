#ifndef QXRDROICACHEPTR_H
#define QXRDROICACHEPTR_H

#include <QSharedPointer>

class QxrdROICache;

typedef QSharedPointer<QxrdROICache> QxrdROICachePtr;
typedef QWeakPointer<QxrdROICache>   QxrdROICacheWPtr;

#endif // QXRDROICACHEPTR_H
