#ifndef QCEPDATASETPTR_H
#define QCEPDATASETPTR_H

#include <QSharedPointer>

class QcepDataset;
typedef QSharedPointer<QcepDataset> QcepDatasetPtr;
typedef QWeakPointer<QcepDataset> QcepDatasetWPtr;

#endif // QCEPDATASETPTR_H

