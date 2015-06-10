#ifndef QCEPDATASETMODELPTR_H
#define QCEPDATASETMODELPTR_H

#include <QSharedPointer>

class QcepDatasetModel;

typedef QSharedPointer<QcepDatasetModel> QcepDatasetModelPtr;
typedef QWeakPointer<QcepDatasetModel> QcepDatasetModelWPtr;

#endif // QCEPDATASETMODELPTR_H

