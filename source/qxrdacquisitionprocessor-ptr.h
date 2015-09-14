#ifndef QXRDACQUISITIONPROCESSORPTR_H
#define QXRDACQUISITIONPROCESSORPTR_H

#include <QSharedPointer>

class QxrdAcquisitionProcessor;

typedef QSharedPointer<QxrdAcquisitionProcessor> QxrdAcquisitionProcessorPtr;
typedef QWeakPointer<QxrdAcquisitionProcessor>   QxrdAcquisitionProcessorWPtr;

#endif // QXRDACQUISITIONPROCESSORPTR_H

