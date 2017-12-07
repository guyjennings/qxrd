#ifndef QCEPSHAREDPOINTER_H
#define QCEPSHAREDPOINTER_H

#ifndef QT_NO_DEBUG
#define QT_SHAREDPOINTER_TRACK_POINTERS
#endif

#include <QSharedPointer>

#define QCEP_SHAREDPOINTER(T) \
class T; \
typedef QSharedPointer<T> T##Ptr; \
typedef QWeakPointer<T>   T##WPtr;

#endif // QCEPSHAREDPOINTER_H
