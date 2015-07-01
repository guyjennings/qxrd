#ifndef QXRDDATASETPTR_H
#define QXRDDATASETPTR_H

#include <QSharedPointer>

class QxrdDataset;

typedef QSharedPointer<QxrdDataset> QxrdDatasetPtr;
typedef QWeakPointer<QxrdDataset>   QxrdDatasetWPtr;

#endif // QXRDDATASETPTR_H

