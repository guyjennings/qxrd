#ifndef QXRDDETECTORPTR_H
#define QXRDDETECTORPTR_H

#include <QSharedPointer>

class QxrdDetector;

typedef QSharedPointer<QxrdDetector> QxrdDetectorPtr;
typedef QWeakPointer<QxrdDetector>   QxrdDetectorWPtr;

#endif // QXRDDETECTORPTR_H
