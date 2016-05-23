#ifndef QXRDDETECTORDRIVERTHREADPTR_H
#define QXRDDETECTORDRIVERTHREADPTR_H

#include <QSharedPointer>

class QxrdDetectorDriverThread;

typedef QSharedPointer<QxrdDetectorDriverThread> QxrdDetectorDriverThreadPtr;
typedef QWeakPointer<QxrdDetectorDriverThread>   QxrdDetectorDriverThreadWPtr;

#endif // QXRDDETECTORDRIVERTHREADPTR_H
