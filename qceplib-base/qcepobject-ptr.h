#ifndef QCEPOBJECTPTR_H
#define QCEPOBJECTPTR_H

#include <QSharedPointer>

class QcepObject;

typedef QSharedPointer<QcepObject> QcepObjectPtr;
typedef QWeakPointer<QcepObject>   QcepObjectWPtr;

#endif // QCEPOBJECTPTR_H

