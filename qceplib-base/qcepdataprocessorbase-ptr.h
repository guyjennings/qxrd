#ifndef QCEPDATAPROCESSORBASEPTR_H
#define QCEPDATAPROCESSORBASEPTR_H

#include <QSharedPointer>

class QcepDataProcessorBase;

typedef QSharedPointer<QcepDataProcessorBase> QcepDataProcessorBasePtr;
typedef QWeakPointer<QcepDataProcessorBase>   QcepDataProcessorBaseWPtr;

#endif // QCEPDATAPROCESSORBASEPTR_H
