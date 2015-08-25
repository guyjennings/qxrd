#ifndef QXRDACQUISITIONWINDOWPTR_H
#define QXRDACQUISITIONWINDOWPTR_H

#include <QSharedPointer>

class QxrdAcquisitionWindow;

typedef QSharedPointer<QxrdAcquisitionWindow>  QxrdAcquisitionWindowPtr;
typedef QWeakPointer<QxrdAcquisitionWindow>    QxrdAcquisitionWindowWPtr;

#endif // QXRDACQUISITIONWINDOWPTR_H

