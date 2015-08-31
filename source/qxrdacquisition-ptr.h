#ifndef QXRDACQUISITION_PTR_H
#define QXRDACQUISITION_PTR_H

#include <QSharedPointer>

class QxrdAcquisition;
typedef QSharedPointer<QxrdAcquisition> QxrdAcquisitionPtr;
typedef QWeakPointer<QxrdAcquisition> QxrdAcquisitionWPtr;

#endif
