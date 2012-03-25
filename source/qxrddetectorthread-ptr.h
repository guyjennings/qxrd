#ifndef QXRDDETECTORTHREADPTR_H
#define QXRDDETECTORTHREADPTR_H

#include <QSharedPointer>

class QxrdDetectorThread;

typedef QSharedPointer<QxrdDetectorThread> QxrdDetectorThreadPtr;
typedef QWeakPointer<QxrdDetectorThread>   QxrdDetectorThreadWPtr;

#endif // QXRDDETECTORTHREADPTR_H
