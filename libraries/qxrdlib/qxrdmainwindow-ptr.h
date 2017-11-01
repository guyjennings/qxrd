#ifndef QXRDMAINWINDOWPTR_H
#define QXRDMAINWINDOWPTR_H

#include <QSharedPointer>

class QxrdMainWindow;

typedef QSharedPointer<QxrdMainWindow> QxrdMainWindowPtr;
typedef QWeakPointer<QxrdMainWindow>   QxrdMainWindowWPtr;

#endif // QXRDMAINWINDOWPTR_H
