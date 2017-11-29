#ifndef QXRDPROCESSORINTERFACEPTR_H
#define QXRDPROCESSORINTERFACEPTR_H

#include <QSharedPointer>
class QxrdProcessorInterface;

typedef QSharedPointer<QxrdProcessorInterface> QxrdProcessorInterfacePtr;
typedef QWeakPointer<QxrdProcessorInterface>   QxrdProcessorInterfaceWPtr;

#endif // QXRDPROCESSORINTERFACEPTR_H
