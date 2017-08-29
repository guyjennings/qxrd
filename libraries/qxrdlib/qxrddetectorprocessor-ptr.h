#ifndef QXRDDETECTORPROCESSORPTR_H
#define QXRDDETECTORPROCESSORPTR_H

#include <QSharedPointer>

class QxrdDetectorProcessor;

typedef QSharedPointer<QxrdDetectorProcessor> QxrdDetectorProcessorPtr;
typedef QWeakPointer<QxrdDetectorProcessor>   QxrdDetectorProcessorWPtr;

#endif // QXRDDETECTORPROCESSORPTR_H

