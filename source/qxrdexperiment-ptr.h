#ifndef QXRDEXPERIMENT_PTR_H
#define QXRDEXPERIMENT_PTR_H

#include <QSharedPointer>

class QxrdExperiment;
typedef QSharedPointer<QxrdExperiment> QxrdExperimentPtr;
typedef QWeakPointer<QxrdExperiment> QxrdExperimentWPtr;

#endif // QXRDEXPERIMENT_PTR_H
