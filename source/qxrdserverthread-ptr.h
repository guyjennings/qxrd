#ifndef QXRDSERVERTHREAD_PTR_H
#define QXRDSERVERTHREAD_PTR_H

#include <QSharedPointer>

class QxrdServerThread;

typedef QSharedPointer<QxrdServerThread> QxrdServerThreadPtr;
typedef QWeakPointer<QxrdServerThread>   QxrdServerThreadWPtr;

#endif // QXRDSERVERTHREAD_PTR_H
