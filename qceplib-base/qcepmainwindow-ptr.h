#ifndef QCEPMAINWINDOWPTR_H
#define QCEPMAINWINDOWPTR_H

#include <QSharedPointer>

class QcepMainWindow;

typedef QSharedPointer<QcepMainWindow> QcepMainWindowPtr;
typedef QWeakPointer<QcepMainWindow>   QcepMainWindowWPtr;

#endif // QCEPMAINWINDOWPTR_H

