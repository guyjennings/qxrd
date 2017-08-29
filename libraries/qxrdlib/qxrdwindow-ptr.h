#ifndef QXRDWINDOW_PTR_H
#define QXRDWINDOW_PTR_H

#include <QSharedPointer>

class QxrdWindow;

typedef QSharedPointer<QxrdWindow> QxrdWindowPtr;
typedef QWeakPointer<QxrdWindow>   QxrdWindowWPtr;

//typedef QxrdWindow *QxrdWindowPtr;
//typedef QxrdWindow *QxrdWindowWPtr;

#endif
