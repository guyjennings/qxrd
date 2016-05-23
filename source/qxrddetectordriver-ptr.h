#ifndef QXRDDETECTORDRIVERPTR_H
#define QXRDDETECTORDRIVERPTR_H

#include <QSharedPointer>

class QxrdDetectorDriver;

typedef QSharedPointer<QxrdDetectorDriver> QxrdDetectorDriverPtr;
typedef QWeakPointer<QxrdDetectorDriver>   QxrdDetectorDriverWPtr;

#endif // QXRDDETECTORDRIVERPTR_H
