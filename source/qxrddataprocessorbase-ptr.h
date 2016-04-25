#ifndef QXRDDATAPROCESSORBASEPTR_H
#define QXRDDATAPROCESSORBASEPTR_H

#include <QSharedPointer>

class QxrdDataProcessorBase;

typedef QSharedPointer<QxrdDataProcessorBase> QxrdDataProcessorBasePtr;
typedef QWeakPointer<QxrdDataProcessorBase>   QxrdDataProcessorBaseWPtr;

#endif // QXRDDATAPROCESSORBASEPTR_H
