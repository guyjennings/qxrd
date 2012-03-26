#ifndef QXRDSERVER_PTR_H
#define QXRDSERVER_PTR_H

#include <QSharedPointer>

class QxrdServer;

typedef QSharedPointer<QxrdServer> QxrdServerPtr;
typedef QWeakPointer<QxrdServer>   QxrdServerWPtr;

#endif
