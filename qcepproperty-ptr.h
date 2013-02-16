#ifndef QCEPPROPERTY_PTR_H
#define QCEPPROPERTY_PTR_H

#include <QSharedPointer>

class QcepProperty;
class QcepDoubleProperty;
class QcepIntProperty;
class QcepBoolProperty;
class QcepStringProperty;
class QcepDateTimeProperty;
class QcepDoubleListProperty;
class QcepStringListProperty;
class QcepByteArrayProperty;

typedef QSharedPointer<QcepProperty> QcepPropertyPtr;
typedef QWeakPointer<QcepProperty>   QcepPropertyWPtr;

#endif // QCEPPROPERTY_H
