#ifndef QXRDEXPERIMENTTHREADPTR_H
#define QXRDEXPERIMENTTHREADPTR_H

#include <QSharedPointer>

class QxrdExperimentThread;

typedef QSharedPointer<QxrdExperimentThread> QxrdExperimentThreadPtr;
typedef QWeakPointer<QxrdExperimentThread>   QxrdExperimentThreadWPtr;

#endif // QXRDEXPERIMENTTHREADPTR_H
