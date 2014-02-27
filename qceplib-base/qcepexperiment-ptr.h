#ifndef QCEPEXPERIMENT_PTR_H
#define QCEPEXPERIMENT_PTR_H

#include <QSharedPointer>

class QcepExperiment;

typedef QSharedPointer<QcepExperiment> QcepExperimentPtr;
typedef QWeakPointer<QcepExperiment>   QcepExperimentWPtr;

#endif // QCEPEXPERIMENTPTR_H
