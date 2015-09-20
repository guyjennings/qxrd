#ifndef QXRDTODOLISTITEMPTR_H
#define QXRDTODOLISTITEMPTR_H

#include <QSharedPointer>

class QxrdToDoListItem;

typedef QSharedPointer<QxrdToDoListItem> QxrdToDoListItemPtr;
typedef QWeakPointer<QxrdToDoListItem>   QxrdToDoListItemWPtr;

#endif // QXRDTODOLISTITEMPTR_H

