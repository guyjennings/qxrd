#ifndef QXRDDETECTORPTR_H
#define QXRDDETECTORPTR_H

#include <QSharedPointer>

class QxrdDetectorSettings;

typedef QSharedPointer<QxrdDetectorSettings> QxrdDetectorPtr;
typedef QWeakPointer<QxrdDetectorSettings>   QxrdDetectorWPtr;

#endif // QXRDDETECTORPTR_H
