#ifndef QXRDEXPERIMENTTHREAD_PTR_H
#define QXRDEXPERIMENTTHREAD_PTR_H

#include <QSharedPointer>

class QxrdExperimentThread;

typedef QSharedPointer<QxrdExperimentThread> QxrdExperimentThreadPtr;
typedef QWeakPointer<QxrdExperimentThread> QxrdExperimentThreadWPtr;

#endif // QXRDEXPERIMENTTHREAD_H
