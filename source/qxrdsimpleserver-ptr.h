#ifndef QXRDSIMPLESERVER_PTR_H
#define QXRDSIMPLESERVER_PTR_H

#include <QSharedPointer>

class QxrdSimpleServer;

typedef QSharedPointer<QxrdSimpleServer> QxrdSimpleServerPtr;
typedef QWeakPointer<QxrdSimpleServer>   QxrdSimpleServerWPtr;

#endif // QXRDSIMPLESERVER_PTR_H
