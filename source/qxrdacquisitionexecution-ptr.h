#ifndef QXRDACQUISITIONEXECUTIONPTR_H
#define QXRDACQUISITIONEXECUTIONPTR_H

#include <QSharedPointer>

class QxrdAcquisitionExecution;

typedef QSharedPointer<QxrdAcquisitionExecution> QxrdAcquisitionExecutionPtr;
typedef QWeakPointer<QxrdAcquisitionExecution>   QxrdAcquisitionExecutionWPtr;

#endif // QXRDACQUISITIONEXECUTIONPTR_H

