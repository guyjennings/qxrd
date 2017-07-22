#ifndef QXRDTESTIMAGEGENERATORTHREADPTR_H
#define QXRDTESTIMAGEGENERATORTHREADPTR_H

#include <QSharedPointer>

class QxrdTestImageGeneratorThread;

typedef QSharedPointer<QxrdTestImageGeneratorThread> QxrdTestImageGeneratorThreadPtr;
typedef QWeakPointer<QxrdTestImageGeneratorThread>   QxrdTestImageGeneratorThreadWPtr;

#endif // QXRDTESTIMAGEGENERATORTHREADPTR_H
