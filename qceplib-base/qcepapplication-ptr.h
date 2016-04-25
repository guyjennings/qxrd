#ifndef QCEPAPPLICATIONPTR_H
#define QCEPAPPLICATIONPTR_H

#include <QSharedPointer>

class QcepApplication;

typedef QSharedPointer<QcepApplication> QcepApplicationPtr;
typedef QWeakPointer<QcepApplication>   QcepApplicationWPtr;

#endif // QCEPAPPLICATIONPTR_H

