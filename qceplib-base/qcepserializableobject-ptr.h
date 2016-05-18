#ifndef QCEPSERIALIZABLEOBJECTPTR_H
#define QCEPSERIALIZABLEOBJECTPTR_H

#include <QSharedPointer>

class QcepSerializableObject;

typedef QSharedPointer<QcepSerializableObject> QcepSerializableObjectPtr;
typedef QWeakPointer<QcepSerializableObject>   QcepSerializableObjectWPtr;

#endif // QCEPSERIALIZABLEOBJECTPTR_H
