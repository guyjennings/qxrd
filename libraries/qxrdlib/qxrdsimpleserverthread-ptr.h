#ifndef QXRDSIMPLESERVERTHREAD_PTR_H
#define QXRDSIMPLESERVERTHREAD_PTR_H

#include <QSharedPointer>

class QxrdSimpleServerThread;

typedef QSharedPointer<QxrdSimpleServerThread> QxrdSimpleServerThreadPtr;
typedef QWeakPointer<QxrdSimpleServerThread>   QxrdSimpleServerThreadWPtr;

#endif // QXRDSIMPLESERVERTHREAD_PTR_H
