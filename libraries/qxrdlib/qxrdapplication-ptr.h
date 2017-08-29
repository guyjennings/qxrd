#ifndef QXRDAPPLICATION_PTR_H
#define QXRDAPPLICATION_PTR_H

#include <QSharedPointer>

class QxrdApplication;

typedef QSharedPointer<QxrdApplication> QxrdApplicationPtr;
typedef QWeakPointer<QxrdApplication>   QxrdApplicationWPtr;

#endif
