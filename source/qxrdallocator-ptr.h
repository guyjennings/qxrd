#ifndef QXRDALLOCATOR_PTR_H
#define QXRDALLOCATOR_PTR_H

#include <QSharedPointer>

class QxrdAllocator;
typedef QSharedPointer<QxrdAllocator> QxrdAllocatorPtr;
typedef QWeakPointer<QxrdAllocator> QxrdAllocatorWPtr;

#endif
