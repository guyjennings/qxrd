#ifndef QXRDACQUISITIONINTERFACEPTR_H
#define QXRDACQUISITIONINTERFACEPTR_H

#include <QSharedPointer>

class QxrdAcquisitionInterface;

typedef QSharedPointer<QxrdAcquisitionInterface> QxrdAcquisitionInterfacePtr;
typedef QWeakPointer<QxrdAcquisitionInterface>   QxrdAcquisitionInterfaceWPtr;

#endif // QXRDACQUISITIONINTERFACEPTR_H

