#ifndef QCEPDATAOBJECTPTR_H
#define QCEPDATAOBJECTPTR_H

#include <QSharedPointer>

class QcepDataObject;
typedef QSharedPointer<QcepDataObject> QcepDataObjectPtr;
typedef QWeakPointer<QcepDataObject> QcepDataObjectWPtr;

#endif // QCEPDATAOBJECTPTR_H

