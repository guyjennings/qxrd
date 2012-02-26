#ifndef QXRDACQUISITIONTHREAD_PTR_H
#define QXRDACQUISITIONTHREAD_PTR_H

#include <QSharedPointer>

class QxrdAcquisitionThread;

typedef QSharedPointer<QxrdAcquisitionThread> QxrdAcquisitionThreadPtr;
typedef QWeakPointer<QxrdAcquisitionThread> QxrdAcquisitionThreadWPtr;

#endif
