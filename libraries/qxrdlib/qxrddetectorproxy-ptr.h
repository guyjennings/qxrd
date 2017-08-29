#ifndef QXRDDETECTORPROXYPTR_H
#define QXRDDETECTORPROXYPTR_H

#include <QSharedPointer>

class QxrdDetectorProxy;

typedef QSharedPointer<QxrdDetectorProxy> QxrdDetectorProxyPtr;
typedef QWeakPointer<QxrdDetectorProxy>   QxrdDetectorProxyWPtr;

#endif // QXRDDETECTORPROXYPTR_H

