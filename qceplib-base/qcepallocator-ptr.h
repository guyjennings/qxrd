#ifndef QCEPALLOCATORPTR_H
#define QCEPALLOCATORPTR_H

#include <QSharedPointer>

class QcepAllocator;
typedef QSharedPointer<QcepAllocator> QcepAllocatorPtr;
typedef QWeakPointer<QcepAllocator> QcepAllocatorWPtr;

#endif // QCEPALLOCATORPTR_H

