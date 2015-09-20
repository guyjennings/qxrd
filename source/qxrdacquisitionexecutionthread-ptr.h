#ifndef QXRDACQUISITIONEXECUTIONTHREADPTR_H
#define QXRDACQUISITIONEXECUTIONTHREADPTR_H

#include <QSharedPointer>

class QxrdAcquisitionExecutionThread;

typedef QSharedPointer<QxrdAcquisitionExecutionThread>  QxrdAcquisitionExecutionThreadPtr;
typedef QWeakPointer<QxrdAcquisitionExecutionThread>    QxrdAcquisitionExecutionThreadWPtr;

#endif // QXRDACQUISITIONEXECUTIONTHREADPTR_H

